#include <C:\Users\Lenovo\projects\Expression\include\Expression.hpp>
#include <iostream>
#include <map>

int main()
{

    std::string s = "x";
    Expression<int> b("x");
    Expression<int> a(5);
    auto c = ((b^2) + a).sin();
    std::cout << a.to_string() << '\n';
    std::cout << b.to_string() << '\n';
    std::cout << c.to_string() << '\n';
    std::map<std::string, int> m;
    m["x"] = 5;
    std::cout << c.eval(m) << '\n';
    std::cout << c.diff("x").to_string() << '\n';
}
/*
cos(x^2+x)*2*x^2
*/