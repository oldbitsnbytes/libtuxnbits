
// Created by oldlonecoder on 8/13/24.
//

//#ifndef INTUI_GLOBALS_H
//#define INTUI_GLOBALS_H
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


//#include <csignal>
#include <map>
//#include <vector>
#include <string_view>
#include <tuxnbits++/bits/string.h>
#include <tuxnbits++/tui/export.h>

using namespace tux::integers;

namespace tux::ui
{


/**
 * @brief Widget ui justification enums or`ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace justify
{
using Type = u16;
static constexpr u16 left        = 0x0001; ///< Justify left
static constexpr u16 right       = 0x0002; ///< Justify right
static constexpr u16 hcenter     = 0x0004; ///< Justify align horizontally at the center
static constexpr u16 vcenter     = 0x0008; ///< Justify Align vertically at the center
static constexpr u16 center      = 0x000C; ///< Justify Vertical and Horizontal center
static constexpr u16 word_wrap   = 0x0010; ///< Justify word wrap
static constexpr u16 prefix      = 0x0020; ///< Justify prefix
static constexpr u16 suffix      = 0x0040; ///< Justify suffix
static constexpr u16 auto_size   = 0x0080; ///< Justify auto-stretch
static constexpr u16 auto_size_text   = 0x0100; ///< Justify auto-stretch
}


/**
 * @brief tux::ui::rectangle enums values
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace direction
{
using type = I8;
static constexpr I8 left = -1; ///< to left
static constexpr I8 right = 1; ///< to right
static constexpr I8 up = 2;    ///< up
static constexpr I8 down = -2; ///< down
}


namespace components
{
using type = u16;
static constexpr u16 None      = 0;
static constexpr u16 Caption   = 0x0001; ///< Caption widgets
static constexpr u16 Glyph     = 0x0002; ///< Glyph widgets
static constexpr u16 StatusBar = 0x0004; ///< Status bar
static constexpr u16 HScrollBar= 0x0008; ///< horizontal scrollbar
static constexpr u16 VScrollBar= 0x0010; ///< vertical scrollbar
static constexpr u16 Frame     = 0x0020; ///< Widget has a frame surrounding it (within geometry)
static constexpr u16 MenuBar   = 0x0040; ///< Widget has a MenuBar system
static constexpr u16 PrefixGlyph   = 0x0080; ///< Widget has a Prefixed Glyph ( Icon )
static constexpr u16 SuffixGlyph   = 0x0100; ///< Widget has a Suffix Glyph ( Icon )
static constexpr u16 InputStats = 0x0100; ///< Widget input tracking report child widget

}

namespace uistyle
{
using Type = u32;
static constexpr u32 None        = 0x0000; ///< Unset
static constexpr u32 Input       = 0x00000010; ///< Widget has or is an Input Field
static constexpr u32 MultiLine   = 0x00000020; ///< Multiline input field
static constexpr u32 HScrollable = 0x00000040; ///< horizontally scrollable
static constexpr u32 VScrollable = 0x00000080; ///< vertically scrollable
static constexpr u32 Window      = 0x00000100; ///< Toplevel Window
static constexpr u32 Resizable   = 0x00000200; ///< Resizable widgets
static constexpr u32 Maximizable = 0x00000400; ///< Maximizable ->
static constexpr u32 Minimizable = 0x00000800; ///< and minimizable widgets
}
/**
 * @brief Widget ui state enums or'ed flags
 * @author &copy; 2008,2022,2023, Serge Lussier, lussier.serge@gmail.com ( oldlonecoder )
 */
namespace uistate
{
using Type = u16;
static constexpr u16 Normal    = 0x0000;
static constexpr u16 Disable   = 0x0001;
static constexpr u16 Active    = 0x0002;
static constexpr u16 Focus     = 0x0004;
static constexpr u16 Visible   = 0x0080;
static constexpr u16 Dirty     = 0x0100;
static constexpr u16 Maximized = 0x0400;
static constexpr u16 Minimized = 0x0800;
}


namespace uiclass
{
using Type = u16;
static constexpr u16 None = 0x00;
static constexpr u16 Error = 0x0001;
static constexpr u16 Warning = 0x0002;
static constexpr u16 Illegal = 0x0004;
static constexpr u16 Invalid = 0x0008;
static constexpr u16 Success = 0x0100;
static constexpr u16 Information = 0x0200;
}


/**
 * geometry policies for stretching and positioning children widgets
 */
namespace anchor
{
using value = U16;
static constexpr anchor::value None    = 0x0000; ///< free fixed xy placement
static constexpr anchor::value fixed    = 0x0001; ///< free fixed xy placement
static constexpr anchor::value Width    = 0x0002; ///< Fits the entire width size
static constexpr anchor::value Height   = 0x0004; ///< Fits the entire height size
static constexpr anchor::value Frame    = 0x0008; ///< Overlaps if wh size > 1, or sits on the frame.
static constexpr anchor::value Right    = 0x0010;
static constexpr anchor::value Left     = 0x0020;
static constexpr anchor::value Top      = 0x0040;
static constexpr anchor::value Bottom   = 0x0080;
static constexpr anchor::value Center   = 0x0100; ///< Both fit_h&vcenter ...
static constexpr anchor::value HCenter  = 0x0200;
static constexpr anchor::value VCenter  = 0x0400;

//...
}

namespace colors
{

/*!
 * \brief The attr_db class
 * Database set of the colors. Theme, components, then state
 */
struct LIBTUXNBITSTUI attr_db
{
    using components = std::map<uistate::Type,   color::pair>;
    using elements = std::map<std::string_view, attr_db::components>;
    using themes = std::map<std::string_view,   attr_db::elements>;
    //using theme     = std::map<std::string_view, db::dictionary>;
    static colors::attr_db::themes s_data;

    static attr_db::themes::iterator get_theme(std::string_view theme_name);
    static bool end(attr_db::themes::iterator it);
    //static attr_db::components& components_table(attr_db::themes::iterator i);
    static themes& theme();
};
} // namespace colors;

} // namespace tux::ui
