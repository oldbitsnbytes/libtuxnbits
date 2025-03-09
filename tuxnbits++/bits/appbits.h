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
#include <tuxnbits++/bits/object.h>
#include <tuxnbits++/bits/cmdargs.h>

namespace tux
{

class LIBTUXNBITS appbits : public object
{
protected:
    tux::string::view_list _args{};
    cmd::line              _proc{};
    signals::notify_action<diagnostic::exception&, appbits&> on_exception{"exception signal"};
    signals::notify_action<diagnostic::exception&, appbits&> on_terminate_request{"terminate request signal"};

    diagnostic::file::handle _dlog{0};
    static void sig_crash(int);
    static void sig_aborted(int);
    static void sig_interrupted(int);

    std::map<const std::string, diagnostic::file::handle> _diagnostic_handles{};
public:
    appbits(const std::string& app_name, tux::string::view_list&& arguments, object* parent_obj=nullptr);
    appbits()=default;
    ~appbits() override;

    virtual rem::cc run() = 0; ///< appbits is an abstract base app class.
    virtual rem::cc setup();
    rem::cc add_diagnostic(const std::string& diag_name);
    virtual rem::cc terminate(rem::type reason);
    virtual rem::cc system_signals();
    virtual rem::action commands();
    cmd::line& command_line() { return _proc; }

protected:
    static appbits& app();

};

} // tux
