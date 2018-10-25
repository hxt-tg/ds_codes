#include "err.h"
#include <iostream>

void err(const std::string info) {
    std::cout << "Error: " + info << std::endl;
    getchar();
    exit(1);
}