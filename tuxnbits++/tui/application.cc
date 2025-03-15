

#include <tuxvision/ui/application.h>
#include <tuxvision/ui/terminal.h>
#include <csignal>
#include <utility>


//#include <memory>


namespace tux::ui
{

application* application::_app_{nullptr};




application::application(std::string app_name, tux::string::view_list _args, tux::string::view_list _env): _app_name_(std::move(app_name)), _args_(std::move(_args)), _env_(std::move(_env))
{

    if(application::_app_ == nullptr)
    {
        application::_app_ = this;
    }
    else
        abort();
    setup();
}

rem::cc application::run()
{
    setup_ui();
    start_events_listening();
    terminate();
    return rem::cc::notimplemented;
}


application::~application()
{
    //...
    _args_.clear();
    //_events_queue_.clear();
    ///...
}


void application::sig_interrupted(int)
{
    log::interrupted() << log::eol;
    terminal::end();
    log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}



void application::sig_aborted(int)
{
    log::aborted() << log::eol;
    terminal::end();
    log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}

void application::sig_crash(int)
{
    log::segfault()  << log::eol;
    //terminal::end();
    //log::purge(nullptr);
    //application::app().terminate();
    exit(0);
}


void application::sig_winch(int)
{
    terminal::query_winch() ;
    //...
}



application& application::app()
{
    if(!application::_app_)
        throw log::exception() [log::except()  << rem::fn::endl << " no instance of application!" << log::endl];

    return *application::_app_;
}


terminal::screen* application::screen() { return application::_app_->_terminal_screen_; }


std::string application::app_name() const
{
    tux::string text{};
    auto [gh,colors] = rem::return_code_attributes(rem::cc::object_ptr);
    text
        | colors()
        | gh
        | class_name()
        | color::reset | "::";
    auto [gl, cc] = rem::return_code_attributes(rem::cc::object_id);
    text
        | cc()
        | gl
        | _app_name_
        | color::reset | ' ';

    return text();
}


// desktop *application::screen_desktop()
// {
//     return desktop::instance();
// }

rem::cc application::install_signals()
{

    std::signal(SIGSEGV, &application::sig_crash);
    log::jnl() << "signal SIGSEV installed."  << log::eol;
    std::signal(SIGABRT, &application::sig_aborted);
    log::jnl() << "signal SIGABRT installed."  << log::eol;
    std::signal(SIGINT, &application::sig_interrupted);
    log::jnl() << "signal SIGINT installed."  << log::eol;
    std::signal(SIGWINCH, &application::sig_winch);
    log::jnl() << "signal SIGWINCH installed."  << log::eol;
    // std::signal(SIGHUP, &application::sig_winch);
    std::signal(SIGKILL, &application::sig_interrupted);
    log::jnl() << "signal SIGKILL installed." << log::eol;
    std::signal(SIGTERM, &application::sig_interrupted);
    log::jnl() << "signal SIGTERM installed." << log::eol;

    return rem::cc::done;
}


/*!
 *
 * @return rem::cc result .
 */
rem::cc application::start_events_listening()
{
    auto& fd = _poll_.add_fd(STDIN_FILENO, io::iofd::IMM, POLLIN|POLLHUP|POLLERR, 1024);
    fd.init();
    log::debug() << "link descriptor handle [" << fd.fd() << "] to tddv::std_input() delegate/slot/handler..." << log::endl;
    (void)fd.set_read_delegate(&_ansi_, &terminal::ansi::parse_input);
    fd.resume();

    // Blocking in the calling thread (Formally the main thread).
    return _poll_.start_polling();
}




rem::cc application::setup()
{
    //...
    log::init();
    install_signals();
    //setup_ui();
    return rem::cc::done;
}


rem::cc application::setup_ui()
{
    terminal::begin();
    _terminal_screen_ = new terminal::screen("{terminal screen}");
    _terminal_screen_->setup();
    //...
    return rem::cc::done;
}


rem::cc application::terminate()
{
    log::debug() << " Restoring the terminal state, ending the UI, closing the journal logs file." << log::endl;
    terminal::end();
    delete application::_terminal_screen_;

    log::end();
    return rem::cc::reimplement;
}





std::string application::pretty_id() const
{
    tux::string text;

    auto [gh,colors] = rem::return_code_attributes(rem::cc::object_ptr);
    text
        | colors()
        | gh
        | class_name()
        | color::reset | "::";
    auto [gl, cc] = rem::return_code_attributes(rem::cc::object_id);
    text
        | cc()
        | gl
        | _app_name_
        | color::reset | ' ';

    return text();
}


event& application::new_event()
{
    application::_app_->_events.emplace_front();
    return application::_app_->_events.front();
}




rem::cc application::cancel_event()
{
    if (application::_app_->_events.empty()) return rem::cc::rejected;
    application::_app_->_events.pop_front();
    return rem::cc::done;
}


static event null_event{};

std::pair<rem::cc,event&> application::pending_event()
{
    if (application::_app_->_events.empty())  return { rem::cc::empty, null_event };
    auto& ev = application::_app_->_events.front();
    if (ev)  return { rem::cc::ready, null_event };
    return { rem::cc::notready, ev };
}

//
// rem::cc application::pop_event()
// {
//     if (application::_app_->_events.empty()) return rem::cc::rejected;
//     application::_app_->_events.pop_front();
//     return rem::cc::done;
// }


std::pair<rem::cc, event> application::get_event()
{
    if (application::_app_->_events.empty())
        return std::make_pair(rem::cc::empty, null_event);
    auto e = application::_app_->_events.front();
    application::_app_->_events.pop_front();
    return {rem::cc::ready, e};
}


bool application::has_events() { return !application::_app_->_events.empty(); }


bool application::last_event_available()
{
    ///@todo set event field to tell if the event is well set.
    return false;
}
}
