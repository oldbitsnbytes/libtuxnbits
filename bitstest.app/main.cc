#include <tuxnbits++/bits/appbits.h>
#include <tuxnbits++/io/terminal.h>
#include <tuxnbits++/io/vchar.h>
#include <tuxnbits++/bits/databits/databits.h>
#include <tuxnbits++/bits/databits/field.h>

using dlog = diagnostic;
namespace tux
{


class test : public appbits
{
    io::terminal term{nullptr,"tests",{}};
    io::vchar::bloc *blk{nullptr};
    sql::databits db{"tuxnbits"};
public:
    test(const std::string& tname, tux::string::view_list&& vargs);
    ~test() override = default;

    rem::cc terminate(rem::type reason) override;
    //est::alu expr();
    dlog::file::handle h{};
    rem::cc run() override;
    rem::cc setup() override;

};




rem::cc test::run()
{
    (void)setup();
    h = *dlog::new_file("tests");
    auto l = dlog::message(h) << color::r << " - The Grand Beginning of the tux::bitsnbytes++ !\n";
    l << color::yellow << "ready" << l;
    //blk->home();
    blk->set_foreground_color(color::yellow);
    *blk << "----- Hello, world! -----";
    term.render(blk,{1,1});

    // dlog::Test Test("[diagnostic::Test]");
    // auto r = Test.exec<std::string>("auto", [&](diagnostic::Test& Tst)->auto {
    //     return std::make_pair<rem::cc, std::string>(rem::cc::success,"allo");
    // });

    //l << rem::fn::month << rem::fn::weekday << color::r << " : Test result: " << r << l;
    l = diagnostic::info(h) << " Starting the terminal input events loop"  << l;
    rem::action ac{rem::action::cont};
    do{
        auto r = term.poll_in();
        if(!r)
            return terminate(rem::type::aborted);

        while(!term.events().empty()){
            auto evc = std::move(term.events().front());
            term.events().pop_front();
            if(evc.is<io::kbhit>()){
                switch(evc.data.kev.mnemonic){
                    case io::kbhit::ESCAPE:
                        return terminate(rem::type::normal);
                    case io::kbhit::CHARACTER:{
                        if(evc.data.kev.code == 'c'){
                            static int col = color::black;
                            blk->clear({{2,3},ui::size{20,3}},{color::red,static_cast<color::code>(col)});
                            *blk << ui::cxy{6,4};
                            *blk << color::name(static_cast<color::code>(col++));
                            term.render(blk,{1,1});
                        }
                    }
                    default:
                        l << "CHARACTER:'" << color::yellow << (char)evc.data.kev.code << color::r << '\'' << l;
                        break;
                }
            }
            else if(evc.is<io::mouse>()){
                l << "mouse event: " << evc.data.mev() << l;
            }
        }
    }while(ac == rem::action::cont);

    l = dlog::status(h) << color::lime << " - " << glyph::rust_crab << " fin." << l;
    return terminate(rem::type::normal);
}

rem::cc test::setup()
{
    appbits::setup();
    auto log = diagnostic::status(_dlog);
    term.begin();
    term.init_stdinput();
    log << "terminal is ready" << log;
    blk = io::vchar::bloc::create({40,7}, {color::yellow, color::code::blueviolet});
    try{
        log << "testing databits:" << log;
        db.open();
        db.init_pragmas(true);
        db.close();
        log << "databits opened and closed gracefully..." << log;
        sql::field f = {"id"};
        f.index_bits = sql::field::PKEY|sql::field::AUTOINC;
        f.type = sql::field::integer_type;
        log = diagnostic::test(_dlog); log << " field::to_string: " << log;
        log << f() << log;
    }
    catch (std::runtime_error& e)
    {
        auto l = diagnostic::except(0); l << "runtime error: " << e.what() << l;
    }
    return rem::cc::ok;
}



test::test(const std::string &tname, string::view_list &&vargs):appbits(tname, std::move(vargs), nullptr){}


rem::cc test::terminate(rem::type reason)
{
    appbits::terminate(reason);
    term.end();
    dlog::close_all();
    if(blk) delete blk;
    return rem::cc::ok;
}


}



auto main(int argc, char** argv, char** env) -> int
{
    std::cout << "Hello, World\n";
    tux::test tests("obb tests", tux::string::string_view_list(argc,argv,1));
    return static_cast<int>(tests.run());
}
