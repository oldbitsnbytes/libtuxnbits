#include <tuxnbits++/bits/databits/databits.h>
#include <tuxnbits++/bits/string.h>

#include <tuxnbits++/bits/diagnostic.h>

namespace tux::sql 
{
int databits::sqlite3_callback(void* _this, int argc , char** data_rows, char** columns_name)
{
    return SQLITE_OK;
}





databits::databits(const std::string& name)
{
    tux::string str = name;
    str <<".dbits";
    db_file_name = str();
}


databits::~databits()
{
    if (state.open)
        sqlite3_close(db_file);
    //...
}



rem::cc databits::open()
{
    auto ok = sqlite3_open(db_file_name.c_str(), &db_file);
    if (ok != SQLITE_OK)
    {
        tux::string msg = "Could not open database file " + db_file_name + ": " + sqlite3_errmsg(db_file);
        throw std::runtime_error(msg());

    }
    char* errMsg{nullptr};

    return rem::cc::notimplemented;
}


void databits::close()
{
    if (!state.open) return;
    sqlite3_close(db_file);
}


/*!
 * 
 * @param wal not used yet.
 * @return  accepted, rejected, or failed.
 */
rem::cc databits::set_wal(bool wal)
{
    if (state.open)
    {
        char* ErrMsg{nullptr};
        auto r = sqlite3_exec(db_file,"PRAGMA journal_mode=WAL;", &databits::sqlite3_callback, nullptr, &ErrMsg);
        if (r != SQLITE_OK)
        {
            //...
            auto l = diagnostic::error(0); l << ErrMsg << l;
            sqlite3_free(ErrMsg);
            return rem::cc::failed;
        }
        auto l = diagnostic::status(0); l << rem::cc::accepted << l;
        state.wal = 1;
        return rem::cc::accepted;
    }
    return rem::cc::rejected;
}


}
