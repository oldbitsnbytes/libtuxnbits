#include <tuxnbits++/bits/diagnostic.h>

using namespace tux;

//////////////////////////////////////////////
/// \brief diagnostic::files
/// \note The static-global diagnostic files array systematically init default 0'th index (STDOUT_FILE[fd #1]) with std::cout.
diagnostic::file::list diagnostic::files
{
    {"std::out", &std::cout}
};
diagnostic::file::handle diagnostic::_default_file_handle=0; ///< std::cout by default.
//////////////////////////////////////////////



////////////////////////////////////////////////
/// \brief diagnostic::file::close
///        closes the opened file by this file struct is the descriptor is not stdout (fd:#1)
/// \return accepted or rejected
///
rem::cc diagnostic::file::close()
{
    if(fileptr){
        if(fileptr != &std::cout){
            delete fileptr;
            fileptr = nullptr;
            return rem::cc::accepted;
        }
    }
    return rem::cc::rejected;
}

////////////////////////////////////////////////////
/// \brief diagnostic::out constructor
/// \param out_ can be nullptr if it is required to use the default std::cout stream.
/// \param message diagnostic output message type
/// \param src implicit / automatic source location.
///
diagnostic::out::out(std::ostream* out_, rem::type message, std::source_location&& src):
    ofs(out_),
    type(message),
    code(rem::cc::ok),
    location(src){
    init_header();
}


////////////////////////////////////////////////////
/// \brief diagnostic::out constructor specific for the header components
/// \param out_ can be nullptr if it is required to use the default std::cout stream.
/// \param message diagnostic output message type
/// \param src implicit / automatic source location.
///
diagnostic::out::out(std::ostream *file_ptr, rem::type message, header_component hc, std::source_location &&src):
    ofs(file_ptr),
    type(message),
    _headercomp_(hc),
    code(rem::cc::ok),
    location(src)
{
    if(!hc.type) {
        hc.type = 1; hc.col  = 1; hc.frame = 1; hc.file = 1; hc.stamp = 1; hc.fun = 0;
        _headercomp_ = hc;
    }

    init_header();
}

diagnostic::out::~out()
{
    header.clear();
    text.clear();
}

#pragma region diagnostic_out

//////////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::init_header
///        internal call from the constructor.
///
void diagnostic::out::init_header()
{

    tux::string dash;
    tux::string hline;
    hline << color::r;
    if(_headercomp_.frame){
        if(_headercomp_.hline){
            for(int x=0; x < diagnostic::files[0].page_width ; x++)
            {
                hline << tux::cadre()[tux::cadre::Horizontal];
                dash << "-";
            }
            (*ofs) << hline() << std::endl;
        }
    }

    if(_headercomp_.stamp){
        auto txt{tux::string::now("%H:%M:%S")};
        auto [ic, a] = rem::function_attributes(rem::fn::stamp);
        tux::string acc;
        acc << a.fg << glyph::data[ic]  << txt;
        header << acc() << color::r << " ";

    }
    if(_headercomp_.type){
        auto [gh,colors] = rem::type_attributes(type);
        header << colors << gh  << ' ' << rem::to_string(type) << color::reset << ' ';
    }

    if(_headercomp_.file){
        auto [gh, colors] = rem::function_attributes(rem::fn::file);
        tux::string txt = location.file_name();
        tux::string::word::list words;
        if(auto count = txt.words(words, false, "/"); !count) return ;
        header << color::reset << " in " << colors << gh;
        if(words.size() > 1) {
            auto i = words.end();
            --i; // filename
            auto filename = *i;
            --i; // parent dir
            header << **i;
            ++i;
            header << '/' << **i << color::r << ' ';
        }
        words.clear();
    }
    // disable uncontrollable and potentially insane length of function signature given by std::source_location.
    // instead, using it explicitly into a distinct book input
    //------------------------------------------------------------------------------------------------------------
    //    if(headercomp.fun)
    //    {
    //        auto [gh, colors] = diagnostic::functionattributes(rem::fn::fun);
    //        header << colors << gh << ' ' << location.function_name() << color::reset;
    //    }
    //------------------------------------------------------------------------------------------------------------
    if(_headercomp_.line){
        auto [gh, colors] = rem::function_attributes(rem::fn::line);
        header << colors << "line: " << std::format("{}", location.line()) << color::reset << ' ';

        (*ofs) << header() << std::endl;
    }


    header = "";
    (*ofs) << dash() << std::endl;
    if(_headercomp_.fun && type != rem::type::test){
        auto [gh, colors] = rem::function_attributes(rem::fn::func);
        header << colors << gh << ' ' << location.function_name()<< color::r;

        (*ofs) << header() << std::endl;
        header = "";
    }
    (*ofs) << dash() << std::endl;
}

