# ROM-free build baseline

The separate complete MAME research target has a verified macOS arm64 baseline:
Xcode 16.2 (16C5032a), AppleClang 16.0.0 (`clang-1600.0.26.6`), Python 3.13.15,
GNU Make 3.81, `OPTIMIZE=0`, all eleven patches, architecture and registration
smoke PASS. See [dated evidence](source-identity-recovery.md#verified-result).
This is distinct from the core/helper build below and from firmware acceptance.

Use C++20, CMake 3.20+ and Python 3.9+ for compatibility. These minimum versions
are not an exact toolchain lock. No source or firmware download occurs during
configuration. VOP3/AsmJit source hashes are checked by the repository auditor.

Verified local reference on 2026-10-27: Windows x64, Visual Studio 2022 Build
Tools, MSVC identification 19.44.35229.0, MSVC tools directory 14.44.35207,
Windows SDK 10.0.26100.0, CMake 3.31.6-msvc6, Python 3.13.3, Release build.
The existing symlink unit test cannot create links on this local account
(WinError 1314); that test was not passed locally. Do not replace this result
with a claim that the full local suite passed.

PR #16 CI run [40454166792](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792)
recorded GNU 13.3.0 with Python 3.13.16 on ubuntu-24.04 x64 and AppleClang
15.0.0.15000309 with Python 3.13.15 on macos-14-arm64. These are observed
versions for that revision, not persistent locks of hosted runner contents.
Windows CI used MSVC 19.51.36260.0 and Python 3.13.16 on windows-2025 x64.
All three CI jobs passed all eleven CTest groups, including the symlink test
that could not execute on the local Windows account.
The first plugin target is macOS arm64. Windows/Linux remain ROM-free build
targets; a successful core build does not establish plugin compatibility.

```sh
cmake --version
python --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
python tools/repository_audit.py
```

On Windows select the installed VS 2022 generator with `-A x64`; the Release
configuration is selected at build/test time. On Apple Silicon use an arm64
terminal/compiler and `-DCMAKE_OSX_ARCHITECTURES=arm64`. Confirm the compiler
identification and generated `CMakeCXXCompiler.cmake` before comparing results.
If the local temporary directory is unwritable, point TEMP/TMP at a writable
ignored build directory. A missing symlink privilege is an environment limit;
do not silently skip the security regression when certifying a complete run.

For every future reference build retain: repository SHA, compiler ID/version,
CMake/Python version, architecture, SDK/deployment target, build configuration,
and the CI run or local test result. Do not commit personal paths or raw local
firmware logs. Current CI logs compiler/CMake identity during configuration;
pinning a macOS image/toolchain and a complete product build environment remains
an M0 follow-up. Bit-for-bit binary reproducibility is not established here.
