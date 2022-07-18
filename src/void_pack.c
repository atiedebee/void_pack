#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "void_pack.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"

// __attribute__((deprecated))
void *void_unpack_var(void *buff, size_t index) 
{
	register size_t ii;
	char *format;
	uintptr_t var, returnval;
	size_t sizeof_table[256] = {
		['c'] = sizeof(char),
		['s'] = sizeof(short),
		['i'] = sizeof(int),
		['l'] = sizeof(long),
		['f'] = sizeof(double),
		['d'] = sizeof(double),
		['D'] = sizeof(long double),
		['p'] =	sizeof(void*)
	};
	size_t alignof_table[256] = {
		['c'] = _Alignof(char),
		['s'] = _Alignof(short),
		['i'] = _Alignof(int),
		['l'] = _Alignof(long),
		['f'] = _Alignof(double),
		['d'] = _Alignof(double),
		['D'] = _Alignof(long double),
		['p'] =	_Alignof(void*)
	};
	/*
	 FIXME
	 This function doesn't work at all (yet)
	 */
	if( buff == NULL || index < 1 )
	{
		return NULL;
	}
	
	format = (char*)buff;
	for( ii = 0; *(char*)(buff+ii) != '\0'; ii++ )
		;
	var = (uintptr_t)buff + ii + 1;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wchar-subscripts"
	returnval = var;
	for( ii = 0; ii < index && format[ii] != '\0'; ii++)
	{
// 		printf("alignof: %ld\nsizeof: %ld\n", alignof_table[ format[ii] ], sizeof_table[ format[ii] ]);
		var += (size_t)var % alignof_table[ format[ii] ];
		returnval = var;
// 		printf("%d\n", *(int*)var);
		var += sizeof_table[ format[ii] ];
	}
#pragma GCC diagnostic pop
	return (void*)returnval;
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
				offset += ((size_t)buff+offset)%_Alignof(char);
				*(char*)(buff + offset) = va_arg(list, int);
				offset += sizeof(char);
				break;
			case 's':
				offset += ((size_t)buff+offset)%_Alignof(short);
				*(short*)(buff + offset) = va_arg(list, int);
				offset += sizeof(short);
				break;
			case 'i':
				offset += ((size_t)buff+offset)%_Alignof(int);
				*(int*)(buff + offset) = va_arg(list, int);
				offset += sizeof(int);
				break;
			case 'l':
				offset += ((size_t)buff+offset)%_Alignof(long);
				*(long*)(buff + offset) = va_arg(list, long);
				offset += sizeof(long);
				break;
				
			case 'f':
			case 'd':
				offset += ((size_t)buff+offset)%_Alignof(double);
				*(double*)(buff + offset) = va_arg(list, double);
				offset += sizeof(double);
				break;
			case 'D':
				offset += ((size_t)buff+offset)%_Alignof(long double);
				*(long double*)(buff + offset) = va_arg(list, long double);
				offset += sizeof(long double);
				break;
				
			case 'p':
				offset += ((size_t)buff+offset)%_Alignof(void*);
				*(void**)(buff + offset) = va_arg(list, void*);
				offset += sizeof(void*);
				break;
			default:
				return NULL;
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
				offset += ((size_t)buff+offset)%_Alignof(char);
				*(char*)temp = *(char*)(buff + offset);
				offset += sizeof(char);
				break;
			case 's':
				offset += ((size_t)buff+offset)%_Alignof(short);
				*(short*)temp = *(short*)(buff + offset);
				offset += sizeof(short);
				break;
			case 'i':
				offset += ((size_t)buff+offset)%_Alignof(int);
				*(int*)temp = *(int*)(buff + offset);
				offset += sizeof(int);
				break;
			case 'l':
				offset += ((size_t)buff+offset)%_Alignof(long);
				*(long*)temp = *(long*)(buff + offset);
				offset += sizeof(long);
				break;
				
			case 'f':
			case 'd':
				offset += ((size_t)buff+offset)%_Alignof(double);
				*(double*)temp = *(double*)(buff + offset);
				offset += sizeof(double);
				break;
			case 'D':
				offset += ((size_t)buff+offset)%_Alignof(long double);
				*(long double*)temp = *(long double*)(buff + offset);
				offset += sizeof(long double);
				break;
				
			case 'p':
			default:
				offset += ((size_t)buff+offset)%_Alignof(void*);
				*(void**)temp = *(void**)(buff + offset);
				offset += sizeof(void*);
				break;
		}
	}
	va_end(list);
	return 0;
}

