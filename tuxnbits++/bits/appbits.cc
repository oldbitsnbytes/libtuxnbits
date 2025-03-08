//
// Created by oldbitsnbytes on 25-03-08.
//

#include <tuxnbits++/bits/appbits.h>
#include <csignal>

namespace tux {

static appbits* _app_{nullptr};
void appbits::sig_crash(int)
{
    _app_->terminate(rem::type::segfault);
    exit(0);
}


void appbits::sig_aborted(int)
{
    _app_->terminate(rem::type::aborted);
    exit(0);
}


void appbits::sig_interrupted(int)
{
    _app_->terminate(rem::type::interrupted);
    exit(0);
}


appbits::appbits(const std::string& app_name, tux::string::view_list&& arguments, object* parent_obj):object(parent_obj, app_name),
                                                                                                      _args(std::move(arguments))
{_app_ = this;}


appbits::~appbits()
{
    diagnostic::status() << "destroy\n";
    on_exception.disconnect_all();
    on_terminate_request.disconnect_all();
    //...

    _args.clear();
}


rem::cc appbits::run()
{

    return rem::cc::implemented;
}


rem::cc appbits::setup()
{
    auto h = diagnostic::new_file("application");
    _dlog = *h;
    _diagnostic_handles["application"] = _dlog;
    auto log = diagnostic::status(_dlog);
    log << "Successfully opened the application diagnostic file" << log;
    return rem::cc::success;
}


rem::cc appbits::add_diagnostic(const std::string& diag_name)
{
    auto h = diagnostic::new_file(diag_name);
    if (!h)
    {
        //...
        auto log = diagnostic::error(_dlog);
        log << "failed to add diagnostic file identified by '" << color::hotpink4 <<  diag_name << color::r << "'.\n" << color::red4 << "rejected." << color::r;
        return rem::cc::rejected;
    }
    _diagnostic_handles[diag_name] = *h;

    return rem::cc::accepted;
}



rem::cc appbits::terminate(rem::type _reason)
{
    diagnostic::status() << "terminate: " << _reason << "\n";
    return rem::cc::implemented;
}


rem::cc appbits::system_signals()
{
    std::signal(SIGSEGV, &appbits::sig_crash      );
    std::signal(SIGABRT, &appbits::sig_aborted    );
    std::signal(SIGINT,  &appbits::sig_interrupted);
    return rem::cc::implemented;
}


rem::action appbits::commands()
{
    return rem::action::end;
}
} // tux
