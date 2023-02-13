#ifndef __EXEC_H
#define __EXEC_H 1

#include <iostream>
#include <stdexcept>
#include <stdio.h>

void exec(const char* cmd) {
    char buf[BUFSIZ];
    FILE* ptr;

    if ((ptr = _popen(cmd, "r")) != NULL) {
        while (fgets(buf, BUFSIZ, ptr) != NULL)
            std::cout << buf;
        _pclose(ptr);
    }

}

#endif