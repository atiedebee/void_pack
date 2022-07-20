#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "void_pack.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"

#ifndef VOID_PACK_ALLOCATOR
#define VOID_PACK_ALLOCATOR malloc
#else
void* VOID_PACK_ALLOCATOR(size_t);
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wchar-subscripts"


const size_t sizeof_table[CHAR_MAX] = {
	['c'] = sizeof(char),
	['s'] = sizeof(short),
	['i'] = sizeof(int),
	['l'] = sizeof(long),
	['f'] = sizeof(double),
	['d'] = sizeof(double),
	['D'] = sizeof(long double),
	['p'] =	sizeof(void*)
};
const size_t alignof_table[CHAR_MAX] = {
	['c'] = _Alignof(char),
	['s'] = _Alignof(short),
	['i'] = _Alignof(int),
	['l'] = _Alignof(long),
	['f'] = _Alignof(double),
	['d'] = _Alignof(double),
	['D'] = _Alignof(long double),
	['p'] =	_Alignof(void*)
};
	
size_t void_pack_size(const char *format)
{
	if( format == NULL )
		return 0;
	
	size_t formatlen = strlen(format);
	size_t void_size = formatlen + 1;
	
	for( size_t i = 0; i < formatlen; i++ ){
		void_size += alignof_table[format[i]] - void_size%alignof_table[format[i]];
		void_size += sizeof_table[format[i]];
	}
	
	return void_size;
}



void *void_unpack_var(const void *buff, size_t index) 
{
	register uintptr_t ii;
	char *format;
	uintptr_t var, returnval;
	
	if( buff == NULL || index < 1 ){
#ifdef DEBUG
		fprintf(stderr, "[%s] invalid arguments: (%s)\n", __func__, buff == NULL?"buff == NULL":"index < 1");
#endif
		return NULL;
	}

	format = (char*)buff;
	for( ii = 0; *(char*)(buff+ii) != '\0'; ii++ )
		;
	
	if( index > ii ){
#ifdef DEBUG
		fprintf(stderr, "[%s] index out of bounds (accessing element %ld/%ld)\n", __func__, index, ii);
#endif
		return NULL;
	}
	
	var = (uintptr_t)buff + ii + 1;
	returnval = var;
	for( ii = 0; ii < index && format[ii] != '\0'; ii++)
	{
		var += alignof_table[ format[ii] ] - (uintptr_t)var % alignof_table[ format[ii] ];
		returnval = var;
		var += sizeof_table[ format[ii] ];
	}
#pragma GCC diagnostic pop
	return (void*)returnval;
}

static 
size_t fill_void_(const char* format, void* buff, va_list list)
{
	register size_t ii, offset;
	
	for( ii = 0; format[ii] != '\0'; ii++)
		*(char*)(buff + ii) = format[ii];
	*(char*)(buff + ii) = '\0';
	offset = ii+1;
	
	/*7 different types*/
	for( ii = 0; format[ii] != '\0'; ii++ )
	{
		switch( format[ii] )
		{
			case 'c':
				offset += _Alignof(char) - ((uintptr_t)buff+offset)%_Alignof(char);
				*(char*)(buff + offset) = va_arg(list, int);
				offset += sizeof(char);
				break;
			case 's':
				offset += _Alignof(short) - ((uintptr_t)buff+offset)%_Alignof(short);
				*(short*)(buff + offset) = va_arg(list, int);
				offset += sizeof(short);
				break;
			case 'i':
				offset += _Alignof(int) - ((uintptr_t)buff+offset)%_Alignof(int);
				*(int*)(buff + offset) = va_arg(list, int);
				offset += sizeof(int);
				break;
			case 'l':
				offset += _Alignof(long) - ((uintptr_t)buff+offset)%_Alignof(long);
				*(long*)(buff + offset) = va_arg(list, long);
				offset += sizeof(long);
				break;
				
			case 'f':
			case 'd':
				offset += _Alignof(double) - ((uintptr_t)buff+offset)%_Alignof(double);
				*(double*)(buff + offset) = va_arg(list, double);
				offset += sizeof(double);
				break;
			case 'D':
				offset += _Alignof(long double) - ((uintptr_t)buff+offset)%_Alignof(long double);
				*(long double*)(buff + offset) = va_arg(list, long double);
				offset += sizeof(long double);
				break;
				
			case 'p':
				offset += _Alignof(void*) - ((uintptr_t)buff+offset)%_Alignof(void*);
				*(void**)(buff + offset) = va_arg(list, void*);
				offset += sizeof(void*);
				break;
			default:
#ifdef DEBUG
				fprintf(stderr, "[%s] received unrecognised type specifier '%c' in format \"%s\"\n", __func__, format[ii], format);
#endif
				return 0;
		}
	}
	
	return offset; /*offset has the current size*/
}


