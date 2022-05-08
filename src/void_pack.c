#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "void_pack.h"

#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic push

void *void_unpack_var(void *buff, size_t index)
{
	register size_t ii;
	size_t offset = 0;
	char *format;
	const size_t offset_table[256] = { 
		#include "sizes.txt"
	};
	
	format = buff;
	while( *(char*)buff != '\0')
	{
		buff += sizeof(char);
	}
	buff += sizeof(char);

#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic push
	for( ii = 0; ii < index; ii++)
	{
		offset += offset_table[ format[ii] ];
	}
	
	return buff + offset;
#pragma GCC diagnostic pop
}


void *void_pack(const char *format, ...)
{
	va_list list;
	void *buff = NULL;
	register size_t ii, offset, format_len;
	
	
	format_len = strlen(format);
	buff = malloc( (format_len * sizeof(char)) + (format_len * 8) );
	if( !buff )
	{
		return NULL;
	}
	
	for( ii = 0; format[ii] != '\0'; ii++)
	{
		*(char*)(buff + ii) = format[ii];
	}
	*(char*)(buff + ii) = format[ii];
	offset = format_len + 1;
	
	va_start(list, format);
	
	for( ii = 0; format[ii] != '\0'; ii++ )
	{
		switch( format[ii] )
		{
			case 'c':
			case 's':
			case 'i':
				*(int*)(buff + offset) = va_arg(list, int);
				offset += sizeof(int);
				break;
			case 'u':
				*(unsigned int*)(buff + offset) = va_arg(list, unsigned int);
				offset += sizeof(unsigned int);
				break;
				
			case 'l':
				*(long*)(buff + offset) = va_arg(list, long);
				offset += sizeof(long);
				break;
			case 'f':
			case 'd':
				*(double*)(buff + offset) = va_arg(list, double);
				offset += sizeof(double);
				break;
			case 'D':
				*(long double*)(buff + offset) = va_arg(list, long double);
				offset += sizeof(long double);
				break;
				
			case 'p':
			case '*':
			default:
				*(void**)(buff + offset) = va_arg(list, void*);
				offset += sizeof(void*);
				break;
		}
	}
	va_end(list);

#ifdef VOID_PACK_MEMORY_EFFICIENCY
	buff = realloc(buff, offset);
#endif

	return buff;
}


int void_unpack(void *buff, ...)
{
	va_list list;
	char format[256] = {0};
	register size_t ii, offset;
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
			case 's':
			case 'i':
				*(int*)temp = *(int*)(buff + offset);
				offset += sizeof(int);
				break;
			case 'u':
				*(unsigned int*)temp = *(unsigned int*)(buff + offset);
				offset += sizeof(unsigned int);
				break;
				
			case 'l':
				*(long*)temp = *(long*)(buff + offset);
				offset += sizeof(long);
				break;
			case 'f':
			case 'd':
				*(double*)temp = *(double*)(buff + offset);
				offset += sizeof(double);
				break;
			case 'D':
				*(long double*)temp = *(long double*)(buff + offset);
				offset += sizeof(long double);
				break;
				
			case 'p':
			case '*':
			default:
				*(void**)temp = *(void**)(buff + offset);
				offset += sizeof(void*);
				break;
		}
	}
	va_end(list);
	return 0;
}
