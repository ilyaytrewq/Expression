#include <C:\Users\Lenovo\projects\Expression\include\Expression.hpp>
#include <iostream>
#include <map>

int main()
{
    std::string s = "exp(x)*x";
    auto exp = make_expression<Real>(s);
    std::map<std::string, Real> m;
    m["x"] = 2;
    std::cout << exp.to_string() <<'\n';
    std::cout << exp.diff("x").to_string() << '\n';
    std::cout << exp.eval(m) << '\n';
}
/*
cos(x^2+x)*2*x^2
*/