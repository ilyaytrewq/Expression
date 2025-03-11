#include <iostream>
#include <map>
#include <Expression.hpp>
#include <string>

int main()
{
    std::string s = "x * (5 + 2 - 2) * 1 * 0 - 3 * x ^ 2";
    auto exp = make_expression<Real>(s);
    std::map<std::string, Real> m;
    m["x"] = 2;
    std::cout << exp.to_string() << '\n';
    std::cout << exp.diff("x").to_string() << '\n';
    std::cout << exp.eval(m) << '\n';
}
/*
cos(x^2+x)*2*x^2
*/