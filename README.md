
# project libtuxnbits++ ( my oldbitsnbytes c++ code Libraries )

# Roadmap of this htuxyist project

## Classes ( namespace tux ) main directory( for ex.: #include <tuxnbits++/bits/string.h>)

    - [class tux::string]
        Composite accumulator for serializing and formatting supported components into its container - std::string.
    - [tux::color]
        Ansi (and future HTML) color mnemonics and renderer(s)
    - [rem : namespace rem : Return Enumrated Mnemonics]
        Coherant and consistent global meaningful return mnemonic codes.
        Also used by the messaging system and //book logging journal.
    - [class tux::signals::notify; tux::signals::signotify ]
        C++ template header for basic signal-slots - A modified copy of :
        https://schneegans.github.io/tutorials/2015/09/20/signal-slot
        -> Not fully multi-threads yet, but I am starting to explore using a working notify signals across threads.
    - [ tux::ui::geometry classes ]
        {tux::ui::cxy, tux::ui::size, tux::ui::rectangle, tux::ui::string2d} Bare 2D geometry components.
    - [::diagnostic - ]
        logging and journaling output to file driven by the rem codes set.
    - [class tux::object]
        Base Object class on the parent-child relational logistic ( smart-pointers not used there - Too much overload (yet?) )
    -[class  tux::glyph and  tux::accent_fr]
        Actually, some hardcoded unicode and french accent glyphes. I am Waiting to find a coherant and a definitive easy to use unicode API.
    -[tux::cmd::cmd_switch tux::cmd::line,  with notify-hook]
        Developping very simple and quick custom command-line arguments processor.
        -> It is very hard to create a coherent realistic model. POSIX or not, the syntax is a mess because there is NO format rules excepted white space separators and confused quotes and subshell/commands tokens....
    
<!--    - [tux::io::lfd tux::io::listener tux::io::console tux::io::kbhit tux::io::mouse tux::io::tcp_socket tux::io::vhcar]
        The custom, modest set of input-output api I am actually exploring, experimenting, writing...
    - [tux::lexer tux::token_data ...]
-->
---
...more descriptions to come.
---
copyrights ...,2025,... Serge Lussier
