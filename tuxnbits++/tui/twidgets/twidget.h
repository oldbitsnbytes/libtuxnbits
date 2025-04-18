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

    rectangle upd_rect{}; ///< Union of agregation of dirty local subrects;

public:
    twidget(twidget* _parent_widget, const std::string& twid);
    ~twidget() override;

    pencil&  begin_paint();
    twidget& end_paint(pencil&);
    rem::cc  set_theme(const std::string& theme_name);
    twidget& set_geometry(const rectangle& r);
    twidget& set_geometry(const cxy& xy, const ui::size& wh);

};

} // namespace tux::ui