/////////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param txt
/// \return
///
diagnostic::out &diagnostic::out::operator <<(const std::string &txt)
{
    (*ofs) << txt;
    return *this;
}



diagnostic::out& diagnostic::out::operator << (glyph::type f)
{
    (*ofs) << glyph::data[f];
    return *this;
}

/////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param cstr
/// \return
///
diagnostic::out& diagnostic::out::operator << (const char* cstr)
{
    ofs->write(cstr,std::strlen(cstr));
    return *this;
}


////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param cstr
/// \return
///
diagnostic::out& diagnostic::out::operator << (std::string_view cstr)
{
    ofs->write(cstr.data(),cstr.length());
    return *this;
}

////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param obstr
/// \return
///
diagnostic::out& diagnostic::out::operator << (tux::string obstr)
{
    ofs->write(obstr().c_str(),obstr().length());
    return *this;
}

//////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param c
/// \return
///
diagnostic::out& diagnostic::out::operator << (char c)
{
    ofs->write(&c,1);
    return *this;
}


////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
///     Render the given color enum (color::code) in ansi encoded out.
/// \param clr
/// \return
///
diagnostic::out& diagnostic::out::operator << (ui::color::code clr)
{
    (*ofs) << ui::color::render(clr);
    return *this;
}


/////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
/// \param clr
/// \return
///
diagnostic::out& diagnostic::out::operator << (ui::color::pair clr)
{
    (*ofs) << ui::color::render(clr);
    return *this;
}


//////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
///         Transdlates to ansi encoded string the given rem::cc enum value
/// \param c
/// \return self& for chaining call
///
diagnostic::out& diagnostic::out::operator << ( rem::cc c)
{
    auto [ic,col] = rem::return_code_attributes(c);
    tux::string str;
    str << col << ic << rem::to_string(c);
    (*ofs) <<str();
    return *this;
}

////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
///         Transdlates to ansi encoded string the given rem::type enum value
/// \param ty
/// \return self& for chaining call
///
diagnostic::out& diagnostic::out::operator << ( rem::type ty)
{
    auto [ic,col] = rem::type_attributes(ty);
    tux::string str;
    str << col << ic << rem::to_string(ty);
    (*ofs) << str();
    return *this;
}

diagnostic::out& diagnostic::out::operator << (rem::fn f)
{
    tux::string str;

    switch (f) {
    case rem::fn::endl:
        (*ofs) << '\n';
        //            switch (appbook::format()) ///@todo create diagnostic::format(); directly instead.
        //            {
        //                case color::format::ansi256:
        //                    input("\n");
        //                    break;
        //                case color::format::html:
        //                    input("<br />");
        //                    break;
        //            }
        return *this;
    case rem::fn::stamp: {
        /*
                 * %d %f ou %d %m %y
                 * %r %t %h %m %s
                 * %a {monday} %a {mon}
                 * %b {june}   %b {jun}
            */


        //std::chrono::zoned_time date{"america/toronto", std::chrono::system_clock::now()};
        //const auto tp{std::chrono::system_clock::now()};
        //auto txt{tux::string::now("{:%h:%m:%s}", tp)};
        auto [ic, a] = rem::function_attributes(rem::fn::stamp);

        str << a.fg << glyph::data[ic] <<color::reset << tux::string::now("%H:%M:%S");
        (*ofs) << str() << " ";
        return *this;
    }

    case rem::fn::file:
        (*ofs) << location.file_name();
        return *this;
    case rem::fn::weekday: {
        auto [ic, a] = rem::function_attributes(rem::fn::weekday);
        //auto today{std::chrono::system_clock::now()};
        str << a.fg << tux::string::now("%A");
        (*ofs) << str();
        return *this;
    }

    case rem::fn::day : {
        auto [ic, a] = rem::function_attributes(rem::fn::day);
        //auto today{std::chrono::system_clock::now()};
        str << a.fg << tux::string::now("%d");
        (*ofs) << str();
        return *this;
    }

    case rem::fn::month: {
        auto [ic, a] = rem::function_attributes(rem::fn::month);
        //auto today{std::chrono::system_clock::now()};
        str << a.fg << tux::string::now("%M");
        (*ofs) << str();
        return *this;
    }
    case rem::fn::monthnum: {
        auto [ic, a] = rem::function_attributes(rem::fn::month);
        //auto today{std::chrono::system_clock::now()};
        str << a.fg <<  tux::string::now("%b");
        (*ofs) << str();
        return *this;
    }
    case rem::fn::year: {
        auto [ic, a] = rem::function_attributes(rem::fn::year);
        //auto today{std::chrono::system_clock::now()};
        tux::string acc;
        acc << /*utf::glyph::data[ic] <<*/ a.fg << tux::string::now("%Y");
        (*ofs) <<acc();
        return *this;
    }
    case rem::fn::func:
        auto [gh, colors] = rem::function_attributes(rem::fn::func);
        str << colors << location.function_name() << color::reset << "\n";
        (*ofs) << str() << std::endl;
        break;

        //default: break;
    }
    return *this;
}


