#include <tuxnbits++/io/mouse.h>
#include <bitset>
#include <tuxnbits++/io/terminal.h>

namespace tux::io
{

/////////////////////////////////////////////////////////////////////////
/// \brief mouse::mev
///
mouse mouse::prev_mev{};

mouse &mouse::operator=(const mouse &cpy)
{
    pos = cpy.pos;
    dxy = cpy.dxy;
    button = cpy.button;
    state = cpy.state;
    return *this;
}




glyph::type direction_arrows[3][3]={
    {tux::glyph::arrow_up_left, tux::glyph::arrow_up,   tux::glyph::arrow_up_right},
    {tux::glyph::arrow_left,    tux::glyph::stop,       tux::glyph::arrowright},
    {tux::glyph::arrow_down_left, tux::glyph::arrow_down,   tux::glyph::arrow_down_right}
};

// ui::cxy indexes[3][3]={
//     {{-1,-1},{0,-1},{1,-1}},
//     {{-1, 0},{0, 0},{1, 0}},
//     {{-1, 1},{0, 1},{1, 1}}
// };


///////////////////////////////////////////////////////////////////
/// \brief mouse::parse
/// \param _fd
/// \return
/// \note Requires that the first byte in _fd must be 0x1b and consumed.
///
rem::cc mouse::parse(lfd &_fd)
{

    u8 b{0};
    int arg{0};
    std::vector<int> args{};
    //auto l = diagnostic::status(); l << "csi begin: " << color::yellow << std::format("0x{:02X}",*_fd) << color::z  << l;
    if(_fd >> b;b != 27){
        auto l = diagnostic::error(1); l << rem::cc::expected << color::r << " ESCape start sequence - got '" << color::hotpink4 << (int)b << color::r << " instead." << l;
        return rem::cc::rejected;
    }
    _fd >> b;
    //l << "csi seq #2 :['" << color::yellow << (int)b << color::z << "|" << color::hotpink4 << (char)b << color::r << "']" << l;
    if(b != '['){
        auto l = diagnostic::error(1); l << rem::cc::expected << color::r << " CSI sequence - got '" << color::hotpink4 << (int)b << color::r << " instead." << l;
        return rem::cc::rejected;
    }
    do{
        _fd >> b;
        //l << "next:['" << color::yellow << (int)b << color::z << "']" << l;
        if(b == '<'){
            //l << "Altered [ ignored as of now ]" << l;
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
            switch(b){
                case 'M' :
                    //auto l = diagnostic::debug(); l << "end of sequence :'" << color::yellow << (char)b << color::r << "' args = [" << color::hotpink4 << tux::string::bytes(args) << color::r << "]" << l;
                    return parse(false,args);
                case 'm':
                    return parse(true,args);
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
    return rem::cc::rejected;
}

mouse::operator bool()
{
    return true;
}


rem::cc mouse::parse(bool brel, std::vector<int> args_)
{


    // pressed 'flag' ignored. Relying on the XTerm Button and meta state byte which reports buttons on the lasts two bits:
    auto l = diagnostic::debug(1);
    l << "parsing sequence : " << color::lightskyblue4 << args_.size() << color::r << " arguments [" << color::yellow << tux::string::bytes(args_) << "]" << l;
    mouse mev{};
    static mouse prev_mev{};
    if (args_.size() != 3){
        //diagnostic::error() << " missing or extra arguments : expected 3, got " << color::yellow << args_.size() << //diagnostic::eol;
        return rem::cc::rejected;
    }

    //////////////////////////////////////////////////////////////////////
    /// Assign the current buttons state, Adjust the button event by the previous mouse data
    l = diagnostic::status(1); l << "button: " << (args_[0]&3) << l;
    mev.button.left   = (args_[0] & 3) == 0 ? 1 :0;
    mev.button.mid    = (args_[0] & 3) == 1 ? 1 :0;
    mev.button.right  = (args_[0] & 3) == 2 ? 1 :0;
    if((args_[0] & 3)==3){
        mev.pressed = false;
        mev.button = {0};
    }
    ///@todo Handle the other buttons...
    /// ...

    mev.state.shift     = (args_[0] & 4   ) != 0;
    mev.state.alt       = (args_[0] & 8   ) != 0;
    mev.state.win       = (args_[0] & 0x10) != 0;
    mev.pressed = !brel;
    // Subtract 1 from the coords. Because the terminal starts at 1,1 and our ui system starts 0,0
    mev.pos.x = args_[1] - 1; //l << " x coord: " << color::yellow << mev.pos.x << color::r << "|" << args_[1] << l;
    mev.pos.y = args_[2] - 1; //l << " y coord: " << color::yellow << mev.pos.y << color::r << "|" << args_[2] << l;
    mev.dxy = mev.pos - prev_mev.pos;
    prev_mev.pos = mev.pos;
    mev = mev & prev_mev;
    //auto l = diagnostic::status(); l << " local test: " << prev_mev() << l;
    terminal::push_event({.type = terminal::event::evt::MEV, .data={.mev=mev}});
    return rem::cc::ready;
}
//if(dxy.x < 0) dir << glyph::arrow_left;
// else if(dxy.x > 0) dir << glyph::data[glyph::arrowright];
// else if(dxy.y < 0) dir << glyph::data[glyph::arrow_up];
// else if(dxy.y > 0) dir << glyph::data[glyph::arrow_down];
// else
//     dir << glyph::data[glyph::big_dot];


/////////////////////////////////////////////////////////////////////
/// \brief mouse::get_direction_arrow
///    Just for fun when "stringify" the mouse states in a string: obtain the arrow glyph to illustrate the direction of the mouse move...lol
/// \param dxy
/// \return
///
std::string mouse::get_direction_arrow(ui::cxy dxy)
{
    tux::string arrow{};
    bool m[3][3]={
        {dxy.x < 0 && dxy.y < 0, dxy.x == 0 && dxy.y < 0, dxy.x > 0 && dxy.y < 0},
        {dxy.x < 0 && dxy.y == 0, dxy.x == 0 && dxy.y == 0, dxy.x > 0 && dxy.y == 0},
        {dxy.x < 0 && dxy.y > 0, dxy.x == 0 && dxy.y > 0, dxy.x > 0 && dxy.y > 0}
    };
    // There can be only one true value:
    for(int y = 0; y < 3; y++)
        for(int x=0; x < 3; x++)
            if(m[y][x]){
                arrow << direction_arrows[y][x];
                return arrow();
            }
    return " ";
}

//////////////////////////////////////////////////////////////////////
/// \brief mouse::operator () - put current state in a std::string.
/// \return std::string representing current mouse state values.
///
std::string mouse::operator()()
{
    tux::string text{};
    tux::string dir{};
    text << "["
         << color::orangered1 << std::format("{:>3d}", pos.x)
         << color::reset << ','
         << color::orangered1 << std::format("{:<3d}", pos.y)
         << color::reset << "]";
    if(button.left)
        text << color::lime << (pressed ? 'L' : 'l') << color::r;
    else
        text << "l";
    text << '|';
    if(button.mid)
        text << color::yellow << (pressed ? 'M' : 'm') << color::r;
    else
        text << "m";
    text << '|';
    if(button.right)
        text << color::hotpink4 << (pressed ? 'R' : 'r') << color::r;
    else
        text << "r";
    text << '|'
         << (dxy != ui::cxy{0,0}          ? color::yellow : color::reset) << get_direction_arrow(dxy)
         << color::reset << "["
         << color::orangered1 << std::format("{:>3d}",dxy.x)
         << color::reset << ','
         << color::orangered1 << std::format("{:<3d}",dxy.y) << color::reset << "]";
    // text << "["
    //      << color::orangered1 << std::format("{:>3d}", pos.x)
    //      << color::reset << ','
    //      << color::orangered1 << std::format("{:<3d}", pos.y)
    //      << color::reset << "]";

    // text << (pressed ? color::lime : color::r)
    //      << (button.left   ? 'L' : 'l')  << color::r << "|"
    //      << (button.mid    ? 'M' : 'm')  << color::r << "|"
    //      << (button.right  ? 'R' : 'r')  << color::r << "|"
    //      << (dxy != ui::cxy{0,0}          ? color::yellow : color::r) << get_direction_arrow(dxy)
    //      << color::reset << "["
    //      << color::orangered1 << std::format("{:>3d}",dxy.x)
    //      << color::reset << ','
    //      << color::orangered1 << std::format("{:<3d}",dxy.y) << color::reset << "]";
    return text();
}

////////////////////////////////////////////////////////////////////////
/// \brief mouse::operator &
/// \param mev
/// \return
/// \note disabled. Will be removed.
mouse& mouse::operator &(const mouse &mev)
{

    return *this;
}


} // namespace tux::io
