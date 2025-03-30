#pragma once

#include <tuxnbits++/bits/diagnostic.h>
#include <tuxnbits++/bits/databits/field.h>


namespace tux::sql
{


struct LIBTUXNBITS table
{

    field::list fields;
    std::string name;

    table() = default;
    table(const std::string& table_name);
    ~table();


    field& operator << (field&& f);


};


}


