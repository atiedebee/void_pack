#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "void_pack.h"

#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic push

__attribute__((deprecated))
void *void_unpack_var(void *buff, size_t index) 
{
	register size_t ii;
	size_t offset = 0, returnval = 0;
	char *format;
	size_t sizeof_table[256] = {0};
	size_t alignof_table[256] = {0};
	/*
	 FIXME
	 This function doesn't work at all (yet)
	 */
	sizeof_table['c'] = 		sizeof(char);
	sizeof_table['s'] = 		sizeof(short);
	sizeof_table['i'] = 		sizeof(int);
	sizeof_table['l'] = 		sizeof(long);
	sizeof_table['f'] = 		sizeof(double);
	sizeof_table['d'] = 		sizeof(double);
	sizeof_table['D'] = 		sizeof(long double);
	sizeof_table['p'] =			sizeof(void*);
	
	
	alignof_table['c'] = 		_Alignof(char);
	alignof_table['s'] = 		_Alignof(short);
	alignof_table['i'] = 		_Alignof(int);
	alignof_table['l'] = 		_Alignof(long);
	alignof_table['f'] = 		_Alignof(double);
	alignof_table['d'] = 		_Alignof(double);
	alignof_table['D'] = 		_Alignof(long double);
	alignof_table['p'] =		_Alignof(void*);
	
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
		offset += offset % alignof_table[ format[ii] ];
		returnval = offset;
		offset += sizeof_table[ format[ii] ];
	}
	
	return buff + returnval;
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
			default:
				offset += ((size_t)buff+offset)%_Alignof(void*);
				*(void**)(buff + offset) = va_arg(list, void*);
				offset += sizeof(void*);
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

