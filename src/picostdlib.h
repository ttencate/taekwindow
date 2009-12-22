#ifndef PICOSTDLIB_H
#define PICOSTDLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Implementation of some stdlib functions that we really can't do without,
 * but we don't want to link in the CRT for.
 * (The CRT adds about 50 kB to the final executable.)
 */

/* Unfortunately, both #pragma intrinsic and /Oi are now merely hints,
 * so we cannot force the compiler to use an intrinsic.
 * So use our own implementation instead.
 * It's not used often, so it doesn't need to be highly optimized.
 */
void *memcpy(void *dst, const void *src, unsigned count);
#pragma function(memcpy)

#ifdef __cplusplus
}
#endif

#endif
