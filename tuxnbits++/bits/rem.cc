#include <map>
#include <tuxnbits++/bits/rem.h>

//#include <vector>

using tux::ui::color;
using tux::glyph;
using tux::accent_fr;

bool operator !(rem::cc c)
{
    return (c != rem::cc::accepted)&&
           (c != rem::cc::ok)      &&
           (c != rem::cc::success) &&
           (c != rem::cc::done)    &&
           (c != rem::cc::ready)   &&
           (c != rem::cc::finish)  &&
           (c != rem::cc::complete);
}

namespace rem
{
std::map<rem::type, const char*> enums_type_dictionary = {
    {rem::type::none,        "none"},
    {rem::type::err  ,       "err"},
    {rem::type::warning,     "warning"},
    {rem::type::fatal,       "fatal"},
    {rem::type::except,      "exception"},
    {rem::type::message,     "message"},
    {rem::type::output,      ""},
    {rem::type::debug,       "debug"},
    {rem::type::info,        "info"},
    {rem::type::comment,     "comment"},
    {rem::type::syntax,      "syntax error"},
    {rem::type::status,      "status"},
    {rem::type::test,        "test"},
    {rem::type::interrupted, "interrupted"},
    {rem::type::aborted,     "aborted"},
    {rem::type::segfault,    "segmentation fault"},
    {rem::type::book,         "book"},
    {rem::type::normal,       "normal state"}
};


std::map<rem::cc, const char*> return_codes_dictionary = {
    {rem::cc::ok,             "ok"},
    {rem::cc::accepted,       "accepted"},
    {rem::cc::success,        "success "},
    {rem::cc::rejected,       "rejected"},
    {rem::cc::failed,         "failed"},
    {rem::cc::empty,          "empty"},
    {rem::cc::full,           "full"},
    {rem::cc::notempty,       "not empty"},
    {rem::cc::implemented,    "implemented"},
    {rem::cc::notimplemented, "not implemented"},
    {rem::cc::untested,       "untested"},
    {rem::cc::eof,            "end of file"},
    {rem::cc::eos,            "end of stream or string"},
    {rem::cc::null_ptr,       "null pointer"},
    {rem::cc::notexist,       "does not exist"},
    {rem::cc::exist,          "exist"},
    {rem::cc::unexpected,     "unexpected"},
    {rem::cc::expected,       "expected"},
    {rem::cc::blocked,        "blocked"},
    {rem::cc::locked,         "locked"},
    {rem::cc::overflow,       "buffer overflow"},
    {rem::cc::oob,            "out of boundaries"},
    {rem::cc::reimplement,    "need to be re-implemented in derived instances"},
    {rem::cc::done,           "done"},
    {rem::cc::complete,       "completed"},
    {rem::cc::finish,         "finished"},
    {rem::cc::undefined,      "undefined"},
    {rem::cc::ready,          "ready"},
    {rem::cc::terminate,      "terminate"},
    {rem::cc::timeout,        "timeout"},
    {rem::cc::divbyzero,      "division by zero"},
    {rem::cc::notvisible,     "not visible"},
    {rem::cc::cancel,         "cancel"},
    {rem::cc::object_ptr,     "pointer to object"},
    {rem::cc::object_id,      "object id"},
    {rem::cc::unhandled,      "unhandled"},
    {rem::cc::notready,      "not ready"},
};


std::map<rem::action, const char*> actions_dictionary= {
    {rem::action::enter,   "enter"},
    {rem::action::leave,   "leave"},
    {rem::action::begin,   "begin"},
    {rem::action::end,     "end"},
    {rem::action::commit,  "commit"},
    {rem::action::cont,    "continue"},
    {rem::action::dismiss, "dismissed"},
};


std::map<rem::fn, const char*> functions_dictionary= {

    {rem::fn::endl,           "end line"},
    {rem::fn::func,            "function"},
    {rem::fn::file,           "file"},
    {rem::fn::line,           "line"},
    {rem::fn::stamp,          "stamp"},
    {rem::fn::hour,           "hour"},
    {rem::fn::minute,         "minute"},
    {rem::fn::seconds,        "seconds"},
    {rem::fn::weekday,        "weekday"},
    {rem::fn::month,          "month name"},
    {rem::fn::dayname,        "day name"},
    {rem::fn::day,            "day"},
    {rem::fn::monthnum,       "month number"},
    {rem::fn::year,           "year"}
};




std::string to_string(rem::type ty)
{
    return enums_type_dictionary[ty];
}

std::string to_string(rem::cc cod)
{
    return return_codes_dictionary[cod];
}



std::string to_string(rem::fn fn)
{
    return functions_dictionary[fn];
}

std::string to_string(rem::action ac)
{
    return actions_dictionary[ac];
}

std::map<rem::type, std::pair<tux::glyph::type, color::pair>> types_database={

    {rem::type::none,        {tux::glyph::computer,  {color::aquamarine1         , color::reset }}},
    {rem::type::err,         {tux::glyph::err1,      {color::red4                , color::reset }}},
    {rem::type::warning,     {tux::glyph::warning,   {color::yellow              , color::reset }}},
    {rem::type::fatal,       {tux::glyph::dead_head, {color::hotpink4            , color::reset }}},
    {rem::type::except,      {tux::glyph::bolt,      {color::skyblue3            , color::reset }}},
    {rem::type::message,     {tux::glyph::comment,   {color::green4              , color::reset }}},
    {rem::type::output,      {tux::glyph::notepad,   {color::grey39              , color::reset }}},
    {rem::type::debug,       {tux::glyph::bug,       {color::pink3               , color::reset }}},
    {rem::type::info,        {tux::glyph::info,      {color::lightskyblue4       , color::reset }}},
    {rem::type::comment,     {tux::glyph::comment,   {color::grey42              , color::reset }}},
    {rem::type::syntax,      {tux::glyph::handwrite, {color::lighcoreateblue     , color::reset }}},
    {rem::type::status,      {tux::glyph::admin,     {color::lightcyan3          , color::reset }}},
    {rem::type::test,        {tux::glyph::file,      {color::aquamarine3         , color::reset }}},
    {rem::type::interrupted, {tux::glyph::circle_x,  {color::khaki3              , color::reset }}},
    {rem::type::aborted,     {tux::glyph::stop,      {color::red4                , color::reset }}},
    {rem::type::segfault,    {tux::glyph::bomb,      {color::sandybrown          , color::reset }}},
    {rem::type::book,        {tux::glyph::book,      {color::orangered1          , color::reset }}},
    {rem::type::normal,      {tux::glyph::school,    {color::orangered1          , color::reset }}}
    //...
};


std::map<rem::action      , std::pair<tux::glyph::type, color::pair>> actions_color_db = {
    {rem::action::enter   , {tux::glyph::enter        , {color::white               , color::reset}}},
    {rem::action::leave   , {tux::glyph::file         , {color::white               , color::reset}}},
    {rem::action::begin   , {tux::glyph::chronos      , {color::white               , color::reset}}},
    {rem::action::end     , {tux::glyph::dead_head    , {color::white               , color::reset}}},
    {rem::action::commit  , {tux::glyph::pencil_dr    , {color::white               , color::reset}}},
    {rem::action::cont    , {tux::glyph::success      , {color::white               , color::reset}}},
    {rem::action::dismiss , {tux::glyph::arrow_dhead_right , {color::yellow         , color::reset}}},
};

std::map<rem::cc, std::pair<tux::glyph::type, color::pair>> codes_database={
    {rem::cc::ok,              {tux::glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::cc::accepted,        {tux::glyph::thumb_up,        {color::lime              ,color::reset }}},
    {rem::cc::success,         {tux::glyph::success,         {color::darkgreen         ,color::reset }}},
    {rem::cc::rejected,        {tux::glyph::thumb_down,      {color::hotpink4          ,color::reset }}},
    {rem::cc::failed,          {tux::glyph::poop,            {color::darkgoldenrod     ,color::reset }}},
    {rem::cc::empty,           {tux::glyph::arrowright,      {color::lighcoreategrey   ,color::reset }}},
    {rem::cc::full,            {tux::glyph::small_dot,       {color::white             ,color::reset }}},
    {rem::cc::notempty,        {tux::glyph::big_dot,         {color::white             ,color::reset }}},
    {rem::cc::implemented,     {tux::glyph::arrow_head_right,{color::lime              ,color::reset }}},
    {rem::cc::notimplemented,  {tux::glyph::err1,            {color::orange3           ,color::reset }}},
    {rem::cc::untested,        {tux::glyph::flag,            {color::yellow            ,color::reset }}},
    {rem::cc::eof,             {tux::glyph::baseline,        {color::white             ,color::reset }}},
    {rem::cc::eos,             {tux::glyph::baseline,        {color::white             ,color::reset }}},
    {rem::cc::null_ptr,        {tux::glyph::circle_x,        {color::hotpink4          ,color::reset }}},
    {rem::cc::notexist  ,      {tux::glyph::circle_x ,       {color::white             ,color::reset }}},
    {rem::cc::exist     ,      {tux::glyph::star5 ,          {color::white             ,color::reset }}},
    {rem::cc::unexpected,      {tux::glyph::flag ,           {color::yellow            ,color::reset }}},
    {rem::cc::expected  ,      {tux::glyph::cat ,            {color::white             ,color::reset }}},
    {rem::cc::blocked   ,      {tux::glyph::unauth ,         {color::indianred3        ,color::reset }}},
    {rem::cc::locked    ,      {tux::glyph::err3 ,           {color::white             ,color::reset }}},
    {rem::cc::overflow  ,      {tux::glyph::dead_head,       {color::red4              ,color::reset }}},
    {rem::cc::oob       ,      {tux::glyph::alien,           {color::lightcoral        ,color::reset }}},
    {rem::cc::reimplement,     {tux::glyph::book,             {color::yellow            ,color::reset }}},
    {rem::cc::done       ,     {tux::glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::cc::complete   ,     {tux::glyph::ok,              {color::yellow            ,color::reset }}},
    {rem::cc::finish     ,     {tux::glyph::ok,              {color::lime              ,color::reset }}},
    {rem::cc::undefined  ,     {tux::glyph::err3,            {color::red               ,color::reset }}},
    {rem::cc::ready      ,     {tux::glyph::ok,              {color::lime              ,color::reset }}},
    {rem::cc::terminate  ,     {tux::glyph::flag,            {color::hotpink4          ,color::reset }}},
    {rem::cc::timeout    ,     {tux::glyph::chronos,         {color::lime              ,color::reset }}},
    {rem::cc::divbyzero  ,     {tux::glyph::circle_x,        {color::red4              ,color::reset }}},
    {rem::cc::notvisible ,     {tux::glyph::circle_x,        {color::yellow            ,color::reset }}},
    {rem::cc::cancel     ,     {tux::glyph::circle_x,        {color::red4              ,color::reset }}},
    {rem::cc::object_ptr ,     {tux::glyph::edit_pencil_br,{color::lightsteelblue3     ,color::reset }}},
    {rem::cc::object_id ,      {tux::glyph::arrowright,      {color::yellow            ,color::reset }}},
    {rem::cc::unhandled ,      {tux::glyph::alien,           {color::yellow            ,color::reset }}},
    {rem::cc::notready ,       {tux::glyph::circle_x,           {color::orange5            ,color::reset }}},
};


std::map<rem::fn, std::pair<tux::glyph::type, color::pair>> functions_database={

    {rem::fn::stamp     ,      {tux::glyph::chronos  ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::func      ,      {tux::glyph::function ,   {color::skyblue3          ,color::reset }}},
    {rem::fn::file      ,      {tux::glyph::file     ,   {color::lightcyan3        ,color::reset }}},
    {rem::fn::line      ,      {tux::glyph::baseline ,   {color::lime              ,color::reset }}},
    {rem::fn::stamp     ,      {tux::glyph::chronos  ,   {color::yellow            ,color::reset }}},
    {rem::fn::hour      ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::minute    ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::seconds   ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::weekday   ,      {tux::glyph::fabric   ,   {color::yellow            ,color::reset }}},
    {rem::fn::month     ,      {tux::glyph::chronos  ,   {color::white             ,color::reset }}},
    {rem::fn::dayname   ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::day       ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::monthnum  ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}},
    {rem::fn::year      ,      {tux::glyph::star5    ,   {color::white             ,color::reset }}}

};





std::pair<tux::glyph::type, color::pair> type_attributes(rem::type ty)
{
    return rem::types_database[ty];
}

std::pair<tux::glyph::type, color::pair> return_code_attributes(rem::cc cod)
{
    return rem::codes_database[cod];
}

std::pair<tux::glyph::type, color::pair> function_attributes(rem::fn fn)
{
    return rem::functions_database[fn];
}

std::pair<tux::glyph::type, color::pair> action_attributes(rem::action a)
{
    return rem::actions_color_db[a];
}


}
