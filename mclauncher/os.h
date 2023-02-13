#ifndef __OS_H
#define __OS_H 1

namespace utils 
{
    bool is_64_bit() {
#if defined(_WIN64)
        return true;
#elif defined(_WIN32)
        return false;
#else
        return sizeof(void*) == 8;
#endif
    }
}

#endif