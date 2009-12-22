#include "picostdlib.h"

/* Stop linker from complaining about unresolved external symbol __fltused.
 * This symbol is injected by the compiler if we use any floating point stuff,
 * to ensure that a floating point library is linked in.
 * We don't use any floats, but still this seems to be necessary.
 */
int _fltused = 0;

void *memcpy(void *dst, void const *src, unsigned count) {
	void *ret = dst;
    while (count--) {
        *(char*)dst = *(char*)src;
        dst = (char*)dst + 1;
        src = (char*)src + 1;
    }
	return ret;
}