diagnostic::out& diagnostic::out::endl()
{
    *ofs << std::endl;
    return *this;
}




/////////////////////////////////////////////////////////////
/// \brief diagnostic::out::operator <<
///      Cheat Trick that outputs newline in the working file.
/// \return
///
diagnostic::out& diagnostic::out::operator << (out&)
{
    *ofs << std::endl;
    return *this;
}



#pragma endregion diagnostic_out

#pragma region exceptions.

///////////////////////////////////////////////////////////////////
/// \brief diagnostic::exception::operator []
///  std::exception based builder.
/// \param el
/// \return
///
diagnostic::exception diagnostic::exception::operator[](diagnostic::out el)
{
    _log_ = std::move(el);
    return *this;
}


const char* diagnostic::exception::what() const noexcept
{
    if(!_log_.text().empty()) _msg_ = _log_.text();
    return _msg_.c_str();
}


#pragma endregion exceptions.

////////////////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::error
///     Create a disgnostic::out for output to the specified diagnostic::file.
/// \param hdnl index number of the files list.
/// \param src implicit
/// \return  diagnostic::out instance
///
diagnostic::out diagnostic::error(diagnostic::file::handle hndl, std::source_location &&src)
{
    return{diagnostic::files[hndl].fileptr, rem::type::err, std::move(src)};
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::statuc
/// \param src
/// \return
///
diagnostic::out diagnostic::status(file::handle h, std::source_location &&src)
{
    return{diagnostic::files[h].fileptr, rem::type::status, std::move(src)};
}




diagnostic::out diagnostic::warning     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::status, std::move(src)}; // NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::fatal       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::fatal, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::except      (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::except, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::message     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::message, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::write       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::output, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::debug       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::debug, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::info        (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::info, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::comment     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::comment, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::syntax      (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::syntax, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::test        (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::test, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::interrupted (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::interrupted, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::aborted     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::aborted, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::segfault    (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::segfault, std::move(src)};// NOLINT(*-move-const-arg)
}

diagnostic::out diagnostic::log         (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::book, std::move(src)};// NOLINT(*-move-const-arg)
}

rem::cc diagnostic::close(file::handle hindex)
{
    return diagnostic::files[hindex].close();
}

rem::cc diagnostic::close_all()
{
    for(auto f : diagnostic::files){
        f.close();
    }
    diagnostic::files.clear();
    return rem::cc::ok;
}




/////////////////////////////////////////////////////////////////////////////////////////
/// \brief diagnostic::open
///     Creates a new file for a subdiagnostic logger.
/// \param file_id
/// \return
///
std::optional<diagnostic::file::handle> diagnostic::new_file(const std::string &file_id)
{
    if(file_id.empty()){
        return {0};
    }

    tux::string filename;
    filename << file_id << ".dlog";
    auto * f = new std::ofstream(filename().c_str(),std::ios_base::trunc);
    if(!f->is_open()){
        delete f;
        return {};
    }
    diagnostic::files.emplace_back(file_id, f);
    return {diagnostic::files.size()-1};
}


rem::cc diagnostic::use_default(diagnostic::file::handle h)
{
    if (h < 0 || h >= diagnostic::files.size()) return rem::cc::rejected;
    diagnostic::_default_file_handle = h;
    return rem::cc::accepted;
}
