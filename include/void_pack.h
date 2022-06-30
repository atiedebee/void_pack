#ifndef VOID_PACK_H
#define VOID_PACK_H

#include <stddef.h>

void *void_pack(const char *format, ...);
int void_unpack(void *buff, ...);
__attribute__((deprecated))
void *void_unpack_var(void *buff, size_t index);

#endif
