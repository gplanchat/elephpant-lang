# ElePHPant lang

*ElePHPant* is an experimental language interpereter and virtual machine.
It is not aimed to be used in production, it is just for fun.

## Example

```php
#include <memory>
#include <iostream>

#include "engine/engine.h"
#include "engine/reflection.h"
#include "lang/standard.h"


int main()
{
    using namespace rephp::engine;

    auto b_standard = bundle_t(new rephp::lang::standard_bundle("core"));

    rephp::reflection::bundle::dump(std::cout, b_standard);

    auto a = b_standard->invoke("Integer", 8);
    auto b = b_standard->invoke("Float", 2.);
    auto c = b_standard->invoke("String", type::string_t("Hello!"));

    rephp::reflection::internal_value::dump(std::cout, a);
    rephp::reflection::internal_value::dump(std::cout, b);
    rephp::reflection::internal_value::dump(std::cout, c);
    rephp::reflection::internal_value::dump(std::cout, a + b);
}
```