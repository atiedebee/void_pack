#ifndef VOID_PACK_H
#define VOID_PACK_H

#include <stddef.h>

void *void_pack(const char *format, ...);
size_t void_pack_static(const char *format, void *restrict buff, ...);

int void_unpack(const void *buff, ...);
void *void_unpack_var(const void *restrict buff, size_t index);

size_t void_pack_size(const char *format);
#endif
