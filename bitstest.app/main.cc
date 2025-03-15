#include <tuxnbits++/bits/appbits.h>
#include <tuxnbits++/io/terminal.h>

using dlog = diagnostic;
namespace tux
{


class test : public appbits
{
    io::terminal term{nullptr,"tests",{}};
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
    auto l = dlog::message(h) << color::r << " - The Great Beginning of the tux::bitsnbytes++ !\n";
    l << color::yellow << "ready" << l;

    // dlog::Test Test("[diagnostic::Test]");
    // auto r = Test.exec<std::string>("auto", [&](diagnostic::Test& Tst)->auto {
    //     return std::make_pair<rem::cc, std::string>(rem::cc::success,"allo");
    // });

    //l << rem::fn::month << rem::fn::weekday << color::r << " : Test result: " << r << l;
    l = diagnostic::info(h) << " Starting the terminal input events loop"  << l;
    rem::action ac{rem::action::cont};
    do{
        auto r = term.poll_in();
        if(!r){
            terminate(rem::type::aborted);
            return rem::cc::terminate;
        }

        while(!term.events().empty()){
            auto evc = std::move(term.events().front());
            term.events().pop_front();
            if(evc.is<io::kbhit>()){
                if(evc.data.kev.mnemonic == io::kbhit::ESCAPE){
                    l << "ESCAPE KEY hit - Terminating!." << l;
                    term.events().clear();
                    ac = rem::action::end;
                    break;//return rem::cc::terminate;
                }
                else{
                    l << "CHARACTER or command: '" << evc.data.kev.name << "' | (char)'" << (char)evc.data.kev.code << '\'' << l;
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
    return rem::cc::ok;
}



test::test(const std::string &tname, string::view_list &&vargs):appbits(tname, std::move(vargs), nullptr){}


rem::cc test::terminate(rem::type reason)
{
    appbits::terminate(reason);
    term.end();
    dlog::close_all();
    return rem::cc::ok;
}


}



auto main(int argc, char** argv, char** env) -> int
{
    std::cout << "Hello, World\n";
    tux::test tests("obb tests", tux::string::string_view_list(argc,argv,1));
    return static_cast<int>(tests.run());
}
