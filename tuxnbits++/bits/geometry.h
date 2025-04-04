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
#include <tuxnbits++/bits/string.h>

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

struct LIBTUXNBITS cxy {

    int x = -1;
    int y = -1;

    using array = std::vector<cxy>;
    using iterator = cxy::array::iterator;
    using citerator = cxy::array::const_iterator;

    cxy() = default;
    cxy(cxy &&) noexcept = default;
    ~cxy() = default;
    cxy(int c_x, int c_y) : x(c_x), y(c_y){}
    cxy(const cxy &p) = default;
    cxy &operator=(const cxy &p) = default;
    cxy &operator-=(const cxy &dxy);
    cxy &operator+=(const cxy &dxy);
    bool operator==(cxy rhs) const;
    bool operator>(cxy rhs) const;
    bool operator<(cxy rhs) const;
    cxy operator+(const cxy &dxy) const;

    cxy operator+(int dx) const
    {
        return {dx + x, y};
    }
    [[nodiscard]] cxy get_max(const cxy &rhs) const;
    [[nodiscard]] cxy get_min(const cxy &rhs) const;
    cxy operator-(const cxy &rhs) const;
    [[nodiscard]] cxy lesser(cxy rhs) const;
    [[nodiscard]] cxy greater(cxy rhs) const;
    // cxy operator<<(cxy rhs) const;
    // cxy operator>>(cxy rhs) const;
    cxy &operator++();
    cxy &operator++(int);
    cxy &operator--();
    cxy &operator--(int);
    cxy &operator()(int x_, int y_);
    operator std::string() const;
};


struct LIBTUXNBITS size {
    int w{0};
    int h{0};

    // size policies:
    cxy min_size{};
    cxy max_size{};
    // --------------
    operator bool() const { return ((w > 0) && (h > 0)); }
    operator std::string() const;
    bool operator<(const size &dwh) const;
    bool operator==(const size &dwh) const;
    bool operator>(const size &dwh) const;
    bool operator!=(const size &dwh) const;
    void set_min_max(cxy mi, cxy ax);
    void set_min_size(int _w, int _h);
    void set_max_size(int _w, int _h);
    [[nodiscard]] int32_t area() const { return w*h;}
    bool has_minmax() const;
    bool has_min_size();
    bool has_max_size();
    int width() { return w; }
    int height() { return h; }

};


/*!
 * @brief Rectangular Geometry object (inspired by the Qt project, many, many years ago).
 *
 * It has evolved with my own point of view and limitations.
 * @li Provides internal Cursor relative coordinates
 * @li Still needs more limit implementations (min-max restrictions helpers)
 *
 * @author oldlonecoder (lussier.serge@gmail.com)
 *
 */
struct LIBTUXNBITS rectangle {

    cxy a{0, 0};
    cxy b{0, 0};
    cxy cursor{0,0}; ///< inner cursor coordinates used by the location operations, relative to rectangle::a (topleft) coord.

    size dwh;
    bool nowrap = true;
    direction::type scroll_dir{0};
    //static constexpr std::string_view    string_format = R"((\{3d},\{-3d})-[a:(\{3d},\{-3d}) b:(\{3d},\{-3d})]-[w:\{3d} h:\{-3d}])";  Disabled;

    using array = std::vector<rectangle>;
    using iterator = rectangle::array::iterator;
    using citerator = rectangle::array::const_iterator;

    rectangle() = default;
    ~rectangle() = default;
    rectangle(rectangle &&r) noexcept = default;
    rectangle(const rectangle &r) = default;
    rectangle(const cxy &a_, const cxy &b_);
    rectangle(const cxy &a_, const size &d);
    rectangle(int x, int y, int w, int h);
    explicit rectangle(const size &dxy);
    rectangle &operator=(rectangle &&r) noexcept = default;
    rectangle &operator=(const rectangle &r) = default;
    void assign(int x, int y, int w, int h);
    void assign(const cxy &a_, const cxy &b_);
    void assign(const cxy &a_, const size &dxy);
    cxy local();
    rectangle tolocal();
    cxy relative();
    void set_topleft(cxy pt);
    rectangle grow(cxy dxy);
    bool operator == (const rectangle& rhs) const;
    rectangle &operator+=(const cxy &dx);
    rectangle &operator-=(const cxy &dx);
    rectangle &operator|=(const rectangle& rhs);
    void resize(const size &new_sz);
    void moveat(const cxy &p);
    bool in(const cxy &pt) const;
    void move(const cxy &deltapt);
    std::tuple<cxy,cxy,ui::size> components() const { return {a, b,dwh}; }
    int width() { return dwh.w; }
    int height() { return dwh.h; }
    bool operator[](const cxy &pt) const;
    cxy top_left()const;
    cxy top_right()const;
    cxy bottom_left()const;
    cxy bottom_right()const;
    [[nodiscard]] rectangle operator&(const rectangle &r) const;

    /*!
     * \brief operator / get the intersection between this and rhs then get the origin of this from rhs
     * \param rhs
     * \return result rectangle
     */
    rectangle operator/(const rectangle &rhs) const;

    /*!
        @brief merges this and r
    */
    rectangle operator|(const rectangle &r) const;
    rectangle operator+(const cxy &pt) const;
    rectangle operator-(const cxy &pt) const;
    [[nodiscard]] std::string tostring() const;
    operator std::string() const;
    operator bool() const { return (dwh); }//.operator bool();
    //------------- location operations ---------------------------------
    void home();
    bool operator++();
    bool operator++(int);
    bool operator--();
    bool operator--(int);
    bool goto_xy(cxy xy);
    cxy operator*() const
    { return cursor; }

};


struct LIBTUXNBITS string2d
{
    tux::string win{};
    rectangle r;

    string2d& gotoxy(int x, int y);
    string2d& operator<< (cxy xy);
    void set_geometry(int w, int h);

    // internal cursor movements:
    string2d& operator++();
    void operator++(int);
    string2d& operator--();
    void operator--(int);
    // --------------------------
    template<typename t> string2d& operator<< (t v)
    {
        tux::string str;
        str << v;
        return put(str());
    }




    string2d& put(const std::string& txt);

    void clear();
    std::string details();
    operator std::string();// {return win ? win->str() : "";}

};

}
