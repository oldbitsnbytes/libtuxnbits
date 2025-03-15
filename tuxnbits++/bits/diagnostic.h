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






using namespace tux::integers;
using namespace tux::ui;


///////////////////////////////////////////////////////////////////////
/// \brief The diagnostic class
/// \note IMPORTANT! file outputs have to be managed in specific diagnostic::file instance instead of new individual diagnotic::out instance
class diagnostic
{
public:

    static constexpr int indent_width=4;

    struct  out
    {
        std::ostream* ofs{&std::cout};
        struct header_component
        {
            U8 type  : 1;
            U8 stamp : 1;
            U8 fun   : 1;
            U8 file  : 1;
            U8 line  : 1;
            U8 col   : 1;
            U8 hline : 1;
            U8 vline : 1;
            U8 frame : 1; ///< 1 = enable vline|hline|corners and joints.
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
        out& operator << (color::code clr);
        out& operator << (color::pair clr);
        out& operator << (rem::cc c);
        out& operator << (rem::type ty);
        out& operator << (rem::fn f);
        out& operator << (glyph::type f);
        out& operator << (cxy xy);
        out& operator << (size z);
        out& operator << (rectangle r);
        out& operator << (string2d s2d);

        template<typename T> out& operator << (const T& v);
    };


    struct file{
        using handle = int; ///< straight vector index.
        std::string name{}; ///< filename
        std::ostream *fileptr{nullptr}; ///< Either a file or the stdout file stream ( std::cout ).
        int page_width{80}; ///< For the frame line.
        using list = std::vector<diagnostic::file>; ///< storage location of the  of diagnostic::file list.
        rem::cc close();

        int inden{0};

        // void init_header();
        // file& write();
        // file& endl();
        // file& operator << (out&);
        // file& operator << (const std::string&);
        // file& operator << (const char*);
        // file& operator << (std::string_view);
        // file& operator << (tux::string tuxstr);
        // file& operator << (char c);
        // file& operator << (color::code clr);
        // file& operator << (color::pair clr);
        // file& operator << (rem::cc c);
        // file& operator << (rem::type ty);
        // file& operator << (rem::fn f);
        // file& operator << (glyph::type f);
        // file& operator << (cxy xy);
        // file& operator << (size z);
        // file& operator << (rectangle r);
        // file& operator << (string2d s2d);

        // template<typename T> file& operator << (const T& v);


    };
    // diagnostic::push(current diagnostic::file is 'A');
    //...auto & out = diagnostic::select(diagnostic::handle B);
    // ...
    // diagnostic::error() << " error text..." << out; // .. << out; = new line and flush to out file...
    // diangostic::pop(previous diagnostic::file is now back to 'A');

    diagnostic()=delete;
    ~diagnostic() = default;


    static diagnostic::out error       (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out status      (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out warning     (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out fatal       (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out except      (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out message     (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out write       (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out debug       (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out info        (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out comment     (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out syntax      (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out test        (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out interrupted (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out aborted     (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out segfault    (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
    static diagnostic::out log         (diagnostic::file::handle h=0,std::source_location&& src = std::source_location::current());
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
