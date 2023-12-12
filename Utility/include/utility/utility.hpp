
#pragma once

#include <string>
#include <stdexcept>

template <typename T>
T *nullThrow(T *ptr, std::string const &msg) {
    if (ptr==nullptr) throw std::runtime_error(msg);
    return ptr;
}

void falseThrow(bool b, std::string const &msg) {
    if (!b) throw std::runtime_error(msg);
}
