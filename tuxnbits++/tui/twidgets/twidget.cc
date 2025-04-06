//
// Created by oldlonecoder on 25-04-06.
//

#include <tuxnbits++/tui/twidgets/twidget.h>

namespace tux::ui
{



twidget::twidget(twidget* _parent_widget, const std::string& twid) : object(_parent_widget, twid){}


twidget::~twidget() {}


/*!
 *
 * @brief Creates a pencil object for the caller who can use any time and as long as needed.
 * @return ref to the pencil instance.
 */
pencil& twidget::begin_paint()
{
    // ...
    auto* p = new pencil(_bf, {});
    //...
    return *p;
}


/*!
 * @brief Release the pencil instance given to the user who does not need it anymore.
 * @param painter
 * @return done...
 */
rem::cc twidget::end_paint(pencil& painter)
{
    // Dirty the painter's geometry...
    //...
    delete &painter;
    return rem::cc::done;
}



} // namespace tux::ui
