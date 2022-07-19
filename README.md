# void_pack
library for packing variables into a void* pointer.

## Building
### Unix-likes
```
git clone https://github.com/atiedebee/void_pack.git
cd void_pack
```
If you're going to use the library as a shared object file, use ``make shared``. For statically linking use ``make static``<br>
Or just use ``make`` for both.<br>
Add ``debug`` before your target to compile with ``-DDEBUG`` which makes the library to print error messages to stderr. For example: ``make debug shared``

## Usage
### packing
Use ``void* void_pack(const char* format, ...);`` to pack variables into a ``void*``. <br>
``format`` is a string of characters that shows the type of variables being packed. Your options are:

| character | type |
|-----------|------|
| c | char |
| s | short |
| i | int |
| f | float |
| d | double |
| D | long double |
| p | void* |

note: float are promoted to doubles when using variable arguments, so they can be used interchangably.<br>
Using this function may look something like this:
```c
int x = 5;
float y = 3.0;
int *pointer = &x;
void* packed_data = void_pack("ifp", x, y, pointer);
... /*packed data can now be used*/
```

### unpacking
For unpacking the data you use ``int void_unpack(void \*data, ...);``. <br>
You don't need to add a format string to unpack variables, as this is stored in the pointer retrieved from ``void_pack``.

``void_unpack`` requires you to pass pointers of the types you want. Usage is as shown:
```c
int a = 1, b = 2;
void *data = void_pack("ii", a, b);
...
int x, y;
void_unpack(data, &x, &y); 
/*the value of a will be stored in x, and the value of b will be stored in y*/
free(data);
```
``void_unpack`` doesn't free the memory located at ``data``, this is to make it a little more flexible to use.

### modifying data or accessing a single element
It's possible to get the address of the n'th variable stored in a void* (starting at 1). This can be done with ``void* void_unpack_var(void *data, size_t index);``. <br>
I only recommend using this function if you're very carefull, since it leaves a lot of possibilities for human error (just like the rest of the library, but especially this one). <br>
Some simple usage of this function may look like the following:
```c
void *data = void_pack("ii", 55, 3);
...
int *x = (int*)void_unpack_var(data, 1); //returns the address of the first variable, with value 55
*x = 23;
int y = *(int*)void_unpack_var(data, 1); // y == 23
int z = *(int*)void_unpack_var(data, 2); // z == 3
```
``void_unpack_var`` returns NULL on failure. Compile the executable with the -DDEBUG flag to catch runtime errors more easily (information will be printed to stderr)
