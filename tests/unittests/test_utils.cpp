#include <catch.hpp>
#include <Utils.hpp>

#include <algorithm>
#include <iostream>

TEST_CASE("replace nothing", "[stringReplace]")
{
    std::string::size_type  len;

    auto    str = GENERATE(as<std::string>{}, "", "nopenope", "nope");
    len = str.length();
    ft::stringReplaceMultiple(str, "sub", "bus");
    CHECK(len == str.length());
}

TEST_CASE("replace everything", "[stringReplace]")
{
    auto    str = GENERATE(as<std::string>{}, "subsubsub", "sub");
    ft::stringReplaceMultiple(str, "sub", "bus");
    for (int idx = 0; idx < str.size(); idx++)
    {
        switch (idx % 3)
        {
            case 0:
                CHECK(str[idx] == 'b');
                break ;
            case 1:
                CHECK(str[idx] == 'u');
                break ;
            case 2:
                CHECK(str[idx] == 's');
                break ;
        }
    }
}

TEST_CASE("replace parts", "[stringReplace]")
{
    auto    str = GENERATE(as<std::string>{}, "nopesubnope", "subnopesubnopesub");
    ft::stringReplaceMultiple(str, "sub", "bus");
    if (str.size() == 9)
        CHECK(str == "nopebusnope");
    if (str.size() == 15)
        CHECK(str == "busnopebusnopebus");
}
