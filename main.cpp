#include <iostream>
#include <map>
#include <Expression.hpp>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        throw std::runtime_error("Not enough arguments\n");
        return 1;
    }
    std::string type = argv[1];

    bool is_comp = 0;
    for (int i = 2; i < argc; ++i)
    {
        is_comp |= is_complex(argv[i]);
    }

    if (type == "--eval")
    {
        std::string expr_str = argv[2];
        if (!is_comp)
        {
            std::map<std::string, Real> vars;
            for (int i = 3; i < argc; ++i)
            {
                std::string Var = argv[i];
                int pos = Var.find("=");
                std::string var = Var.substr(0, pos);
                Real val = stold(Var.substr(pos + 1));
                if (vars.count(var))
                    throw std::runtime_error("2 times one variable");
                vars[var] = val;
            }
            std::cout << make_expression<Real>(expr_str).eval(vars) << '\n';
        }
        else{
            std::map<std::string, Complex> vars;
            for (int i = 3; i < argc; ++i)
            {
                std::string Var = argv[i];
                int pos = Var.find("=");
                std::string var = Var.substr(0, pos);
                // std::cout << "var = " << Var.substr(pos + 1) << '\n';
                Complex val = ParseComplex(Var.substr(pos + 1));
                // std::cout << val << '\n';
                if (vars.count(var))
                    throw std::runtime_error("2 times one variable");
                vars[var] = val;
            }
            std::cout << make_expression<Complex>(expr_str).eval(vars) << '\n';
        }
    }
    else if (type == "--diff")
    {
        if (argc != 5 || std::string(argv[3]) != "--by")
            throw std::runtime_error("Invalid request");
        std::string expr_str = argv[2];
        if(!is_comp)
            std::cout << make_expression<Real>(expr_str).diff(argv[4]) << '\n';
        else
        std::cout << make_expression<Complex>(expr_str).diff(argv[4]) << '\n';
    }
    else
    {
        throw std::runtime_error("Unkown function");
    }
}