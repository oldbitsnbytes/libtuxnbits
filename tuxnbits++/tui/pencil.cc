#include <tuxnbits++/tui/pencil.h>


namespace tux::ui
{
/*!
 *
 * @param bf
 * @param subregion_
 * @note Post-Construction state must be checked (_valid==true) prior to using any pencil routine due to the subregion validation.
 */
pencil::pencil(io::vchar::bloc* bf, rectangle subregion_): _bf(bf), _rect(subregion_), _colors(_bf->colours)
{
    _rect = _bf->geometry.tolocal() / subregion_;
    _valid = !!_rect;
}



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
    // nowrap :
    auto sz = std::min(txt.length(), static_cast<size_t>(_rect.width()-_rect.cursor.x));
    // else
    //auto sz = std::min(txt.length(), static_cast<size_t>(_bf->buffer->end() - _bf->_c_));

    ///@todo Check boudaries and set scroll flag if scrolling enabled...
    if (!sz)
        return rem::cc::rejected;

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

rem::cc pencil::draw_cadre(cadre::frame_matrix frame_model)
{
    home(true);

    return rem::cc::done;
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


io::vchar::iterator pencil::at(const cxy& xy)
{
    gotoxy(xy);
    return _bf->_c_;
}


void pencil::set_foreground_color(color::code fg)
{
    _bf->set_foreground_color(fg);
}


void pencil::set_background_color(color::code bg)
{
    _bf->set_background_color(bg);
}


void pencil::set_colors(color::pair cp)
{
    _bf->set_colors(cp);
}


pencil& pencil::clear(const ui::rectangle& r, color::pair cp)
{
    rectangle ar = _rect / r;
    if (!ar){
        auto l = diagnostic::error(); l << rem::cc::rejected << " Clear area request is out of boundaries (" << color::hotpink4 <<r << color::r << ")" << l;
        return *this;
    }
    _bf->set_colors(cp);
    for (int y=0; y<ar.height(); y++)
    {
        gotoxy(ar.a + ui::cxy{0,y});
        for (int x=0; x<ar.width(); x++)
            *_bf->_c_++ = io::vchar(color::pair(_bf->colours));

    }
    return *this;
}

pencil &pencil::home(bool abs)
{
    gotoxy({0,abs? 0: _rect.cursor.y});
    return *this;
}

pencil &pencil::operator << (const cxy& xy)
{
    gotoxy(xy);
    return *this;
}


/*!
 * @brief Prefix increment operator : applies to the x coord.
 * @return ref to self
 */
pencil& pencil::operator++()
{
    ///@note fix ambiguous wrap/nowrap check here...
    if (! ++_rect) return *this;
    // _rect is checked and computed.
    _bf->gotoxy(_rect.cursor + _rect.a);

    return *this;
}

/*!
 * @brief Prefix increment operator : applies to the y coord.
 * @return ref to self
 */
pencil& pencil::operator++(int)
{
    ///@note fix ambiguous wrap/nowrap/scroll check here...
    if (! _rect++) return *this; // _rect.scroll_dir is always set in this case.
    // _rect is checked and computed.
    _bf->gotoxy(_rect.cursor + _rect.a);

    return *this;
}


pencil& pencil::operator--()
{
    if (! --_rect) return *this;
    _bf->gotoxy(_rect.cursor + _rect.a);
    return *this;
}


pencil& pencil::operator--(int)
{
    if (! _rect--) return *this;
    return *this;

}


pencil &pencil::draw_frame(cadre::frame_matrix frmat)
{
    home() << cadre::TopLeft
             << _rect.top_right() << cadre::TopRight
             << _rect.bottom_right() << cadre::BottomRight
             << _rect.bottom_left() << cadre::BottomLeft;

    auto l = diagnostic::debug(); l << pretty_id() << " les hosizontaux :" << l;
    auto pos = at(ui::cxy{1,0}); // Real assign from here.
    std::fill(pos, pos + _rect.dwh.w-2, io::vchar(_colors) << cadre::Horizontal);
    pos = at(ui::cxy{1,_rect.b.y});
    std::fill(pos, pos + _rect.dwh.w-2, io::vchar(_colors) << cadre::Horizontal);

    l << pretty_id() << " la verticalité :" << l;
    for(int y=1; y < _rect.dwh.h-1; y++)
        *this << ui::cxy{0,y} << cadre::Vertical << ui::cxy{_rect.b.x,y} << cadre::Vertical;

    l << rem::cc::done << l;
    return *this;
}



} // namespace tux::ui
