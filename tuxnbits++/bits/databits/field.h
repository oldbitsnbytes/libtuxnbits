/******************************************************************************************
*   Copyright (C) ...,2025,... by Serge Lussier                                          *
*   serge.lussier@oldbitsnbytes.club / lussier.serge@gmail.com                            *
*   ----------------------------------------------------------------------------------   *
*   Unless otherwise specified, all Codes ands files in this project is written          *
*   by the author and owned by the auther (Serge Lussier), unless otherwise specified.   *
*   ----------------------------------------------------------------------------------   *
*   Copyrights from authors other than Serge Lussier also apply here.                    *
*   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
*   ----------------------------------------------------------------------------------   *
*   Usual GNU FREE GPL-1,2, MIT... or whatever -  apply to this project.                 *
*   ----------------------------------------------------------------------------------   *
*   NOTE : All code source that I am the only author, I reserve for myself, the rights to*
*   make it to restricted private license.                                               *
******************************************************************************************/



#pragma once

#include <tuxnbits++/bits/diagnostic.h>


namespace tux::sql
{


//////////////////////////////////////////////////////////////
/// \brief The field class ( reflect column schema ).
/// \author Serge Lussier ( oldbitsnbytes.club )
///
/// --------- From SQLite3 docs (https://www.sqlite.org/datatype3.html)
/// \li NULL.       The value is a NULL value.
/// \li INTEGER.    The value is a signed integer, stored in 0, 1, 2, 3, 4, 6, or 8 bytes depending on the magnitude of the value.
/// \li REAL.       The value is a floating point value, stored as an 8-byte IEEE floating point number.
/// \li TEXT.       The value is a text string, stored using the database encoding (UTF-8, UTF-16BE or UTF-16LE).
/// \li BLOB.       The value is a blob of data, stored exactly as it was input.
/// ...
/// A column that uses INTEGER affinity behaves the same as a column with NUMERIC affinity. The difference between INTEGER and NUMERIC
/// affinity is only evident in a CAST expression: The expression "CAST(4.0 AS INT)" returns an integer 4, whereas "CAST(4.0 AS NUMERIC)"
/// leaves the value as a floating-point 4.0.
/// ...
/// -----------------------------------------------------------------------
///
struct LIBTUXNBITS field
{
    enum sqlite3_types
    {
        null_type,
        integer_type,
        real_type,
        text_type,
        blob_type
    }type{field::integer_type};

    struct properties
    {
        u8 primary_key :1;
        u8 autoinc     :1;
        u8 unique      :1;
        u8 index       :1;
    }bits{0,0,0,0};



    std::string name;
    tux::string::view_list rows{};
    field()=default;
    field(const std::string& name);
    field(const field&) = default;
    field(field&&) = default;

    ~field() = default;

    field& operator=(const field&) = default;
    field& operator=(field&&) = default;
    std::string to_string();
};

} // tux::sql
