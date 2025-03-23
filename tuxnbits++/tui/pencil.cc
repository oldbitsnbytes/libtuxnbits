#include <tuxnbits++/tui/pencil.h>


namespace tux::ui
{

pencil::pencil() {}

pencil::pencil(io::vchar::bloc* bf, rectangle subregion_): _bf(bf), _rect(subregion_){}




} // namespace tux::ui
