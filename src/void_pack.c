#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdalign.h>

#include "void_pack.h"

#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic push


void *void_unpack_var(void *buff, size_t index)
{
	register size_t ii;
	size_t offset = 0;
	char *format;
	size_t sizeof_table[256] = {0};
	size_t alignof_table[256] = {0};
	
	sizeof_table['c'] = sizeof(char);
	sizeof_table['s'] = sizeof(short);
	sizeof_table['i'] = sizeof(int);
	sizeof_table['l'] = sizeof(long);
	sizeof_table['p'] = sizeof(void*);
	sizeof_table['f'] = sizeof(double);
	sizeof_table['d'] = sizeof(double);
	sizeof_table['D'] = sizeof(long double);
	
	alignof_table['c'] = alignof(char);
	alignof_table['s'] = alignof(short);
	alignof_table['i'] = alignof(int);
	alignof_table['l'] = alignof(long);
	alignof_table['p'] = alignof(void*);
	alignof_table['f'] = alignof(double);
	alignof_table['d'] = alignof(double);
	alignof_table['D'] = alignof(long double);
	
	
	if( buff == NULL )
	{
		return NULL;
	}
	
	format = (char*)buff;
	while( *(char*)buff != '\0')
	{
		buff += sizeof(char);
	}
	buff += sizeof(char);

#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic push
	for( ii = 0; ii < index && format[ii] != '\0'; ii++)
	{
		offset += sizeof_table[ format[ii] ];
		offset += offset % alignof_table[ format[ii] ];
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
	buff = malloc( ((format_len + 1) * sizeof(char)) + (format_len * 16) );
	if( !buff )
	{
		return NULL;
	}
	
	for( ii = 0; format[ii] != '\0'; ii++)
	{
		*(char*)(buff + ii) = format[ii];
	}
	*(char*)(buff + ii) = '\0';
	offset = ii+1;
	
	/*7 different types*/
	va_start(list, format);	
	for( ii = 0; format[ii] != '\0'; ii++ )
	{
		switch( format[ii] )
		{
			case 'c':
				*(char*)(buff + offset) = va_arg(list, int);
				offset += sizeof(char);
				offset += offset%alignof(char);
				break;
			case 's':
				*(short*)(buff + offset) = va_arg(list, int);
				offset += sizeof(short);
				offset += offset%alignof(short);
				break;
			case 'i':
				*(int*)(buff + offset) = va_arg(list, int);
				offset += sizeof(int);
				offset += offset%alignof(int);
				break;
			case 'l':
				*(long*)(buff + offset) = va_arg(list, long);
				offset += sizeof(long);
				offset += offset%alignof(long);
				break;
				
			case 'f':
			case 'd':
				*(double*)(buff + offset) = va_arg(list, double);
				offset += sizeof(double);
				offset += offset%alignof(double);
				break;
			case 'D':
				*(long double*)(buff + offset) = va_arg(list, long double);
				offset += sizeof(long double);
				offset += offset%alignof(long double);
				break;
				
			case 'p':
			default:
				*(void**)(buff + offset) = va_arg(list, void*);
				offset += sizeof(void*);
				offset += offset%alignof(void*);
				break;
		}
	}
	
	va_end(list);

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
				*(char*)temp = *(char*)(buff + offset);
				offset += sizeof(char);
				offset += offset%alignof(char);
				break;
			case 's':
				*(short*)temp = *(short*)(buff + offset);
				offset += sizeof(short);
				offset += offset%alignof(short);
				break;
			case 'i':
				*(int*)temp = *(int*)(buff + offset);
				offset += sizeof(int);
				offset += offset%alignof(int);
				break;
			case 'l':
				*(long*)temp = *(long*)(buff + offset);
				offset += sizeof(long);
				offset += offset%alignof(long);
				break;
				
			case 'f':
			case 'd':
				*(double*)temp = *(double*)(buff + offset);
				offset += sizeof(double);
				offset += offset%alignof(double);
				break;
			case 'D':
				*(long double*)temp = *(long double*)(buff + offset);
				offset += sizeof(long double);
				offset += offset%alignof(long double);
				break;
				
			case 'p':
			default:
				*(void**)temp = *(void**)(buff + offset);
				offset += sizeof(void*);
				offset += offset%alignof(void*);
				break;
		}
	}
	va_end(list);
	return 0;
}

