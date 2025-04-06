
/******************************************************************************************
 *   Copyright (C) ...,2024,... by Serge Lussier                                          *
 *   serge.lussier@oldlonecoder.club                                                      *
 *   ----------------------------------------------------------------------------------   *
 *   Unless otherwise specified, all Code IsIn this project is written                    *
 *   by the author (Serge Lussier).                                                       *
 *   ----------------------------------------------------------------------------------   *
 *   Copyrights from authors other than Serge Lussier also apply here.                    *
 *   Open source FREE licences also apply To the Code from the author (Serge Lussier)     *
 *   ----------------------------------------------------------------------------------   *
 *   Usual GNU FREE GPL-1,2, MIT... apply to this project.                                *
 ******************************************************************************************/


#pragma once


#include <tuxnbits++/bits/appbits.h>
#include <csignal>
//#include <memory>
#include <tuxnbits++/bits/cadres.h>
#include <tuxnbits++/io/terminal.h>
#include <tuxnbits++/tui/export.h>
namespace tux::tui
{

class LIBTUXNBITSTUI application
{

    CLASSNAME_START(application)

    static void sig_interrupted(int);
    static void sig_aborted(int);
    static void sig_crash(int);
    static void sig_winch(int);

    tux::string::view_list _args_{};
    tux::string::view_list _env_{};
    std::string _app_name_{};
    static application* _app_;


    event::list _events{};

    io::ipoll _poll_{"tuxvision application io polling"};

    terminal::screen* _terminal_screen_{nullptr};
    terminal::ansi    _ansi_{};
public:
    application() = delete;
    virtual ~application();
    //application(std::string app_name, int argc, char **argv);
    application(std::string app_name, tux::string::view_list _args, tux::string::view_list _env={});

    virtual rem::cc run(); //{ return rem::cc::notimplemented;}
    virtual rem::cc terminate();

    static application& app();
    static terminal::screen* screen();
//    static desktop* screen_desktop();
    [[nodiscard]] std::string app_name() const;
    std::string pretty_id() const;
    static event& new_event();
    static std::pair<rem::cc,event&> pending_event();
    static rem::cc cancel_event();
    static rem::cc wait_event() { return rem::cc::reimplement;}; ///< sous r&eacute;s&egrave;rve
    //static rem::cc pop_event();
    static std::pair<rem::cc, event> get_event();
    static bool has_events();
    static bool last_event_available();
protected:
    rem::cc setup();
    virtual rem::cc setup_ui();
    virtual rem::cc install_signals();
    virtual rem::cc start_events_listening();
};



}
