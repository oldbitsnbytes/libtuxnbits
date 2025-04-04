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
#include <tuxnbits++/tui/export.h>
#include <tuxnbits++/io/kbhit.h>
#include <tuxnbits++/io/mouse.h>
//#include <tuxnbits++/tui/tuins.h>
#include <variant>



namespace tux::ui
{

struct LIBTUXNBITSTUI event
{
    u64 bits{0};
    static constexpr u64 noop           = 0x0000000000000000;
    static constexpr u64 open           = 0x0000000000000001;
    static constexpr u64 close          = 0x0000000000000002;
    static constexpr u64 cancel         = 0x0000000000000004;
    static constexpr u64 dismiss        = 0x0000000000000008;
    static constexpr u64 begin          = 0x0000000000000010;
    static constexpr u64 end            = 0x0000000000000020;
    static constexpr u64 terminate      = 0x0000000000000040;
    static constexpr u64 input          = 0x0000000000000080;
    static constexpr u64 output         = 0x0000000000000100;
    static constexpr u64 btnleftup      = 0x0000000000000200;
    static constexpr u64 btnrightup     = 0x0000000000000400;
    static constexpr u64 btnmidup       = 0x0000000000000800;
    static constexpr u64 btnleftdown    = 0x0000000000001000;
    static constexpr u64 btnrightdown   = 0x0000000000002000;
    static constexpr u64 btnmiddown     = 0x0000000000004000;
    static constexpr u64 keypress       = 0x0000000000008000;
    //...

    struct command
    {
        u64 cmd{0};
    };


    std::variant<io::kbhit,io::mouse, event::command> data{};
    ////////////////////////////////////////////////////////////
    /// \brief Check current event_type
    ///
    /// \tparam `EVT` Type of the event_type to check against
    ///
    /// \return `true` if the current event_type matches given template parameter
    ///
    ////////////////////////////////////////////////////////////
    template <typename EVT> [[nodiscard]] bool is() const;

    ////////////////////////////////////////////////////////////
    /// \brief Attempt to get specified event event_type
    ///
    /// \tparam `event_type` Type of the desired event event_type
    ///
    /// \return Address of current event event_type, otherwise `nullptr`
    ///
    ////////////////////////////////////////////////////////////
    template <typename EVT> [[nodiscard]] const EVT* get() const;

    ////////////////////////////////////////////////////////////
    /// \brief Apply a visitor to the event
    ///
    /// \param visitor The visitor to apply
    ///
    /// \return The result of applying the visitor to the event
    ///
    ////////////////////////////////////////////////////////////
    template <typename T> decltype(auto) visit(T&& visitor) const;



private:
    ////////////////////////////////////////////////////////////
    // Helper functions
    ////////////////////////////////////////////////////////////
    template <typename T, typename... Ts> [[nodiscard]] static constexpr bool is_in_params_pack(const std::variant<Ts...>*)
    {
        return (std::is_same_v<T, Ts> || ...);
    }

    template <typename T> static constexpr bool is_event_type = is_in_params_pack<T>(decltype (&data)(nullptr));


    rem::cc translate();
};


} // namespace tux::ui

//#include <tuxnbits++/tui/event.inline>
