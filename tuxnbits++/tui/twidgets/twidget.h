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

#include <tuxnbits++/tui/pencil.h>
#include  <tuxnbits++/tui/tuins.h>




namespace tux::ui
{

class LIBTUXNBITSTUI twidget: public object
{
    io::vchar::bloc* _bf{nullptr};
    std::string _theme_name{"default"};
    std::string _element{}; ///< It is important to assign the proper component name to this element...
    colors::attr_db::elements* _theme{nullptr};


public:
    twidget(twidget* _parent_widget, const std::string& twid);
    ~twidget() override;

    pencil& begin_paint();
    rem::cc end_paint(pencil&);
};

} // namespace tux::ui
