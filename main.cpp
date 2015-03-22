

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

    rephp::reflection::bundle::dump(std::cout, be_standard) << std::endl;

    auto a = engine::invoke("Integer", 8);
    auto b = engine::invoke("Float", 2.);
    auto c = engine::invoke("String", type::string_t("Hello!"));
    auto d = engine::invoke("String", type::string_t("Hello World!"));

    rephp::reflection::internal_value::dump(std::cout, a) << std::endl;
    rephp::reflection::internal_value::dump(std::cout, b) << std::endl;

    rephp::reflection::internal_value::dump(std::cout, (a + a)) << std::endl;
    rephp::reflection::internal_value::dump(std::cout, (a + b)) << std::endl;
    rephp::reflection::internal_value::dump(std::cout, (b + b)) << std::endl;
    rephp::reflection::internal_value::dump(std::cout, (b + a)) << std::endl;

    //std::cout << a->call('toString') << std::endl;
    //std::cout << (a->call(rephp::engine::OP_ADD, {b}));
    std::cout << std::endl;
}