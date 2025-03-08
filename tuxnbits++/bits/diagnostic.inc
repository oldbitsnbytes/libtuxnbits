#pragma once

#include "diagnostic.h"

template<typename T> diagnostic::out& diagnostic::out::operator << (const T& v)
{
    std::ostringstream o;
    o << v;
    (*ofs) << o.str();
    return *this;
}


