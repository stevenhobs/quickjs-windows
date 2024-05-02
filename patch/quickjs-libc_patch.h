/***
 * This header file is for patching the js module .so on the Windows temporary.
 * Usage: Just include this in the quickjs-libc.c: row 464 & row 522 
*/
#if defined(_WIN32)
#undef _WIN32
#define dlopen(filename, foo) LoadLibrary(filename)
#define dlsym (JSInitModuleFunc*)GetProcAddress
#define dlclose FreeLibrary
#else
#define _WIN32
#endif