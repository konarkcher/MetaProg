#pragma once

#include <iostream>

void PrintArgs() {
}

template<class Arg1>
void PrintArgs(Arg1 arg1) {
    std::cout << arg1;
}

template<class Arg1, class... Args>
void PrintArgs(Arg1 arg1, Args... args) {
    std::cout << arg1 << ", ";
    PrintArgs(args...);
}
