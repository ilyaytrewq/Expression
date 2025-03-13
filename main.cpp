#include <iostream>
#include <map>
#include <Expression.hpp>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("Недоостаточно аргументов\n");
        return 1;
    }\
    std::string type = argv[1];
    if(type == "--eval"){
        std::string expr_str = argv[2];
        std::map<std::string, Real> vars;
        for(int i = 3; i < argc; ++i){
            std::string Var = argv[i];
            int pos = Var.find("=");
            std::string var = Var.substr(0, pos);
            Real val = stold(Var.substr(pos + 1));
            if(vars.count(var))
                throw std::runtime_error("2 раза одна переменная");
            vars[var] = val;
        }
        std::cout << make_expression<Real>(expr_str).eval(vars) << '\n';
    }
    else if (type == "--diff"){ 
        if(argc != 5 || std::string(argv[3]) != "--by")
            throw std::runtime_error("некоректный запрос");
        std::string expr_str = argv[2];
        std::cout << make_expression<Real>(expr_str).diff(argv[4]) << '\n';
    }
    else{
        throw std::runtime_error("Unkown function");
    }
}