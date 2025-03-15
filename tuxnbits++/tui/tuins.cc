


#include <tuxnbits++/tui/tuins.h>

namespace tux::ui::colors
{



attr_db::themes attr_db::s_data =
{
    {
        "Default",
        {
            {
                "Widget",
                {
                    {uistate::Active, {color::grey100, color::grey27}},
                    {uistate::Disable, {color::grey30, color::grey35}}, // keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::grey27}},
                    {uistate::Focus, {color::red4, color::grey27}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistate::Active, {color::lightcyan3, color::darkblue}},
                    {uistate::Disable, {color::grey30, color::darkblue}}, // keep coherence with widgets base
                    {uistate::Normal, {color::grey100, color::darkblue}},
                    {uistate::Focus, {color::greenyellow, color::darkblue}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistate::Active, {color::indianred, color::blue}},
                    {uistate::Disable, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::blue}}, // Keep coherence with widgets base
                    {uistate::Focus, {color::yellow, color::blue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistate::Active, {color::hotpink4, color::grey27}},
                    {uistate::Disable, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::grey27}},
                    {uistate::Focus, {color::red4, color::grey27}}
                }
            },
            {
                "Icon",
                {
                    {uistate::Active, {color::blue1, color::grey27}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey19, color::grey35}},
                    {uistate::Normal, {color::blue1, color::grey35}},
                    {uistate::Focus, {color::aquamarine3, color::grey27}},
                    //{uistate::Success, {color::green5, color::grey27}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistate::Active, {color::white, color::grey30}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey39, color::grey35}},
                    {uistate::Normal, {color::grey37, color::grey30}},
                    {uistate::Focus, {color::white, color::grey30}}
                }
            },
            {
                "Input-Field",
                {
                    {uistate::Active, {color::grey70, color::grey27}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey39, color::grey35}},
                    {uistate::Normal, {color::grey70, color::grey27}},
                    {uistate::Focus, {color::white, color::grey27}}
                },
            },
            {
                "Input-Field:Placeholder",
                {
                    {uistate::Active, {color::darkcyan, color::grey27}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::blue, color::grey27}},
                    {uistate::Normal, {color::blue, color::grey27}},
                    {uistate::Focus, {color::blue, color::grey27}}
                }
            },
            {
                "StatusBar",
                {
                    {uistate::Active, {color::lime,      color::grey27}},
                    {uistate::Disable, {color::grey30,   color::grey27}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70,    color::grey27}},
                    {uistate::Focus, {color::red4,       color::grey27}}
                  //...
                }
            }
        }
    },
    {
        "C64",
        {
            {
                "Widget",
                {
                    {uistate::Active, {color::darkturquoise, color::darkblue}},
                    {uistate::Disable, {color::grey30, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::darkblue}},
                    {uistate::Focus, {color::red4, color::darkblue}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistate::Active, {color::lightcyan3, color::darkblue}},
                    {uistate::Disable, {color::grey30, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey100, color::darkblue}},
                    {uistate::Focus, {color::greenyellow, color::darkblue}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistate::Active, {color::lightcyan3, color::darkblue}},
                    {uistate::Disable, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Focus, {color::yellow, color::darkblue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistate::Active, {color::hotpink4, color::darkblue}},
                    {uistate::Disable, {color::grey19, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::darkblue}},
                    {uistate::Focus, {color::red4, color::darkblue}}
                }
            },
            {
                "Icon",
                {
                    {uistate::Active, {color::blue1, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey19, color::darkblue}},
                    {uistate::Normal, {color::blue1, color::darkblue}},
                    {uistate::Focus, {color::aquamarine3, color::darkblue}},
                    //{uistate::Success, {color::green5, color::darkblue}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistate::Active, {color::white, color::blue}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey39, color::blue}},
                    {uistate::Normal, {color::grey37, color::blue}},
                    {uistate::Focus, {color::white, color::blue}}
                }
            },
            {
                "Input-Field",
                {
                    {uistate::Active, {color::lightcyan3, color::blue4}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::blue, color::darkblue}},
                    {uistate::Normal, {color::blue, color::darkblue}},
                    {uistate::Focus, {color::blue, color::darkblue}}
                },

            },
            {
                "Input-Field:Placeholder",
                {
                    {uistate::Active, {color::darkcyan, color::blue4}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::blue, color::darkblue}},
                    {uistate::Normal, {color::blue, color::darkblue}},
                    {uistate::Focus, {color::blue, color::darkblue}}
                },
            },
            {
                "StatusBar",
                {
                    {uistate::Active, {color::lime, color::blue4}},
                    {uistate::Disable, {color::grey30, color::blue4}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::blue4}},
                    {uistate::Focus, {color::red4, color::blue4}}
                      //...
                }
            }
        }
    },
    {
        "C128",
        {
            {
                "Widget",
                {
                    {uistate::Active, {color::darkseagreen, color::grey15}},
                    {uistate::Disable, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::grey15}},
                    {uistate::Focus, {color::red4, color::grey15}}
                    //...
                }
            },
            {
                "Frame",
                {
                    {uistate::Active, {color::darkseagreen, color::grey15}},
                    {uistate::Disable, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::lightcyan3, color::grey15}},
                    {uistate::Focus, {color::greenyellow, color::grey15}}
                    //...
                }
            },
            {
                "Label",
                {
                    {uistate::Active, {color::darkseagreen4, color::grey15}},
                    {uistate::Disable, {color::grey19, color::grey35}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Focus, {color::yellow, color::darkblue}}
                }
            },
            {
                "Shortkey",
                {
                    {uistate::Active, {color::hotpink4, color::darkblue}},
                    {uistate::Disable, {color::grey19, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::darkblue}},
                    {uistate::Focus, {color::red4, color::darkblue}}
                }
            },
            {
                "Icon",
                {
                    {uistate::Active, {color::yellow3, color::darkblue}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::grey19, color::darkblue}},
                    {uistate::Normal, {color::blue1, color::darkblue}},
                    {uistate::Focus, {color::aquamarine3, color::darkblue}},
                    //{uistate::Success, {color::green5, color::darkblue}}
                }
            },
            {
                "Input-Area", // widgets::text_input
                {
                    {uistate::Active, {color::darkseagreen, color::grey15}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::darkseagreen, color::grey15}},
                    {uistate::Normal, {color::darkseagreen, color::grey15}},
                    {uistate::Focus, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "Input-Field",
                {
                    {uistate::Active, {color::darkseagreen, color::grey11}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::darkseagreen, color::grey15}},
                    {uistate::Normal, {color::darkseagreen, color::grey15}},
                    {uistate::Focus, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "Input-Field:Placeholder",
                {
                    {uistate::Active, {color::grey23, color::grey11}}, // Keep coherence with widgets base
                    {uistate::Disable, {color::darkseagreen, color::grey15}},
                    {uistate::Normal, {color::darkseagreen, color::grey15}},
                    {uistate::Focus, {color::darkseagreen, color::grey15}}
                }
            },
            {
                "StatusBar",
                {
                    {uistate::Active, {color::lime, color::grey15}},
                    {uistate::Disable, {color::grey30, color::grey15}}, // Keep coherence with widgets base
                    {uistate::Normal, {color::grey70, color::grey15}},
                    {uistate::Focus, {color::red4, color::grey15}}
                    //...
                }
            }
        }
    }
};


attr_db::themes::iterator attr_db::get_theme(std::string_view Tname)
{
    auto it = attr_db::s_data.begin();
    auto it_end = attr_db::s_data.end();
    while (it != it_end)
    {
        if (it->first == Tname) break;
        ++it;
    }
    // ReSharper disable once CppDFALocalValueEscapesFunction
    return it;
}


bool attr_db::end(attr_db::themes::iterator it)
{
    return it == attr_db::s_data.end();
}

attr_db::themes &tux::ui::colors::attr_db::theme()
{
    return attr_db::s_data;
}

}
