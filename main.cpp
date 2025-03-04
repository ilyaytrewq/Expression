#include <Expression.hpp>
#include <iostream>
#include <map>

int main()
{

    std::string s = "x";
    Expression<Real> b("x");
    Expression<Real> a(5);
    auto c = (a + (b / a ^ a)).sin();
    std::cout << a.to_string() << '\n';
    std::cout << b.to_string() << '\n';
    std::cout << c.to_string() << '\n';
    std::map<std::string, Real> m;
    m["x"] = 5;
    std::cout << c.eval(m) << '\n';
}