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
            diagnostic::except(1) << rem::type::fatal << rem::cc::oob
                << rem::fn::endl << "strlen:" << color::yellow << _str.length() << color::r
                << rem::fn::endl << "buffer length from end() pointer: " << color::yellow << (buffer->end() - buffer->begin())
                << rem::fn::endl << "logical cursor: " << color::yellow << geometry.cursor
        ];

    for (auto c: _str)
        **_c_++ << colours << c;
    sync_cursors(); // We must sync internal cursors before further inputs.
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(glyph::type _glyph)
{
    **_c_++ << colours << _glyph;
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
    **_c_++ << _frameindex;
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(ui::cxy xy)
{
    gotoxy(xy);
    // Handle result...
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(const vchar::bloc& blk)
{
    return *this;
}


vchar::bloc& vchar::bloc::operator<<(char c)
{
    **_c_ << c;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::clear
///     Clears sub-area given by \arg r using colors attributes \arg cp
/// \param r
/// \param cp
/// \return accepted if all of the clear went ok, rem::cc::rejected otherwise
///
rem::cc vchar::bloc::clear(ui::rectangle r, color::pair cp)
{
    auto rw = geometry.tolocal() & (r);
    if(!rw){
        auto l = diagnostic::status(); l << rem::cc::oob << " : request area:" << r << "; within " << geometry.dwh << l;
        return rem::cc::rejected;
    }

    for(int y=0; y < rw.height(); y++){
        if(auto c = gotoxy({rw.a.x, rw.a.y+y}); !c)
            return c;
        for(int x=0; x < rw.width(); x++){
            ***this << cp << ' ';
            if(!++*this)
                return rem::cc::rejected;
        }
    }
    return rem::cc::accepted;
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

rem::cc vchar::bloc::draw_frame()
{
    return rem::cc::notimplemented;
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
    const auto blk = new vchar::bloc;
    blk->buffer = std::make_shared<vchar::string>(_dim.area(), vchar(color::pair(_colours)));
    blk->colours = _colours;
    blk->geometry = {{0,0},_dim};
    blk->home();
    auto l = diagnostic::debug(1); l << " Confirm bloc buffer size:" << color::yellow << blk->buffer->size() << l;
    return blk;
}



/**
 * @brief vchar::bloc::set_pos
 *      Set this bloc's geometry's topleft position to \arg xy
 * @param xy
 * @return always rem::cc::accepted.
 */
void vchar::bloc::set_pos(cxy xy)
{
    geometry.moveat(xy);
}


//////////////////////////////////////////////////////////////
/// \brief vchar::bloc::clear
///        Clears the buffer with the current colors attributes
void vchar::bloc::clear()
{
    std::fill_n(buffer->begin(),geometry.dwh.area(), vchar(color::pair(colours)));
}

/////////////////////////////////////////////////////////////
/// \brief vchar::bloc::sync_cursors
///     Sync's the internal cursor at the cooridantes from the offset of _c_.
///
void vchar::bloc::sync_cursors()
{
    const auto  w = _c_ - buffer->begin();
    geometry.cursor = {static_cast<int>(w) % geometry.width(), static_cast<int>(w) / geometry.width()};
    end_pos = geometry.cursor;
}


//////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::home
///     Resets iterator and internal cursor at {0,0};
///
void vchar::bloc::home() { geometry.goto_xy({0,0});_c_ = buffer->begin(); }


///////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::gotoxy
///     Set internal coursor position to xy and set _c_ to the corresponding linear offset value.
/// \param xy
/// \return
///
rem::cc vchar::bloc::gotoxy(cxy xy)
{
    if(geometry.goto_xy(xy)){
        _c_ = buffer->begin() + (geometry.width() * geometry.cursor.y + geometry.cursor.x);
        return rem::cc::accepted;
    }
    return rem::cc::rejected;
}



#pragma region colors
////////////////////////////////////////
/// \defgroup bloc-colors
///     Just copying into the current colors member vchar::bloc::colours
///


///////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::set_foreground_color
/// \param fg
/// \group bloc-colors
void vchar::bloc::set_foreground_color(color::code fg) { colours.fg = fg; }

///////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::set_background_color
/// \param bg
/// \group bloc-colors
void vchar::bloc::set_background_color(color::code bg) { colours.bg = bg; }

///////////////////////////////////////////////////////
/// \brief vchar::bloc::set_colors
/// \param cp
/// \group bloc-colors
void vchar::bloc::set_colors(color::pair cp) { colours = cp; }

////////////////////////////////////////////////////////////
/// \brief vchar::bloc::fg
/// \return
/// \group bloc-colors
///
color::code vchar::bloc::fg() { return colours.fg; }

////////////////////////////////////////////////////////////
/// \brief vchar::bloc::bg
/// \return
/// \group bloc-colors
///
color::code vchar::bloc::bg() { return colours.bg; }



////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::operator &
///     Intersect the rectangular geometry of this bloc with \arg rhs.
/// \param rhs
/// \return resulting of the intersection.
/// \note \arg rhs origin must be on the same origin of this bloc.
///
rectangle vchar::bloc::operator &(const rectangle rhs) { return geometry & rhs; }


////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::operator /
///     Intersect the rectangular inner geometry ( origin at {0,0} ) of this bloc with \arg rhs.
/// \param rhs
/// \return resulting of the intersection shifted to the inner origin {0,0}.
/// \note \arg rhs must be on the same origin scale of this bloc. So the resulting rectangle will have its offset moved to the relative geometry of this bloc
///
rectangle vchar::bloc::operator /(const rectangle rhs) { return geometry / rhs; }

vchar::string::iterator vchar::bloc::operator *() { return _c_; }

//-----------------------------------------------------------------------------
// TODO

/*
 *  +==================================================+
 *  |         ======================================== | ->
 *  |         ======================================== |  |
 * =|        ========================================  | <- nrows = 2
 *  |        ========================================  |
 *  |        ========================================  |
 *  |        ========================================  |
 *  |        ========================================  |
 *  |        ========================================  |
 *  |        ========================================  |       ->
 *  |        ========================================  |    ->  |
 *  |        ========================================  | ->  | <-
 *  |        ========================================  |    <-
 *  |        ========================================  | <-
 *  +==================================================+
 */
rem::cc vchar::bloc::scroll_up(int nrows)
{
    auto rect = rectangle{{0,0},ui::size{geometry.width(),geometry.height()-nrows}};
    auto [sz,blk] = copy(rect);
    put(sz,blk,ui::cxy{0,nrows});
    clear(rectangle{{0,0},ui::size{geometry.width(),nrows}},colours);

    return rem::cc::done;
}

/////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::scroll_down
/// \param nrows
///
void vchar::bloc::scroll_down(int nrows)
{
    auto rect = rectangle{{0,nrows},ui::size{geometry.width(),geometry.height()-nrows}};
    auto [sz,blk] = copy(rect);
    clear();
    put(sz,blk,ui::cxy{0,0});
}


////////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::scroll_left
/// \param nrows
///
void vchar::bloc::scroll_left(int nrows)
{
    auto rect = rectangle{{nrows,0},ui::size{geometry.width()-nrows,geometry.height()}};
    auto [sz,blk] = copy(rect);
    clear();
    put(sz,blk,ui::cxy{0,0});
}

////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::scroll_right
/// \param nrows
///
void vchar::bloc::scroll_right(int nrows)
{
    auto rect = rectangle{{0,0},ui::size{geometry.width()-nrows,geometry.height()}};
    auto [sz,blk] = copy(rect);
    clear();
    put(sz,blk,ui::cxy{nrows,0});
}



/////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::put
///
///     Puts (2d bloc to linear) vchar::string  into this bloc at the given coords.
/// \param sz  width-height of the vhcar::string
/// \param blk the vhcar::string
/// \param xy destination coords.
/// \return always accepted as of now.
///
rem::cc vchar::bloc::put(ui::size sz, const vchar::string& blk, cxy xy)
{

    for(int y = 0; y < sz.h; y++){
        gotoxy({xy.x,xy.y+y});
        auto sline = blk.begin() + y*sz.w;
        std::copy(sline, sline+sz.w ,_c_);
    }
    return rem::cc::notimplemented;
}


//////////////////////////////////////////////////////////////////////////////////
/// \brief vchar::bloc::copy
/// \param rect
/// \return
/// \todo Clip the rect.
///
std::pair<size, vchar::string> vchar::bloc::copy(rectangle rect)
{
    rectangle r = geometry.tolocal() & rect;
    if(!r)
        return {{0,0},{}};

    vchar::string str(r.dwh.area());
    auto i = str.begin();

    auto sz = r.dwh;
    for(int y = 0; y < sz.h; y++){
        gotoxy(r.a + cxy{0,y});
        auto sline = i + y*sz.w;
        std::copy(_c_, _c_ + sz.w, sline);
        //for(int x = 0; x < r.width(); x++) *i++ = *_c_++;
    }
    //...
    return {r.dwh,str};
}
//-----------------------------------------------------------------------------


}
