# void_pack
library for packing variables into a void* pointer.

## Building
### Unix-likes
```
git clone https://github.com/atiedebee/void_pack.git
cd void_pack
```
If you're going to use the library as a shared object file, use ``make shared``. For statically linking use ``make static``

## Usage
### packing
Use ``void_pack(const char* format, ...);`` to pack variables into a ``void*``. <br>
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
For unpacking the data you use ``void_unpack(void *data, ...);``. <br>
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

If you only want a single variable from your packed data, you can use ``void_unpack_var(void *data, size_t index)``. <br>
This function returns the <b>address</b> of the index'th variable in the data (starting at 0). It's recommended to use ``void_unpack`` over this function, as it goes through the data once when accessing multiple variables.
Usage of this function may look like the following:
```c
int x = 1, y = 3;
void *data = void_pack("ii", x, y);
...
int z;
z = *(int*)void_unpack_var(data, 0); /*z is set to 1*/
z = *(int*)void_unpack_var(data, 1); /*z is set to 3*/
```
