#include <tuxnbits++/io/vchar.h>


namespace tux::io
{

#pragma region vchar-bloc
// ---------------------------------------------------------------------------------------------------------------------


vchar::bloc& vchar::bloc::cursor(ui::cxy _pos)
{
    if (!geometry.goto_xy(_pos))
    {
        state = rem::cc::oob;
        return *this;
    }
    state = rem::cc::accepted;
    return *this;
}


int vchar::bloc::line() const
{
    return geometry.cursor.y;
}


int vchar::bloc::column() const
{
    return geometry.cursor.x;
}


vchar::bloc& vchar::bloc::operator<<(const std::string& _str)
{
    if (_str.length() >= (buffer->end() - _c_))
        throw diagnostic::exception()[
            diagnostic::except(1) << rem::type::fatal << rem::cc::oob << rem::fn::endl
                                                            << "\nstrlen:" << color::yellow << _str.length() << color::r
                                                            << "\nbuffer length from end() pointer: " << color::yellow << (buffer->end() - buffer->begin())
                                                            << "\nlogical cursor: " << color::yellow << geometry.cursor
        ];

    for (auto c: _str)
    {
        **_c_++ << colours << c;
    }
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(glyph::type _glyph)
{
    **_c_ << colours << _glyph;
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(color::code _color)
{
    _c_->set_bg(_color);
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(color::pair _colors)
{
    colours =  _colors;
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(cadre::index _frameindex)
{
    **_c_ << _frameindex;
    return *this;
}

/*!
 * @note bypasses natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge Lussier (oldlonecoder)
 */
bool vchar::bloc::operator++()
{
    if (++geometry)
    {
        ++_c_;
        return true;
    }
    return false;
}


/*!
 * @note bypass natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge Lussier (oldlonecoder)
 */
bool vchar::bloc::operator++(int)
{
    if (++geometry)
    {
        ++_c_;
        return true;
    }
    return false;
}


/*!
 *
 * @note bypass natural increment-decrement because this method is the convenient way to advance the internal cursor with our boundaries.
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge Lussier (oldlonecoder)
 */
bool vchar::bloc::operator--()
{
    if (--geometry)
    {
        --_c_;
        return true;
    }
    return false;
}



/*!
 *
 * @return true if the cursor is within the boundaries  false otherwise.
 * @author Serge Lussier (oldlonecoder)
 */
bool vchar::bloc::operator--(int)
{
    if (--geometry)
    {
        --_c_;
        return true;
    }
    return false;
}



vchar::bloc::~bloc()
{
    buffer->clear();
}


vchar::bloc::operator bool() const
{
    return !buffer->empty() && geometry;
}


vchar::bloc* vchar::bloc::create(const ui::size& _dim, color::pair _colours)
{
    vchar::bloc* blk = new vchar::bloc;
    blk->buffer = std::make_shared<vchar::string>(_dim.area(), vchar(color::pair(_colours)));
    blk->colours = _colours;
    blk->geometry = {{0,0},_dim};
    blk->home();
    auto l = diagnostic::debug(1); l << " Confirm bloc buffer size:" << color::yellow << blk->buffer->size() << l;
    return blk;
}

rem::cc vchar::bloc::set_pos(cxy xy)
{
    if(!geometry.in(xy)){
        auto l = diagnostic::error(); l << rem::cc::oob << ":{" << color::hotpink4 << xy << color::r << "}" << l;
        return rem::cc::rejected;
    }
    geometry.moveat(xy);
    return rem::cc::accepted;
}

void vchar::bloc::clear()
{
    std::fill_n(buffer->begin(),geometry.dwh.area(), vchar(color::pair(colours)));
}


void vchar::bloc::sync_cursors()
{
    auto  w = _c_ - buffer->begin();
    geometry.cursor = {static_cast<int>(w) % *geometry.width(),static_cast<int>(w) / *geometry.width()};
    end_pos = geometry.cursor;
}

rem::cc vchar::bloc::home()
{
    if(geometry.goto_xy({0,0})){
        _c_ = buffer->begin();
        return rem::cc::accepted;
    }
    return rem::cc::rejected;
}

rem::cc vchar::bloc::gotoxy(cxy xy)
{
    if(geometry.goto_xy(xy)){
        _c_ = buffer->begin() + (*geometry.width() * geometry.cursor.y + geometry.cursor.x);
        return rem::cc::accepted;
    }
    return rem::cc::rejected;
}

void vchar::bloc::set_foreground(color::code fg) { colours.fg = fg; }
void vchar::bloc::set_background(color::code bg) { colours.bg = bg; }
color::code vchar::bloc::fg() { return colours.fg; }
color::code vchar::bloc::bg() { return colours.bg; }




#pragma endregion char32-bloc

// ---------------------------------------------------------------------------------------------------------------------


}
