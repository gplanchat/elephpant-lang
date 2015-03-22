

#include <memory>
#include <iostream>

#include "engine/engine.h"
#include "engine/reflection.h"
#include "lang/standard.h"


int main()
{
    using namespace rephp::engine;

    auto be_standard = type::bundle_t(new rephp::lang::standard_bundle("core"));

    engine::register_bundle(be_standard);

    //rephp::reflection::bundle::dump(std::cout, be_standard);

    auto a = be_standard->invoke("Integer", 8);
    auto b = be_standard->invoke("Float", 2.);
    auto c = be_standard->invoke("String", type::string_t("Hello!"));
    auto d = engine::invoke("String", type::string_t("Hello World!"));

    //rephp::reflection::internal_value::dump(std::cout, a);
    //rephp::reflection::internal_value::dump(std::cout, b);
    //rephp::reflection::internal_value::dump(std::cout, c);
    //rephp::reflection::internal_value::dump(std::cout, d) << std::endl;

    rephp::reflection::internal_value::dump(std::cout, (a + a)) << std::endl;
    rephp::reflection::internal_value::dump(std::cout, (a + b)) << std::endl;
//    rephp::reflection::internal_value::dump(std::cout, a->call(rephp::engine::OP_ADD, {b})) << std::endl;
//    rephp::reflection::internal_value::dump(std::cout, b->call(rephp::engine::OP_ADD, {b})) << std::endl;
//    rephp::reflection::internal_value::dump(std::cout, b->call(rephp::engine::OP_ADD, {a})) << std::endl;

    //std::cout << a->call('toString') << std::endl;
    //std::cout << (a->call(rephp::engine::OP_ADD, {b}));
    std::cout << std::endl;
}