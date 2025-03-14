#pragma once


#include <optional>

#include <tuxnbits++/bits/glyphes.h>
#include <tuxnbits++/bits/string.h>

#include <source_location>

#include <functional>
#include <tuxnbits++/bits/geometry.h>
#include <filesystem>
#include <fstream>


using tux::ui::color;
using tux::glyph;
using tux::accent_fr;
using tux::ui::rectangle;
using tux::ui::cxy;
using tux::ui::size;






using namespace tux;



///////////////////////////////////////////////////////////////////////
/// \brief The diagnostic class
/// \note IMPORTANT! file outputs have to be managed in specific diagnostic::file instance instead of new individual diagnotic::out instance
class diagnostic
{
public:
    struct  out
    {
        std::ostream* ofs{&std::cout};
        struct header_component
        {
            integers::U8 type  : 1;
            integers::U8 stamp : 1;
            integers::U8 fun   : 1;
            integers::U8 file  : 1;
            integers::U8 line  : 1;
            integers::U8 col   : 1;
            integers::U8 hline : 1;
            integers::U8 vline : 1;
            integers::U8 frame : 1; ///< 1 = enable vline|hline|corners and joints.
            //...
        };
        rem::type type{};
        rem::cc code{};
        std::source_location location{};
        // ---------------------------------
        tux::string  text{};
        tux::string  header{};

        header_component _headercomp_{1,1,1,1,1,1,1,1,1};
        out() = default;
    private:
        out(std::ostream* file_ptr, rem::type message, std::source_location&& src={});
        out(std::ostream* file_ptr, rem::type message, diagnostic::out::header_component hc, std::source_location&& src={});
        friend class diagnostic;
    public:
        ~out();

        void init_header();
        out& write();
        out& endl();
        out& operator << (out&);
        out& operator << (const std::string&);
        out& operator << (const char*);
        out& operator << (std::string_view);
        out& operator << (tux::string tuxstr);
        out& operator << (char c);
        out& operator << (ui::color::code clr);
        out& operator << (ui::color::pair clr);
        out& operator << (rem::cc c);
        out& operator << (rem::type ty);
        out& operator << (rem::fn f);
        out& operator << (glyph::type f);
        out& operator << (ui::cxy xy);
        out& operator << (ui::size z);
        out& operator << (ui::rectangle r);
        out& operator << (ui::string2d s2d);

        template<typename T> out& operator << (const T& v);
    };


    struct file{
        using handle = int; ///< straight vector index.
        std::string name{};
        std::ostream *fileptr{nullptr}; ///< Either a file or the stdout file stream ( std::cout ).
        int page_width{80}; ///< For the frame line.
        using list = std::vector<diagnostic::file>;
        rem::cc close();

        void init_header();
        file& write();
        file& endl();
        file& operator << (out&);
        file& operator << (const std::string&);
        file& operator << (const char*);
        file& operator << (std::string_view);
        file& operator << (tux::string tuxstr);
        file& operator << (char c);
        file& operator << (ui::color::code clr);
        file& operator << (ui::color::pair clr);
        file& operator << (rem::cc c);
        file& operator << (rem::type ty);
        file& operator << (rem::fn f);
        file& operator << (glyph::type f);
        file& operator << (ui::cxy xy);
        file& operator << (ui::size z);
        file& operator << (ui::rectangle r);
        file& operator << (ui::string2d s2d);

        template<typename T> file& operator << (const T& v);


    };
    // diagnostic::push(current diagnostic::file is 'A');
    //...auto & out = diagnostic::select(diagnostic::handle B);
    // ...
    // diagnostic::error() << " error text..." << out; // .. << out; = new line and flush to out file...
    // diangostic::pop(previous diagnostic::file is now back to 'A');

    diagnostic()=delete;
    ~diagnostic() = default;


    static diagnostic::file& error       (std::source_location&& src=std::source_location::current());
    static diagnostic::file& status      (std::source_location&& src=std::source_location::current());
    static diagnostic::file& warning     (std::source_location&& src = std::source_location::current());
    static diagnostic::file& fatal       (std::source_location&& src = std::source_location::current());
    static diagnostic::file& except      (std::source_location&& src = std::source_location::current());
    static diagnostic::file& message     (std::source_location&& src = std::source_location::current());
    static diagnostic::file& write       (std::source_location&& src = std::source_location::current());
    static diagnostic::file& debug       (std::source_location&& src = std::source_location::current());
    static diagnostic::file& info        (std::source_location&& src = std::source_location::current());
    static diagnostic::file& comment     (std::source_location&& src = std::source_location::current());
    static diagnostic::file& syntax      (std::source_location&& src = std::source_location::current());
    static diagnostic::file& t1est        (std::source_location&& src = std::source_location::current());
    static diagnostic::file& interrupted (std::source_location&& src = std::source_location::current());
    static diagnostic::file& aborted     (std::source_location&& src = std::source_location::current());
    static diagnostic::file& segfault    (std::source_location&& src = std::source_location::current());
    static diagnostic::file& log         (std::source_location&& src = std::source_location::current());
    //...
    static rem::cc close(diagnostic::file::handle hindex=-1);
    static rem::cc close_all();
    static std::optional<diagnostic::file::handle> new_file(const std::string& file_name);
    static rem::cc use_default(diagnostic::file::handle h=-1);
    static diagnostic::file::handle _default_file_handle; ///< std::cout by default.
    static diagnostic::file& get(diagnostic::file::handle h);
    class exception :  public std::exception
    {
    public:

        explicit exception():std::exception() { _e_ = true; }
        explicit exception(const char *txt) { _msg_ = txt; _e_ = false; }
        explicit exception(const std::string &txt) { _msg_ = txt; _e_ = false; }

        ~exception() noexcept override {
            _msg_.clear();
        }

        const char* what() const noexcept override;

        diagnostic::exception operator[](diagnostic::out el);
    private:
        bool _e_;
        mutable std::string _msg_;
        diagnostic::out _log_;
    };


    struct LIBTUXNBITS Test
    {
        std::string name{};
        template<typename T> rem::cc exec(const std::string& sub_test_name, std::function<std::pair<rem::cc, T>(diagnostic::Test&)> lambda );

        Test(const std::string& test_name):name(test_name){}
        ~Test() { name.clear(); }

    };

private:
    static file::list files;
};


#include "diagnostic.inc"
