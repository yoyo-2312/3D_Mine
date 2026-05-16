#ifdef __MINGW32__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include <cstdio>

extern "C" FILE* __cdecl __iob_func(void)
{
    static FILE files[] = { *stdin, *stdout, *stderr };
    return files;
}

extern "C" {
void* __imp___iob_func = reinterpret_cast<void*>(&__iob_func);
}

#pragma GCC diagnostic pop
#endif
