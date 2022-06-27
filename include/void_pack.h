#ifndef VOID_PACK_H
#define VOID_PACK_H

void *void_pack(const char *format, ...);
int void_unpack(void *buff, ...);
void *void_unpack_var(void *buff, size_t index);

#endif
