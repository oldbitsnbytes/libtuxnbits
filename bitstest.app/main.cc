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
    l << color::yellow << "More to come!" << l;
    dlog::Test Test("[diagnostic::Test]");
    auto r = Test.exec<std::string>("auto", [&](diagnostic::Test& Tst)->auto {
        return std::make_pair<rem::cc, std::string>(rem::cc::success,"allo");
    });
    l << rem::fn::weekday << color::r << " : Test result: " << r << l;
    l = dlog::status(h) << color::lime << " - 🦀 fin";
    l << l;
    return terminate(rem::type::status);
}

rem::cc test::setup()
{
    appbits::setup();
    auto log = diagnostic::status(_dlog);
    log << " There's nothing to do as of now [" << rem::fn::weekday << ", " << rem::fn::monthnum << '/' << rem::fn::day << '/'  << rem::fn::year << ']' << log;
    return rem::cc::ok;
}



test::test(const std::string &tname, string::view_list &&vargs):appbits(tname, std::move(vargs), nullptr),
    _args(std::move(vargs))
{

}


rem::cc test::terminate(rem::type reason)
{
    appbits::terminate(reason);
    dlog::close(h);
    return rem::cc::ok;
}


}



auto main(int argc, char** argv, char** env) -> int
{
    std::cout << "Hello, World\n";
    tux::test tests("obb tests", tux::string::string_view_list(argc,argv,1));
    return static_cast<int>(tests.run());
}
