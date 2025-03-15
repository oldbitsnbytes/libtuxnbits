//
// Created by oldlonecoder on 25-02-03.
//

#include <tuxnbits++/io/listener.h>
#include <cerrno>

namespace tux::io
{


static lfd _null{};

listener::listener(const std::string& obj_id){}


listener::~listener()
{
    //_fds.clear();
}




std::pair<rem::cc, lfd&> listener::attach(lfd&& fds)
{
    _fds.emplace_back(std::move(fds));
    epoll_event e{};
    auto& fd = _fds.back();
    fd.init();
    e.events = fd._poll_bits;
    e.data.fd = fd._fd;
    //diagnostic::info() << "attach new fd: " << color::yellow << fd._id << color::z << ":" <<//diagnostic::endl;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd._fd, &e) < 0)
    {
        //diagnostic::error() << "epoll_ctl() failed: " << std::strerror(errno) << //diagnostic::endl;
        fd._flags.active = 0;
    }
    return {rem::cc::accepted, _fds.back()};
}


// std::pair<rem::cc, lfd&> listener::add_fd(lfd&& fds)
// {
//     _fds.emplace_back(std::move(fds));
//     //diagnostic::info() << "added " << color::aqua << _fds.back()._id << color::z << //diagnostic::endl;
//     return {rem::cc::accepted, _fds.back()};
// }


rem::cc listener::detach(int fnum)
{
    auto fdi = std::find_if(_fds.begin(), _fds.end(), [fnum](const auto& fd)->bool
        {
        return fnum == fd._fd;
    });

    if (fdi == _fds.end()) return rem::cc::rejected;
    //diagnostic::info() << "detach and remove lfd '" << fdi->_id << color::z << "' from this listener." << //diagnostic::endl;
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fdi->_fd, nullptr);
    _fds.erase(fdi);

    return rem::cc::accepted;
}



rem::cc listener::terminate()
{
    _kill = true;
    return rem::cc::accepted;
}


rem::cc listener::run()
{

    do
    {
        if (_fds.empty())
        {
            //diagnostic::warning() << color::yellow << "cancelling this listener: fd list is empty()..." << //diagnostic::endl;
            if (_epoll_fd > 2 )
                close();
            return rem::cc::rejected;
        }

        auto nevents = epoll_wait(_epoll_fd, _poll_events, listener::max_events,-1);
        //diagnostic::info() << color::yellow << nevents << color::z << " events:" << //diagnostic::endl;
        refresh_fds();
        if ((nevents <= 0) && (errno != EINTR)){
            //diagnostic::error() << "epoll_wait() failed: (events count = " << color::yellow << nevents << color::z << "): " << color::deeppink8 <<  strerror(errno) << //diagnostic::endl;
            return rem::cc::failed;
        }
        for (int n = 0; n < nevents; n++){
            for (auto&fd : _fds){
                if (fd._fd ==_poll_events[n].data.fd){
                    if (_poll_events[n].events & EPOLLIN){
                        if (fd._flags.active){
                            auto a = fd._read();
                            ////diagnostic::debug() << color::aqua << fd._id << color::z << ": [" << static_cast<int>(a) << "] " << rem::to_string(a) << //diagnostic::endl;
                            if (a != rem::action::cont){
                                ////diagnostic::info() << "[" << a << "] active lfd to be killed."  << //diagnostic::endl;
                                fd.kill();
                            }
                        }
                        else
                            ;//diagnostic::debug() << " invoked lfd is NOT active." << //diagnostic::endl;
                    }
                    if ((_poll_events[n].events & EPOLLHUP) || (_poll_events[n].events & EPOLLERR)){
                        //diagnostic::info() << " broken link on '" << color::aqua << fd._id << color::z <<  //diagnostic::endl;
                        fd.kill();
                        continue;
                    }
                }
            }
        }
    }
    while (!_kill);

    close();
    return rem::cc::done;
}

rem::cc listener::poll(int _fd)
{
    auto [r, f] = query_lfd(_fd);
    if(!r){
        //diagnostic::error() << " file descriptor #" << color::red4 << _fd << color::z << " is not registered into this listener's group." << //diagnostic::eol;
        return r;
    }

    auto nevents = epoll_wait(_epoll_fd, _poll_events, listener::max_events,-1);
    //diagnostic::info() <<  color::yellow << nevents << color::z << " event(s):" << //diagnostic::endl;
    refresh_fds();

    if ((nevents <= 0) && (errno != EINTR)){
        //diagnostic::error() << "epoll_wait() failed: (events count = " << color::yellow << nevents << color::z << "): " << color::deeppink8 <<  strerror(errno) << //diagnostic::endl;
        return rem::cc::failed;
    }
    rem::action A{};
    if(f._fd == _poll_events[0].data.fd){
        if (_poll_events[0].events & EPOLLIN)
        {
            if (f._flags.active)
            {
                A = f._read(); // Actual read and process input data signal
                //diagnostic::debug() << color::aqua << f._id << color::z << ": [" << A << "] "  << //diagnostic::endl;
                if (A != rem::action::cont){
                    //diagnostic::info() << "[" << A << "] active lfd to be killed."  << //diagnostic::endl;
                    f.kill();
                    return rem::cc::terminate;
                }
            }
            else;
                //diagnostic::debug() << " invoked lfd is NOT active." << //diagnostic::endl;
        }
        else
        {
            if ((_poll_events[0].events & EPOLLHUP) || (_poll_events[0].events & EPOLLERR))
            {
                //diagnostic::info() << " broken link on '" << color::aqua << f._id << color::z <<  //diagnostic::endl;
                f.kill();
                return rem::cc::terminate;
            }
        }
        return rem::cc::ready;
    }

    return rem::cc::ready;
}




std::pair<rem::cc, lfd &> listener::query_lfd(int fnum)
{
    for(auto& fd : _fds)
        if(fd._fd == fnum)
            return {rem::cc::ready, fd};

    //auto i = std::find_if(_fds.begin(), _fds.end(), [&](const auto& c){ return fnum == c._fd; });
    //if(i != _fds.end()) return {rem::cc::ready,*i};

    // Yes returns valid lfd, but we can modify it as we wish - not a good idea. Only temporary solution.
    return {rem::cc::rejected, lfd::null_};
}



rem::cc listener::refresh_fds()
{
    for (auto& fd : _fds)
    {
        if (fd._flags.kill || fd._flags.del)
        {
            //diagnostic::info() << "removing lfd: '" << color::aqua << fd._id << color::z << //diagnostic::endl;
            if (fd._flags.kill)
                detach(fd._fd);
            else
                epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd._fd, nullptr);
        }
    }
    return rem::cc::done;
}


//////////////////////////////////////////////////////
/// \brief listener::operator []
/// \param fd_num
/// \return ref to the lfd instance at the given index num.
///
lfd &listener::operator[](int fd_num)
{
    return _fds[fd_num];
}


rem::cc listener::open()
{
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd < 0)
    {
        //diagnostic::error() << "epoll_create1() failed: " << strerror(errno) << //diagnostic::endl;
        return rem::cc::rejected;
    }

    //diagnostic::info() << "listener opened: file # " << color::yellow << _epoll_fd << color::z;
    //diagnostic::write() << "ready to run" << //diagnostic::endl;

    return rem::cc::ready;
}


rem::cc listener::close()
{
    _fds.clear();
    ::close(_epoll_fd);
    //diagnostic::info() << "listener is closed clean." << //diagnostic::endl;
    return rem::cc::ok;
}


} // namespace lux::io
