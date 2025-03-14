#include <tuxnbits++/io/terminal.h>
#include <csignal>
namespace tux::io
{

terminal* terminal::_terminal{nullptr};

signals::notify_action<rectangle>& terminal::term_resize_signal()
{
    return _window_resize_signal;
}



terminal::termslist terminal::terms;



rem::cc terminal::enque(event &&ev)
{
    _events.push_back(std::move(ev));
    return rem::cc::accepted;
}

//////////////////////////////////////////////////////////////////////////////////////
/// \brief terminal::push_event
/// \param ev
/// \todo Guard and unlock _events queue with a mutex and signal/notify threads pool with condition variable.
///
void terminal::push_event(event&& ev) { _terminal->_events.push_back(std::move(ev)); }


rem::cc terminal::query_winch()
{
    winsize win{};
    ioctl(fileno(stdout), TIOCGWINSZ, &win);
    if((!win.ws_col)||(!win.ws_row))
        return rem::cc::notexist;

    _geometry = rectangle{{0,0}, ui::size{static_cast<int>(win.ws_col), static_cast<int>(win.ws_row)}};
    //diagnostic::message() << " (new) terminal size: ["
                    //<< color::yellow << std::format("{:>3d}x{:<3d}",_geometry.dwh.w,_geometry.dwh.h)
                    //<< color::reset << "]" << //diagnostic::endl;

    return rem::cc::done;
}


rectangle terminal::geometry() { return _geometry; }



// /*!
// * \brief
//     Instant call to this function when the system's terminal resize event occured.
//     We take its new geometry and then notify connected hooks
// * \author  Serge Lussier ( serge.lussier@oldbitsnbytes.club )
// */

// void terminal::resize_signal(int )
// {
//     if (!_terminal)
//         throw //diagnostic::exception() [//diagnostic::error() << "terminal::resize_signal: no terminal instance was set." << rem::action::commit];

//     terminal::query_winch();
//     _window_resize_signal(_geometry);
// }

terminal::terminal(terminal *parent_term, const std::string &_name_id, rectangle dim) : object(parent_term,_name_id),
    _geometry(dim)
{
    if(_terminal) return ;
    _terminal = this;
}

/*!
    @brief (static) terminal::begin()
        tuxvision terminal setup

    @author  Serge Lussier ( serge.lussier@oldbitsnbytes.club )
*/
rem::cc terminal::begin()
{
    tcgetattr(STDIN_FILENO, &_saved);
    _this = _saved;

    //new_term.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | IGNCR | IXON );
    _this.c_iflag &= ~(BRKINT | PARMRK | ISTRIP | IGNCR | IXON );
    _this.c_oflag &= ~OPOST;
    _this.c_cflag |= (CS8);
    _this.c_cflag &= ~PARENB;
    _this.c_lflag &= ~(ICANON | ECHO | IEXTEN | TOSTOP | ECHONL);
    _this.c_cc[VMIN] = 0;
    _this.c_cc[VTIME] = 1;
    _this.c_cc[VSTART] = 0;
    _this.c_cc[VSTOP] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_this);

    //diagnostic::write() << " terminal set to raw mode..." << //diagnostic::endl;

    //::signal(SIGWINCH, &terminal::resize_signal); // --> Must be Handled in the thread/proc/ env
    //if(_flags & terminal::use_double_buffer)
        switch_alternate();
    //cursor_off();
    terminal::enable_mouse();
    terminal::init_stdinput();

    return rem::cc::done;
}

rem::cc terminal::end()
{
    //if(_flags & terminal::use_double_buffer)
        switch_back();
    //diagnostic::info() << "closing terminal state:" << //diagnostic::eol;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_saved);
    //diagnostic::write() << "cursor on, disabling mouse tracking mode." << //diagnostic::eol;
    cursor_on();
    stop_mouse();
    //diagnostic::write() << "closing the terminal polling file." << //diagnostic::eol;
    close(_epoll_fd);
    _fd0.clear();
    return rem::cc::done;
}

void terminal::switch_alternate()
{
    std::cout << "\x1b[0m\x1b[?1049h" << std::flush;
    _flags |= terminal::st_saved;
}

void terminal::switch_back()
{
    std::cout << "\x1b[0m\x1b[?1049l" << std::flush;
    _flags &= ~terminal::st_saved;
}


/**
 * @brief Set terminal to report the mouse events.

    @note Execute CSI on each parameters because I realized that "\x1b[?1000;1002;...h" is not working.
 *
 */
rem::cc terminal::enable_mouse()
{
    //std::cout << MOUSE_VT200            << terminal::SET;
    //std::cout << MOUSE_REPORT_BUTTONS   << SET;
    std::cout << MOUSE_REPORT_ANY       << SET;
    std::cout << MOUSE_SGR_EXT_MODE     << SET<< std::flush;
    //std::cout << MOUSE_URXVT_MODE       << SET << std::flush;
    _flags |= terminal::use_mouse;

    return rem::cc::accepted;
}

rem::cc terminal::stop_mouse()
{
    //std::cout << MOUSE_VT200            << RESET;
    //std::cout << MOUSE_REPORT_BUTTONS   << RESET;
    std::cout << MOUSE_REPORT_ANY       << RESET;
    std::cout << MOUSE_SGR_EXT_MODE     << RESET << std::flush;
    //std::cout << MOUSE_URXVT_MODE       << RESET << std::flush;

    _flags &= ~terminal::use_mouse;

    return rem::cc::accepted;
}


