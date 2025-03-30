//
// Created by oldlonecoder on 25-03-26.
//

#include "field.h"

namespace tux::sql
{


///////////////////////////////////////////////////////////
/// \brief field::to_string
///     Builds the text to generate this field's schema given the data type and properties.
/// \return
///
field::field(const std::string &field_name): name(field_name){}

field::field(const std::string &field_name, sqlite3_types data_type, u16 props, foreign foreign_prop):
    name(field_name),type(data_type), index_bits(props), fkey(foreign_prop){}

std::string field::to_string()
{

    std::map<field::sqlite3_types, std::string> _={
        {field::sqlite3_types::blob_type,"BLOB"},
        {field::sqlite3_types::integer_type, "INTEGER"},
        {field::sqlite3_types::real_type,"REAL"},
        {field::sqlite3_types::text_type,"TEXT"},
        {field::sqlite3_types::null_type,"NULL"}
    };

    tux::string str;
    str << name << ' ' << _[type];

    if(index_bits & field::PKEY && type == integer_type)
    {
        str << " PRIMARY KEY";
        if(index_bits & field::AUTOINC)
        str << " AUTOINCREMENT";
    }

    //...
    return str();
}

} // namespace tux::sql;
