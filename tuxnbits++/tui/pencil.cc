#include <tuxnbits++/tui/pencil.h>


namespace tux::ui
{


pencil::pencil(io::vchar::bloc* bf, rectangle subregion_): _bf(bf), _rect(subregion_), _colors(_bf->colours){}



///////////////////////////////////////////////////////////////////////////////////////
/// \brief pencil::~pencil
///   Restores the colors to the Bloc Buffer's original before destroying this pencil.
///
pencil::~pencil()
{
    _bf->set_colors(_colors);
    //...
}

rem::cc pencil::draw_text(const std::string &txt)
{
    auto sz = std::min(txt.length(), static_cast<size_t>(_rect.width()));

    return rem::cc::notimplemented;
}



///////////////////////////////////////////////////////////////////////////////////////
/// \brief pencil::draw_glyph
/// \param ic
/// \return
///
rem::cc pencil::draw_glyph(glyph::type ic)
{
    return draw_text(glyph::data[ic]);
}



///////////////////////////////////////////////////////////////////////////
/// \brief pencil::gotoxy
/// \param xy
/// \return  accpeted or rejected.
///
rem::cc pencil::gotoxy(const cxy &xy)
{
    if(!_rect.goto_xy(xy)) return rem::cc::rejected;
    _bf->gotoxy(_rect.cursor + _rect.a);
    return rem::cc::accepted;
}






} // namespace tux::ui
