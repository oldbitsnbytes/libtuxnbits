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
/*
(en/us): Experimenting a diagnostic::Test approach
(fr/ca): Experimente une forme de test sous diagnostic::Test
--------------------------------------------------------------
    modifié : CMakeLists.txt
    modifié : tuxnbits++/bits/diagnostic.cc
    modifié : tuxnbits++/bits/diagnostic.h
    modifié : tuxnbits++/bits/diagnostic.inc
    modifié : tuxnbits++/bits/expect.h
    nouveau fichiers : tuxnbits++/bits/expect.inc, 	bitstest.cmake

 */