void terminal::clear()
{
    std::cout << "\x1b[2J";
}


void terminal::cursor_off()
{
    std::cout << "\x1b[?25l" << std::flush;
    _flags &= ~terminal::st_caret_on;
}


/*!
 * @brief (Re-)activate the screen cursor.
 *
 */
void terminal::cursor_on()
{
    std::cout << "\x1b[?25h" << std::flush;
    _flags |= terminal::st_caret_on;
}




bool terminal::cursor(ui::cxy xy)
{
    if(_geometry.goto_xy(xy))
        std::cout << std::format("\x1b[{:d};{:d}H", xy.y+1,xy.x+1);
    return true;
}

void mv(ui::direction::type dir, int d)
{
    char ch = dir == ui::direction::right ? 'C': dir==ui::direction::left  ? 'D' : dir==ui::direction::up    ? 'A' : 'B';
    std::cout << "\x1b[" << d << ch;
}






terminal::~terminal()
{
    std::cout << __PRETTY_FUNCTION__ << ": instance destroyed.\n";
}


rem::cc terminal::render(vchar::bloc* blk, ui::cxy xy)
{

    for (int y=0; y < blk->dim.height<int>(); y++){
        terminal::cursor({xy.x, xy.y+1+y});
        std::cout << vchar::render_line(blk->buffer->begin() + (*blk->dim.width<>() * y), *blk->dim.width<>());
    }
    std::cout << std::flush;
    return rem::cc::done;

}


static io::mouse mev{};

/*!
 * @brief Blocking terminal stdin input listener & loop.
 * @return rem::cc
 */
rem::cc terminal::init_stdinput()
{

    _epoll_fd = epoll_create1(0);
    if (_epoll_fd < 0)
    {
        //diagnostic::error() << "epoll_create1() failed: " << strerror(errno) << //diagnostic::endl;
        return rem::cc::rejected;
    }

    //diagnostic::info() << "terminal epoll file # " << color::yellow << _epoll_fd << color::z << //diagnostic::eol;
    //diagnostic::info() << "Setting STDIN_FILENO to the epoll set" << //diagnostic::endl;
    epoll_event e{};
    _fd0._id = "fd #" + std::to_string(0);
    _fd0._poll_bits = EPOLLIN|EPOLLERR|EPOLLHUP;
    _fd0._bits = lfd::IMM | lfd::READ;
    _fd0._window_block_size = 4096;
    _fd0._fd = STDIN_FILENO;
    _fd0.init();
    e.events = EPOLLIN|EPOLLERR|EPOLLHUP;
    e.data.fd = STDIN_FILENO;
    //diagnostic::info() << "init terminal::_fd0:" << color::yellow << _fd0._id << color::z << ":" <<//diagnostic::endl;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &e) < 0)
    {
        //diagnostic::error() << "epoll_ctl() failed: " << std::strerror(errno) << //diagnostic::endl;
        _fd0._flags.active = 0;
    }
    //diagnostic::write() << "terminal input polls ready to run" << //diagnostic::endl;
    return rem::cc::ready;
}


rem::cc terminal::poll_in()
{
    int nev = epoll_wait(_epoll_fd, _poll_events, 1,-1);
    ////diagnostic::debug() << " number of event(s) = " << color::yellow << nev << color::z << //diagnostic::eol;
    if (nev <= 0)// && (errno != EINTR))
    {
        ////diagnostic::error() << "epoll_wait() failed: (events count = " << color::yellow << nev << color::z << "): " << color::deeppink8 <<  strerror(errno) << //diagnostic::endl;
        return rem::cc::rejected;
    }
    for(int n = 0; n < nev; n++){

        if(_poll_events[n].data.fd == _fd0._fd)
        {

            if (_poll_events[n].events & EPOLLIN)
            {
                if(_fd0._read() != rem::action::cont) return rem::cc::rejected;
                return terminal::stdin_proc();
            }
            if (_poll_events[n].events & EPOLLERR)
            {
                //diagnostic::error() << "epoll_wait() failed: (events count = " << color::yellow << nev << color::z << "): " << color::deeppink8 <<  strerror(errno) << //diagnostic::endl;
                return rem::cc::failed;
            }
            if (_poll_events[n].events & EPOLLERR)
            {
                //diagnostic::error() << "epoll_wait() hangup event" << //diagnostic::endl;
                return rem::cc::failed;
            }
        }
    }

    throw diagnostic::exception()[ diagnostic::except() << "epoll_wait seems unable to address the registered STDIN_FILENO. "];
}
//////////////////////////////////////////////////////////////////////
/// \brief terminal::stdin_proc
/// \return rejected or ready;
///
rem::cc terminal::stdin_proc()
{

    while(!_fd0.empty())
    {
        if(auto rcc = kbhit::test(_fd0); !!rcc){
            if(_events.back().data.kev.mnemonic == kbhit::ESCAPE){
                return rem::cc::ready;
            }
            continue;
        }
        if(auto rcc = mouse::test(_fd0); !!rcc){
            auto& e = _events.back();
            if(!e.is<mouse>()){
                _fd0.clear(); // discard and dismiss the rest of the input data because it is not parsable.
                return rem::cc::rejected;
            }

            mouse::prev_mev = e.data.mev;
            auto l = diagnostic::status();l << " mouse event: " << e.data.mev() << l;
        }
    }

    return rem::cc::ready;
}


} // namespace tux::io
