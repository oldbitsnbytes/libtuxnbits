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

#include <tuxnbits++/bits/object.h>
#include <tuxnbits++/bits/geometry.h>
#include <tuxnbits++/io/vchar.h>
#include <tuxnbits++/tui/export.h>
namespace tux::ui
{

class LIBTUXNBITSTUI pencil : public object
{
    io::vchar::bloc* _bf{nullptr}; ///< drawing canva.
    rectangle        _rect{};      ///< For now it is the bloc's subregion ( by default it is the same dimensions, local/relative to the bloc's geometry )..
    color::pair      _colors{};    ///< Stored current bloc buffers colors. The bloc buffer colors are restored with this _colors when this painter/pencil terminates.
    bool             _valid{false};
public:
    pencil()=default;
    pencil(io::vchar::bloc* bf, rectangle subregion_);
    ~pencil() override;

    rem::cc draw_text(const std::string& txt);
    rem::cc draw_glyph(glyph::type ic);
    rem::cc draw_cadre(cadre::frame_matrix frame_model);
    rem::cc gotoxy(const ui::cxy& xy);

    void set_foreground_color(color::code fg);
    void set_background_color(color::code bg);
    void set_colors(color::pair cp);

    pencil& clear(const ui::rectangle& r={}, color::pair cp = {color::reset,color::reset});
    pencil& home(bool);
    pencil& operator ++();      // ++x
    pencil& operator ++(int);   // ++y
    pencil& operator --();      // --x
    pencil& operator --(int);   // --y

    pencil& draw_frame(cadre::frame_matrix frmat={2,2,2,1});




};


} // namespace tux::ui
