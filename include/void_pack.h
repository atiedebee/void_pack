#ifndef VOID_PACK_H
#define VOID_PACK_H

#include <stddef.h>

void *void_pack(const char *format, ...);
int void_unpack(const void *buff, ...);
void *void_unpack_var(const void *buff, size_t index);

#endif
