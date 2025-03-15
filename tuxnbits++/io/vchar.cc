
#include <tuxnbits++/io/vchar.h>
#include <tuxnbits++/bits/diagnostic.h>
//#include <tuxnbits++/io/ui/console.h>


namespace tux::io
{

#define _eol_ color::pair(color::reset, color::reset)()









[[maybe_unused]] vchar::vchar(U32 Ch) : d(Ch){}

vchar::vchar(vchar* C): d(C->d){}

vchar::vchar(color::pair cc)
{
    set_colors(cc);
    d |= ASCII | 0x20&0xFF;
}


vchar &vchar::set_fg(color::code fg) { d = d & ~FGMask | static_cast<U8>(fg) << FGShift; return *this; }
vchar &vchar::set_bg(color::code bg) { d = d & ~BGMask | static_cast<U8>(bg) << BGShift; return *this; }
[[maybe_unused]] vchar &vchar::set_attributes(U32 ch) { d = (d & ~AttrMask) | ch; return *this; }
//[[maybe_unused]] vchar &vchar::set_colors(color::pair &&Ch) { return set_fg(Ch.fg).set_bg(Ch.bg); }

[[maybe_unused]] vchar &vchar::set_colors(const color::pair &CP)
{
    d = d & ~ColorsMask | static_cast<U8>(CP.fg) << FGShift | static_cast<U8>(CP.bg) << BGShift;
    return *this;
}

vchar &vchar::operator=(U32 Ch) { d = Ch; return *this; }

vchar &vchar::operator=(const U32* Ch) { d = *Ch; return *this; }


std::string vchar::get_utf_string() const
{
    if(d & UTFBITS)
    {
        switch(d & UTFBITS)
        {
        case UGlyph:
            return  glyph::data[icon_id()];
        case Accent:
            return  accent_fr::data[accent_id()];
        case Frame:
            return cadre()[frame_id()];
        default: break;
            //throw //diagnostic::exception() [//diagnostic::fatal() << " Memory corruption error into vchar data!"];
        }
    }
    std::string s;
    s += ascii();
    return s;
}


vchar &vchar::operator=(char Ch)
{
    d = d & ~(CharMask|UGlyph|Accent|Frame) | d & (CMask|Underline|Stroke|Blink|TRGB)  | ASCII | Ch & 0xff;
    return *this;
}

color::code vchar::foreground() const { return static_cast<color::code>((d & FGMask) >> FGShift); }
color::code vchar::background() const { return static_cast<color::code>((d & BGMask) >> BGShift); }
[[maybe_unused]] color::pair vchar::colors() const { return {foreground(),background()}; }

[[maybe_unused]] glyph::type vchar::icon_id() const
{
    if (!(d & UGlyph))
        throw diagnostic::exception() [ diagnostic::except() << rem::cc::rejected << "attempt to use this vchar cell as a glyph which is not,"];

    auto Ic = d & CharMask;
    if (Ic > glyph::book)
        throw diagnostic::exception()[diagnostic::except() <<  rem::cc::oob << ':' << Ic];
    return  Ic;
}


[[maybe_unused]] accent_fr::type vchar::accent_id() const
{
    auto AID = d & CharMask;
    if (AID > accent_fr::Ucirc)
        throw diagnostic::exception()[diagnostic::fatal()  << rem::cc::oob << ':' << AID];

    return static_cast<accent_fr::type>(AID);
}




cadre::index vchar::frame_id() const
{
    auto c = d & 0xff;
    if(c > 11)
        throw diagnostic::exception() [diagnostic::except() << rem::cc::oob  << " invalid frame index: " << color::red4 << c];

    return static_cast<cadre::index>(d & 0xFF);
}

char vchar::ascii() const
{ return static_cast<char>(d & 0xff); }

[[maybe_unused]] uint16_t vchar::attributes() const
{ return (d & AttrMask) >> ATShift; }

vchar &vchar::operator<<(glyph::type gl)
{ d = (d & ~(UtfMask|CharMask))  | (d&ColorsMask)|UGlyph | (gl&0xFF); return *this; }

vchar &vchar::operator<<(accent_fr::type ac)
{ d = (d & ~(AttrMask|CharMask)) | (d & (Underline|Stroke|Blink|ColorsMask)) | Accent | ac; return *this; }

vchar& vchar::operator<<(cadre::index fr)
{ d = (d & ~(UtfMask|CharMask))  | (d&ColorsMask)|(fr&0xFF) | Frame; return *this; }

vchar& vchar::operator<<(color::pair cp)
{ d = (d & ~ColorsMask)          | static_cast<U8>(cp.fg) << FGShift | static_cast<U8>(cp.bg) << BGShift; return *this; }

vchar& vchar::operator<<(char Ch)
{ d = (d & ~(UTFBITS|CharMask))  | (d & (Underline|Stroke|Blink|ColorsMask)) | ASCII | (Ch & 0xff); return *this; }


[[maybe_unused]] std::string vchar::render_colors() const
{
    std::string str;
    str += color::render({background(),foreground()});
    return str;
}

vchar::operator std::string() const { return details(); }



// std::string vchar::render_line(const vchar::string& _string)
// {
//     return "vchar::render_line(const vchar::string& _string) is not implemented";
// }



//////////////////////////////////////////////////////////////////////////////
/// \brief vchar::render_line
/// \param _it
/// \param count
/// \return string bloc of the ansi codes and text
///
std::string vchar::render_line(vchar::string::iterator _it, std::size_t count)
{
    color::pair current_colors = _it->colors();
    ////diagnostic::debug() << "iterator details:" << _it->details() << //diagnostic::eol;
    std::string _o = current_colors();
    auto c = _it;
    for(int x =0; x< count; x++)
    {
        vchar ch = *c++;
        auto  [fg, bg] = ch.colors();
        if(current_colors.bg != bg)
        {
            current_colors.bg = bg;
            _o += color::render_background(current_colors.bg);
        }
        if(current_colors.fg != fg)
        {
            current_colors.fg = fg;
            _o += color::render(current_colors.fg);
        }
        if(ch.d & UTFBITS)
        {
            if(ch.d & Frame)
                _o += cadre()[ch.frame_id()];
            else
                if(ch.d & Accent)
                    _o += accent_fr::data[ch.accent_id()];
                else
                    if(ch.d & UGlyph)
                    {
                        _o += glyph::data[ch.icon_id()];
                        _o += '\x08';
                    }
        }
        else
            _o += ch.ascii();
    }
    _o += _eol_;
    return _o;
}





std::string vchar::details() const
{

    tux::string infos;
    tux::string utf_info{};
    if(d & UTFBITS)
    {
        switch(d & UTFBITS)
        {
        case UGlyph:
            utf_info << "icon index:" << icon_id();
            break;
        case Accent:
            utf_info << "accent (fr) index:" << accent_id();
            break;
        case Frame:
            utf_info << "frame index:" << frame_id();
            break;
        default:break;
            //throw rem::exception() [rem::fatal() << " Memory corruption into screen::vchar data!"];
        }
    }
    else
        utf_info << ascii();
    infos << "| foreground color:" << foreground() << color::name(foreground()) << color::reset << "| background color:" << background() << color::name(background()) << color::reset;
    infos << " char:['" << colors() << utf_info() << color::reset <<  "']";
    if(d & Underline) infos << "|Underline";
    if(d & Stroke) infos << "|Stroke";
    if(d & Blink) infos << "|Blink";

    return infos();
}




} // namespace tux::io
