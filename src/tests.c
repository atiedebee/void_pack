#include "void_pack.h"

#include <stdio.h>
#include <assert.h>

void test1()/*simple test for packing multiple numbers*/
{
	void *data = void_pack("iiiiiiiiii", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	int arr[10];
	int i;
	
	void_unpack(data, &arr[0], &arr[1], &arr[2], &arr[3], &arr[4], &arr[5], &arr[6], &arr[7], &arr[8], &arr[9]);
	
	for( i = 0; i < 10; i++ ){
		assert(arr[i] == i);
	}
	
}

void test2()/*test for packing variables of different sizes*/
{
	void *data = void_pack("cscscs", 0, 1, 2, 3, 4, 5);
	char chars[3];
	short shorts[3];
	
	void_unpack(data, &chars[0], &shorts[0], &chars[1], &shorts[1], &chars[2], &shorts[2]);
	
	assert(chars[0] == 0);	
	assert(shorts[0] == 1);
	assert(chars[1] == 2);	
	assert(shorts[1] == 3);
	assert(chars[2] == 4);	
	assert(shorts[2] == 5);
}


void test3()/*test for packing variables with different alignments*/
{
	void *data = void_pack("cDic", 'a', 2.0l, 4, 'b');
	char c1, c2;
	long double ld;
	int i;
	
	void_unpack(data, &c1, &ld, &i, &c2);
	assert(c1 == 'a');
	assert(c2 == 'b');
	assert(ld == 2.0l);
	assert(i == 4);
	
}

void test4()/* void_unpack_var */
{
	void *data = void_pack("ii", 5, 3);
	int a, b;
	
	a = *(int*)void_unpack_var(data, 1);
	b = *(int*)void_unpack_var(data, 2);
	assert(a == 5);
	assert(b == 3);
}


void test5()/* void_unpack_var, but more complicated */
{
	void *data = void_pack("cDic", 'a', 2.0l, 4, 'b');
	char c1, c2;
	long double ld;
	int i;
	
	void_unpack(data, &c1, &ld, &i, &c2);
	c1 = *(char*)void_unpack_var(data, 1);
	ld = *(long double*)void_unpack_var(data, 2);
	i = *(int*)void_unpack_var(data, 3);
	c2 = *(char*)void_unpack_var(data, 4);
	
	assert(c1 == 'a');
	assert(c2 == 'b');
	assert(ld == 2.0l);
	assert(i == 4);
}

int main(void)
{
	void (*tests[])() = {
		test1, test2, test3, test4, test5
	};
	unsigned long i;
	
	for( i = 0; i < sizeof(tests)/sizeof(tests[0]); i++ ){
		printf("Test nr %ld\n", i+1);
		tests[i]();
	}
	printf("All tests succesful\n");
	return 0;
}
