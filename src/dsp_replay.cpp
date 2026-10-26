// Offline host-register upload audit. This never synthesizes audio or supplies H8 responses.
#include "vop3.h"
#include <array>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

static uint64_t number(const std::string& text, uint64_t maximum) {
    uint64_t value = 0;
    auto result = std::from_chars(text.data(), text.data() + text.size(), value);
    if (result.ec != std::errc{} || result.ptr != text.data() + text.size() || value > maximum)
        throw std::runtime_error("Invalid unsigned field: " + text);
    return value;
}

static void word(std::vector<uint8_t>& bytes, uint16_t value) {
    bytes.push_back(uint8_t(value >> 8)); bytes.push_back(uint8_t(value));
}

int main(int argc, char** argv) {
    try {
        if (argc != 3) throw std::runtime_error("Usage: yamanix_dsp_replay events.tsv output-state.bin");
        std::ifstream input(argv[1]);
        if (!input) throw std::runtime_error("Cannot open event input");
        std::string line;
        if (!std::getline(input, line) || line != "# YamANIx DSP events v1: seq time_ns op chip reg data mask pc (decimal)")
            throw std::runtime_error("Missing event format header");
        std::array<std::unique_ptr<Vop3>, 2> chips = {std::make_unique<Vop3>(), std::make_unique<Vop3>()};
        uint64_t expected = 0, previous_time = 0, writes = 0, reads = 0;
        bool ended = false;
        while (std::getline(input, line)) {
            if (ended) throw std::runtime_error("Data after event footer");
            if (line.starts_with("# END events=")) {
                if (number(line.substr(13), std::numeric_limits<uint64_t>::max()) != expected)
                    throw std::runtime_error("Event footer count mismatch");
                ended = true;
                continue;
            }
            std::istringstream row(line);
            std::array<std::string, 8> fields;
            for (auto& field : fields) if (!(row >> field)) throw std::runtime_error("Truncated event row");
            std::string extra;
            if (row >> extra) throw std::runtime_error("Trailing event fields");
            const uint64_t seq = number(fields[0], std::numeric_limits<uint64_t>::max());
            const uint64_t time = number(fields[1], std::numeric_limits<uint64_t>::max());
            const unsigned chip = unsigned(number(fields[3], 1));
            const unsigned reg = unsigned(number(fields[4], 127));
            const uint16_t data = uint16_t(number(fields[5], 65535));
            const unsigned mask = unsigned(number(fields[6], 65535));
            number(fields[7], 0xffffff);
            if (seq != expected++ || time < previous_time) throw std::runtime_error("Sequence gap or reversed time");
            if (mask != 65535) throw std::runtime_error("Partial-word DSP access is not implemented");
            previous_time = time;
            if (fields[2] == "W") { chips[chip]->write(reg, data); ++writes; }
            else if (fields[2] == "R") ++reads; // Passive reads are not hardware truth; do not feed them to the core.
            else throw std::runtime_error("Unknown operation");
        }
        if (!input.eof() || !expected || !ended) throw std::runtime_error("Empty, truncated or unreadable event stream");
        std::vector<uint8_t> snapshot;
        for (const auto& chip : chips) {
            word(snapshot, chip->select);
            for (const auto& row : chip->core.prog) for (auto value : row) word(snapshot, value);
            for (auto value : chip->core.coef) word(snapshot, value);
            for (auto value : chip->core.tag) snapshot.push_back(value);
            for (const auto& row : chip->core.slot) for (auto value : row) word(snapshot, value);
        }
        std::ofstream output(argv[2], std::ios::binary);
        output.write(reinterpret_cast<const char*>(snapshot.data()), std::streamsize(snapshot.size()));
        if (!output) throw std::runtime_error("Cannot write snapshot");
        std::cout << "{\"events\":" << expected << ",\"writes_replayed\":" << writes
                  << ",\"passive_reads_skipped\":" << reads << ",\"snapshot_bytes\":" << snapshot.size()
                  << ",\"audio_rendered\":false,\"dsp_samples_executed\":0}\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "DSP replay error: " << error.what() << '\n';
        return 2;
    }
}