size_t void_pack_static(const char *format, void *restrict buff, ...)
{
	va_list list;
	size_t size;
	
	if( !buff || !format )
		return 0;
	
	va_start(list, buff);	
	size = fill_void_(format, buff, list);
	va_end(list);

	return size;
}


void *void_pack(const char *format, ...)
{
	va_list list;
	void *buff = NULL;
	uintptr_t format_len;
	
	
	format_len = strlen(format);
	buff = VOID_PACK_ALLOCATOR( ((format_len + 1) * sizeof(char)) + (format_len * 16) );
	if( !buff )
		return NULL;
	
	
	va_start(list, format);	
	if( fill_void_(format, buff, list) == 0 ){
		free(buff);
		va_end(list);
		return NULL;
	}
	va_end(list);

	return buff;
}


int void_unpack(const void *restrict buff, ...)
{
	va_list list;
	char format[256] = {0};
	register uintptr_t ii, offset;
	void *temp;
	
	if( !buff )
	{
		return -1;
	}
	
	for( ii = 0; *(char*)(buff + (ii * sizeof(char))) != '\0' && ii < 256; ii++)
	{
		format[ii] = *(char*)(buff + (ii * sizeof(char)));
	}
	format[ii] = '\0';
	offset = ii + 1;
	
	
	va_start(list, buff);
	for( ii = 0; format[ii] != '\0'; ii++ )
	{
		temp = va_arg(list, void*);
		
		switch( format[ii] )
		{
			case 'c':
				offset += _Alignof(char) - ((uintptr_t)buff+offset)%_Alignof(char);
				*(char*)temp = *(char*)(buff + offset);
				offset += sizeof(char);
				break;
			case 's':
				offset += _Alignof(short) - ((uintptr_t)buff+offset)%_Alignof(short);
				*(short*)temp = *(short*)(buff + offset);
				offset += sizeof(short);
				break;
			case 'i':
				offset += _Alignof(int) - ((uintptr_t)buff+offset)%_Alignof(int);
				*(int*)temp = *(int*)(buff + offset);
				offset += sizeof(int);
				break;
			case 'l':
				offset += _Alignof(long) - ((uintptr_t)buff+offset)%_Alignof(long);
				*(long*)temp = *(long*)(buff + offset);
				offset += sizeof(long);
				break;
				
			case 'f':
			case 'd':
				offset += _Alignof(double) - ((uintptr_t)buff+offset)%_Alignof(double);
				*(double*)temp = *(double*)(buff + offset);
				offset += sizeof(double);
				break;
			case 'D':
				offset += _Alignof(long double) - ((uintptr_t)buff+offset)%_Alignof(long double);
				*(long double*)temp = *(long double*)(buff + offset);
				offset += sizeof(long double);
				break;
				
			case 'p':
			default:
				offset += _Alignof(void*) - ((uintptr_t)buff+offset)%_Alignof(void*);
				*(void**)temp = *(void**)(buff + offset);
				offset += sizeof(void*);
				break;
		}
	}
	va_end(list);
	return 0;
}

