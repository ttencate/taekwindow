#ifndef PICOSTDLIB_H
#define PICOSTDLIB_H

#include <stdlib.h>

/* Implementation of some stdlib functions that we really can't do without,
 * but we don't want to link in the CRT for.
 * (The CRT adds about 50 kB to the final executable.)
 */

/* Stop linker from complaining about unresolved external symbol __fltused.
 * This symbol is injected by the compiler if we use any floating point stuff,
 * to ensure that a floating point library is linked in.
 * We don't use any floats, but still this seems to be necessary.
 */
int _fltused = 0;

/* Implementing memcpy as a compiler intrinsic doesn't seem to work.
 * So use our own implementation instead.
 * It's not used often, so it doesn't need to be highly optimized.
 */
void *memcpy(void *dst, void const *src, size_t count);
#pragma function(memcpy)

#endif
