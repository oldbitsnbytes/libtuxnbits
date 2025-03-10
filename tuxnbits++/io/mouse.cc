#include <tuxnbits++/io/mouse.h>
#include <bitset>
#include <tuxnbits++/io/terminal.h>

namespace tux::io
{




////////////////////////////////////////////////////////////////////////
/// \brief mouse::operator &
/// \param mev
/// \return
/// \note This method is called once the latest mouse data is assigned to this instance so we can evaluate the mouse events according to the comparison against mev
mouse& mouse::operator &(const mouse &mev)
{
    if(std::bitset<2>(mev.button.left) != std::bitset<2>(button.left)){
        // event on left button:
        button.left = mev.button.left == 1 ? mouse::BUTTON_PRESSED : mouse::BUTTON_RELEASE;
    }
    if(std::bitset<2>(mev.button.right) != std::bitset<2>(button.right)){
        // event on right button:
        button.right = mev.button.right == 1 ? mouse::BUTTON_PRESSED : mouse::BUTTON_RELEASE;
    }
    if(std::bitset<2>(mev.button.mid) != std::bitset<2>(button.mid)){
        // event on middle button:
        button.mid = mev.button.mid == 1 ? mouse::BUTTON_PRESSED : mouse::BUTTON_RELEASE;
    }

    dxy = pos - mev.pos;

    // todo : Where do I put mouse move event ??
    return *this;
}


mouse mouse::mev{};
///////////////////////////////////////////////////////////////////
/// \brief mouse::test
/// \param _fd
/// \return
/// \note Requiers that the first byte in _fd must be 0x1b and consumed.
///
rem::cc mouse::test(lfd &_fd)
{
    u8 b{0};
    int arg{0};
    std::vector<int> args{};
    auto l = diagnostic::status(); l << "csi begin: " << color::yellow << std::format("0x{:02X}",*_fd) << color::z  << l;
    if(b = *_fd;b != 27)
    {
        l = diagnostic::error() << rem::cc::expected << color::r << " ESCape start sequence - got '" << color::hotpink4 << (int)b << color::r << " instead." << l;
        return rem::cc::rejected;
    }
    _fd >> b;
    if(b != '[')
    {
        l = diagnostic::error() << rem::cc::expected << color::r << " CSI sequence - got '" << color::hotpink4 << (int)b << color::r << " instead." << l;
        return rem::cc::rejected;
    }
    do{
        _fd >> b;
        //l << "next:['" << color::yellow << (char)*_fd << color::z << "']" << l;
        if(b == '<'){
            l << "Altered [ ignored as of now ]" << l;
            //...
            continue;
        }

        if(std::isdigit(b)){
            arg *=10; // Shift 10'th slide
            arg += b - '0';
            continue;
        }

        if(b ==';'){
            args.push_back(arg);
            arg = 0;
            continue;
        }

        // CSI is terminated by a character in the range 0x40–0x7E
        // (ASCII @A–Z[\]^_`a–z{|}~),
        // Note: I don't remember why we exclude '<'
        // To handle F1-F4, we exclude '['.
        if ((b >= '@') && (b <= '~') && (b != '<') && (b != '[')){
            args.push_back(arg);
            //diagnostic::status() << "end csi sequence on '" << color::yellow << (int)b << color::z << "' :\n";
            switch(b)
            {
                case 'M' : case 'm':
                   // l << "end of sequence :'" << color::yellow << (char)b << color::r << "' args = [" << color::hotpink4 << tux::string::bytes(args) << color::r << "]" << l;
                    return parse(args);
                case 'R':
                    //diagnostic::warning() <<" R :Caret report - ignored" << //diagnostic::eol;
                    break;
                default:
                    //diagnostic::error() << " Unhandled csi sequence. " << //diagnostic::eol;
                    break;
            }
            return rem::cc::rejected;
        }

    }while(!_fd.empty());
    _fd.clear();
    return rem::cc::ready;
}

mouse::operator bool()
{
    return true;
}

std::string mouse::operator()()
{
    tux::string text{};
    tux::string dir{};
    if(dxy.x < 0) dir << glyph::arrow_left;
    else if(dxy.x > 0) dir << glyph::data[glyph::arrowright];
    else if(dxy.y < 0) dir << glyph::data[glyph::arrow_up];
    else if(dxy.y > 0) dir << glyph::data[glyph::arrow_down];
    else
        dir << glyph::data[glyph::big_dot];

    text << "["
         << color::orangered1 << std::format("{:>3d}", pos.x)
         << color::reset << ','
         << color::orangered1 << std::format("{:<3d}", pos.y)
         << color::reset << "]"
         << (button.left   ? color::orangered1 : color::reset)  << (button.left == mouse::BUTTON_PRESSED    ?'L' : 'l')  << color::r << "|"
         << (button.mid ? color::lime : color::reset)           << (button.mid == mouse::BUTTON_PRESSED     ?'M' : 'm')  << color::r << "|"
         << (button.right  ? color::red4 : color::reset)        << (button.right == mouse::BUTTON_PRESSED   ?'R' : 'r')  << color::r << "|"
         << (dxy != ui::cxy{0,0}          ? color::yellow : color::reset) << dir()
         << color::reset << "["
         << color::orangered1 << std::format("{:>3d}",dxy.x)
         << color::reset << ','
         << color::orangered1 << std::format("{:<3d}",dxy.y) << color::reset << "]";
    return text();
}

rem::cc mouse::parse(std::vector<int> args_)
{
    // pressed 'flag' ignored. Relying on the XTerm Button and meta state byte which reports buttons on the lasts two bits:
    //auto l = diagnostic::debug();
    //l << "parsing seuqence : " << color::lightskyblue4 << args_.size() << color::r << " arguments [" << color::yellow << tux::string::bytes(args_) << "]" << l;
    mouse mev{};

    if (args_.size() != 3){
        //diagnostic::error() << " missing or extra arguments : expected 3, got " << color::yellow << args_.size() << //diagnostic::eol;
        return rem::cc::rejected;
    }

    //////////////////////////////////////////////////////////////////////
    /// Assign the current buttons state, Adjust the button event on the previous mouse data
    mev.button.left   = (args_[0] & 3) == 0 ? 1 :0;
    mev.button.mid    = (args_[0] & 3) == 1 ? 1 :0;
    mev.button.right  = (args_[0] & 3) == 2 ? 1 :0;
    ///@todo Handle the other buttons...
    /// ...

    mev.state.shift     = (args_[0] & 4   ) != 0;
    mev.state.alt       = (args_[0] & 8   ) != 0;
    mev.state.win       = (args_[0] & 0x10) != 0;

    //if(mev.state.alt)
     //   l << color::pair({.fg=color::grey100, .bg=color::red4}) << "meta" << l;

    // Subtract 1 from the coords. Because the terminal starts at 1,1 and our ui system starts 0,0
    mev.pos.x = args_[1] - 1; //l << " x coord: " << color::yellow << mev.pos.x << color::r << "|" << args_[1] << l;
    mev.pos.y = args_[2] - 1; //l << " y coord: " << color::yellow << mev.pos.y << color::r << "|" << args_[2] << l;
    ////diagnostic::info() << "mouse position:" << color::yellow << mev.pos << color::z << //diagnostic::eol;
    terminal::push_event({.type = terminal::event::evt::MEV, .data={.mev=mev}});
    return rem::cc::ready;
}



} // namespace tux::io
