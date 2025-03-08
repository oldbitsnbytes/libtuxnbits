#include <tuxnbits++/bits/appbits.h>

using dlog = diagnostic;
namespace tux
{


class test : public appbits
{
    //io::terminal term{nullptr,"tests",{}};
    tux::string::view_list _args{};
public:
    test(const std::string& tname, tux::string::view_list&& vargs);
    ~test(){ _args.clear(); }

    void terminate();
    //est::alu expr();
    dlog::file::handle h{};
    rem::cc run();

};




rem::cc test::run()
{
    h = *dlog::new_file("tests");
    auto l = dlog::message(h) << color::r << " - The Great Beginning of the tux::bitsnbytes++ !\n";
    l << color::yellow << "More to come!" << l;
    dlog::Test Testest("Testing the diangostic::Test object...");
    auto r = Testest.exec<std::string>("auto", [&](diagnostic::Test& Tst)->auto {
        return std::make_pair<rem::cc, std::string>(rem::cc::success,"allo");
    });
    l << rem::fn::weekday << color::r << " : Test result: " << r << l;
    l = dlog::status(h) << color::lime << " - fin";
    l << l;
    terminate();
    return rem::cc::ok;
}



test::test(const std::string &tname, string::view_list &&vargs): _args(std::move(vargs)){}


void test::terminate()
{
    dlog::close(h);
}


}



auto main(int argc, char** argv, char** env) -> int
{
    std::cout << "Hello, World\n";
    tux::test tests("obb tests", tux::string::string_view_list(argc,argv,1));
    return static_cast<int>(tests.run());
}
