#include <tuxnbits++/bits/diagnostic.h>

using namespace tux;

//////////////////////////////////////////////
/// \brief diagnostic::files
/// \note The static-global diagnostic files array systematically init default 0'th index (STDOUT_FILE[fd #1]) with std::cout.
diagnostic::file::list diagnostic::files
{
    {"std::out", &std::cout}
};
//////////////////////////////////////////////

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
/// \brief diagnostic::out::out constructor
/// \param out_ can be nullptr if it is required to use the default std::cout stream.
/// \param message diagnostic output message type
/// \param src implicit / automatic source location.
///
diagnostic::out::out(std::ostream* out_, rem::type message, std::source_location&& src):
    ofs(out_),
    type(message),
    code(rem::cc::ok),
    location(std::move(src))
{
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
    int width = 0;
    if(_headercomp_.stamp){
        auto txt{tux::string::now("%H:%M:%S")};
        auto [ic, a] = rem::function_attributes(rem::fn::stamp);
        tux::string acc;
        width = txt.length() + std::strlen(glyph::data[ic]);
        acc << a.fg << glyph::data[ic]  << txt;

        header << acc() << " ";
        width ++;
    }
    if(_headercomp_.type){
        auto [gh,colors] = rem::type_attributes(type);
        header << colors << gh  << ' ' << rem::to_string(type) << color::reset << ' ';
        width += rem::to_string(type).length() + std::strlen(glyph::data[gh]);
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
            header << '/' << **i << ' ';
            width += (**i).length();
        }
        words.clear();
    }
    // disable uncontrollable and potentially insane length of function signature given by std::source_location.
    // instead, using it explicitly into a distinct book input
    //------------------------------------------------------------------------------------------------------------
    //    if(headercomp.fun)
    //    {
    //        auto [gh, colors] = book::functionattributes(rem::fn::fun);
    //        header << colors << gh << ' ' << location.function_name() << color::reset;
    //    }
    //------------------------------------------------------------------------------------------------------------
    if(_headercomp_.line){
        auto [gh, colors] = rem::function_attributes(rem::fn::line);
        header << colors << "line: " << std::format("{}", location.line());
        width += std::format("{}", location.line()).length() + 3;
    }
    if(_headercomp_.fun){
        auto [gh, colors] = rem::function_attributes(rem::fn::func);
        header << colors << gh << ' ' << location.function_name();
        width += std::strlen(location.function_name())+3;
    }
    if(ofs){
        (*ofs) << header();
        //(*fout) << tux::cadre()[tux::cadre::Horizontal]
    }
    else
        std::cout << header;
    header = "";
    header << color::white;
    if(_headercomp_.frame){
        if(_headercomp_.hline){
            for(int x=0; x < width + 13 ; x++)// for(int x=0; x < diagnostic::files[0].page_width; x++)
                header << tux::cadre()[tux::cadre::Horizontal];
            (*ofs) << std::endl << header();
        }
    }
    (*ofs) << std::endl;
}

diagnostic::out &diagnostic::out::operator <<(const std::string &txt)
{
    (*ofs) << txt;
    return *this;
}

diagnostic::out& diagnostic::out::operator << (const char* cstr)
{
    ofs->write(cstr,std::strlen(cstr));
    return *this;
}
diagnostic::out& diagnostic::out::operator << (std::string_view cstr)
{
    ofs->write(cstr.data(),cstr.length());
    return *this;
}
diagnostic::out& diagnostic::out::operator << (tux::string obstr)
{
    ofs->write(obstr().c_str(),obstr().length());
    return *this;
}
diagnostic::out& diagnostic::out::operator << (char c)
{
    ofs->write(&c,1);
    return *this;
}

diagnostic::out& diagnostic::out::operator << (ui::color::code clr)
{
    (*ofs) << ui::color::render(clr);
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
    return {diagnostic::files[h].fileptr,rem::type::status, std::move(src)};
}

diagnostic::out diagnostic::fatal       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::fatal, std::move(src)};
}

diagnostic::out diagnostic::except      (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::except, std::move(src)};
}

diagnostic::out diagnostic::message     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::message, std::move(src)};
}

diagnostic::out diagnostic::write       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::output, std::move(src)};
}

diagnostic::out diagnostic::debug       (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::debug, std::move(src)};
}

diagnostic::out diagnostic::info        (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::info, std::move(src)};
}

diagnostic::out diagnostic::comment     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::comment, std::move(src)};
}

diagnostic::out diagnostic::syntax      (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::syntax, std::move(src)};
}

diagnostic::out diagnostic::test        (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::test, std::move(src)};
}

diagnostic::out diagnostic::interrupted (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::interrupted, std::move(src)};
}

diagnostic::out diagnostic::aborted     (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::aborted, std::move(src)};
}

diagnostic::out diagnostic::segfault    (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::segfault, std::move(src)};
}

diagnostic::out diagnostic::jnl         (diagnostic::file::handle h, std::source_location&& src){
    return {diagnostic::files[h].fileptr,rem::type::book, std::move(src)};
}


rem::cc diagnostic::close(file::handle hindex)
{
    return diagnostic::files[hindex].close();
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
    filename << file_id << ".diagnostic";
    auto * f = new std::ofstream(filename().c_str(),std::ios_base::trunc);
    if(!f->is_open()){
        delete f;
        return {};
    }
    diagnostic::files.emplace_back(file_id, f);
    return {diagnostic::files.size()-1};
}


