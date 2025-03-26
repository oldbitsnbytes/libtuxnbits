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

#include <tuxnbits++/bits/rem.h>
#include <tuxnbits++/bits/notify.h>
#include <tuxnbits++/bits/string.h>
#include <sqlite3.h>

namespace tux::sql
{



class LIBTUXNBITS databits  
{
    sqlite3 *db_file{nullptr};
    std::string db_file_name;

    struct istates
    {
        integers::u8 open : 1;
        integers::u8 err  : 1;
        integers::u8 wal  : 1; ///< Write-Ahead mode. (For our intensive tux::diagnostic logging journal for ex.)
    }state{0};

    // SQLITE_API int sqlite3_open(const char *filename, sqlite3 **ppDb);
    // SQLITE_API int sqlite3_close(sqlite3 *db);
    // SQLITE_API int sqlite3_exec(sqlite3 *db, const char *sql, int (*callback)(void*,int,char**,char**), void *arg, char **errmsg);

    tux::signals::notify<rem::cc, tux::string::view_list, tux::string::view_list> query_callback{"databits_system_callback_signal"};
    //                                  objptr  argc   rows   colnames
    static  int sqlite3_callback(void*,int,   char**,  char**);
public:


    databits() = default;
    explicit databits(const std::string& name);

    ~databits();

    rem::cc open();
    void close();

    [[nodiscard]] bool is_wal() const { return state.wal; }
    [[nodiscard]] bool is_open() const { return state.open; }
    [[nodiscard]] bool is_error() const { return state.err; }

    rem::cc set_wal(bool wal);


private:

};

}
