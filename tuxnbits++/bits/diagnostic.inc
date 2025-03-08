#pragma once

#include "diagnostic.h"

template<typename T> diagnostic::out& diagnostic::out::operator << (const T& v)
{
    std::ostringstream o;
    o << v;
    (*ofs) << o.str();
    return *this;
}

template<typename T> rem::cc diagnostic::Test::exec(const std::string& sub_test_name, std::function<T(diagnostic::Test&)> lambda )
{
    auto log = diagnostic::test(0) << name << "::" << sub_test_name << ":\n";
    log << "Calling the test:" << log;
    auto te = lambda(*this);
    log << te << log;
    if(!te.empty())
        return rem::cc::success;
    return rem::cc::failed;
}
