#include <Expression.hpp>
#include <iostream>

int main(){
    Expression a(5), b(4);
    auto c = a + b;
    std::cout << a.to_string() << '\n';
    std::cout << b.to_string() << '\n';
    std::cout << c.to_string() << '\n';
}