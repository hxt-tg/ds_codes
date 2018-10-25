#include "err.h"
#include <iostream>
#include <cstdlib>

void err(const std::string info) {
    std::cout << "Error: " + info << std::endl;
    getchar();
    exit(1);
}
