#ifndef YAMANIX_VOP3_BRIDGE_H
#define YAMANIX_VOP3_BRIDGE_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct yamanix_vop3 yamanix_vop3;
typedef struct {
    uint64_t writes[2], reads[2], unsupported_reads[2], sample_passes[2];
} yamanix_vop3_metrics;
uint32_t yamanix_vop3_abi_version(void);
yamanix_vop3 *yamanix_vop3_create(void);
void yamanix_vop3_destroy(yamanix_vop3 *handle);
int yamanix_vop3_write(yamanix_vop3 *, uint32_t chip, uint32_t reg, uint16_t value, uint16_t mask);
int yamanix_vop3_read(yamanix_vop3 *, uint32_t chip, uint32_t reg, uint16_t mask, uint16_t *value);
int yamanix_vop3_snapshot(yamanix_vop3 *, uint8_t *output, size_t length);
int yamanix_vop3_get_metrics(yamanix_vop3 *, yamanix_vop3_metrics *output);
// ABI v2, single-threaded. Profile 0 preserves zero for unsupported reads.
// Profile 1 is EXPERIMENTAL: selected EG level * 64, truncated and saturated to int16.
// advance() steps both chips at the core model's 48 kHz; NOT a verified AN1x clock.
// Each advance may run at most 4096 samples/chip. MMIO remains caller-ordered.
// No table ROM loading or audio routing is provided.
int yamanix_vop3_set_eg_readback(yamanix_vop3 *, uint32_t profile);
int yamanix_vop3_advance(yamanix_vop3 *, uint64_t absolute_ns);
// Returns 0 on success; -1 invalid arguments, -2 unsupported mask, -3 internal failure (discard handle), -4 unsupported index/time/budget.
#ifdef __cplusplus
}
#endif
#endif
