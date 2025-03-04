#include <Expression.hpp>
#include <iostream>
#include <map>

int main(){
    Expression a(5), b("x"), d("y");
    auto c = a + (b^d);
    std::cout << a.to_string() << '\n';
    std::cout << b.to_string() << '\n';
    std::cout << c.to_string() << '\n';
    std::map<std::string, long double> m;
    m["x"] = 2;
    m["y"] = 3;
    std::cout << c.eval(m) << '\n';
    std::cout << (b^d).eval(m) << '\n';
}