#include <gtest/gtest.h>

#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>

TEST(general, foo)
{
    char ch[] = "  12.,38 13,-39    ";
    char *p = ch;

    char *e = std::end(ch);
    --e;
    while (0 == (int)*e || isspace(*e)) {
        std::cout << *e << ' '  << (int)*e << std::endl;;
        --e;
    }
    ++e;

    while (p != e) {
        std::cout << strtod(p, &p) << std::endl;
        std::cout << strtod(++p, &p) << std::endl;
    }

}
