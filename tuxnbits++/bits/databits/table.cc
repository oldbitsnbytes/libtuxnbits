#include <tuxnbits++/bits/databits/table.h>


namespace tux::sql
{


table::~table()
{
    fields.clear();
}

field &table::operator <<(field &&f)
{
    std::string fname = f.name;
    fields[f.name] = std::move(f);
    return fields[fname];
}

}
