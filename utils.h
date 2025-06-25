#pragma once
#include <limits>
#include <iostream>

template<typename T>
bool bezpiecznyInput(T& zmienna) {
    std::cin >> zmienna;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}
