#include "picostdlib.h"

void *memcpy(void *dst, void const *src, unsigned count) {
	void *ret = dst;
    while (count--) {
        *(char*)dst = *(char*)src;
        dst = (char*)dst + 1;
        src = (char*)src + 1;
    }
	return ret;
}