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

namespace tux::ui
{

class pencil : public object
{
    io::vchar::bloc*        _bf{nullptr}; ///< drawing canva.
    rectangle               _rect{};      ///< For now it is the bloc's subregion ( by default it is the same dimensions, local/relative to the bloc's geometry )..
    color::pair             _colors{};    ///< Current colors. ( fallback/reset colors are stored in _bf ).

public:
    pencil();
    pencil(io::vchar::back_buffer bf, rectangle subregion_);
    ~pencil();


};


} // namespace tux::ui


