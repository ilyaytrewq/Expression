#ifndef Expression_HPP
#define Expression_HPP

#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>
#include <complex>
#include <concepts>
#include <iostream>
#include <functional>
#include <stack>
#include <type_traits>

using Real = long double;
using Complex = std::complex<Real>;

template <typename T>
concept Numeric = std::is_arithmetic_v<T> || (std::is_same_v<T, Complex>);

/*==========*/
/*ExprType*/
/*==========*/

enum class ExprType : char;

inline std::string ExprTypeToString(ExprType type);

/*===========*/
/*AST*/
/*==========*/

template <Numeric T>
class Node
{
public:
    virtual ~Node() = default;
    virtual T eval(const std::map<std::string, T> &vars) const = 0;
    virtual std::string to_string() const = 0;
    virtual std::shared_ptr<Node<T>> clone() const = 0;
    virtual std::shared_ptr<Node<T>> diff(const std::string &dvar) const = 0;
    virtual ExprType getType() const = 0;
};

template <Numeric T>
class ConstNode : public Node<T>
{
    T value;
    ExprType type;

public:
    ConstNode(T val);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &var) const override;

    ExprType getType() const override { return type; }

    T getVal() { return value; }
};

template <Numeric T>
class VarNode : public Node<T>
{
    std::string var;
    ExprType type;

public:
    VarNode(std::string var);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;

    ExprType getType() const override { return type; }
};

template <Numeric T>
class BinaryOpNode : public Node<T>
{
private:
    ExprType type;
    std::shared_ptr<Node<T>> left, right;

public:
    BinaryOpNode(ExprType type, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;

    ExprType getType() const override { return type; }
};

template <Numeric T>
class FunctionNode : public Node<T>
{
private:
    ExprType type;
    std::shared_ptr<Node<T>> arg;

public:
    FunctionNode(ExprType type, std::shared_ptr<Node<T>> arg);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;

    ExprType getType() const override { return type; }
};

/*
=====================
EXPRESSION CLASS
=====================
*/

template <Numeric T = Real>
class Expression
{
private:
    std::shared_ptr<Node<T>> root;

public:
    Expression(T val);
    Expression(const std::string &var);
    Expression(const char var[]);
    explicit Expression(std::shared_ptr<Node<T>> node);
    Expression(const Expression &other);     // Копирование
    Expression(Expression &&other) noexcept; // Перемещение

    Expression &operator=(const Expression &other);
    Expression &operator=(Expression &&other) noexcept;

    Expression operator+(const Expression &other) const;
    Expression operator-(const Expression &other) const;
    Expression operator*(const Expression &other) const;
    Expression operator/(const Expression &other) const;
    Expression operator^(const Expression &other) const;

    T eval(const std::map<std::string, T> &vars) const;

    std::string to_string() const;

    std::shared_ptr<Node<T>> clone() const;

    Expression sin() const;
    Expression cos() const;
    Expression exp() const;
    Expression ln() const;

    Expression diff(const std::string &dvar) const;
};

template <Numeric T>
std::ostream &operator<<(std::ostream &out, const Expression<T> &expr);

template <Numeric T>
bool is_one(std::shared_ptr<Node<T>> node);

template <Numeric T>
bool is_zero(std::shared_ptr<Node<T>> node);

/*==========*/
/*Realisation*/
/*==========*/

enum class ExprType : char
{
    // Базовые элементы
    Constant, // Число (например, 5.8)
    Variable, // Переменная (например, "x")

    // Бинарные арифметические операции
    Add,      // a + b
    Subtract, // a - b
    Multiply, // a * b
    Divide,   // a / b
    Power,    // a ^ b (возведение в степень)

    // Унарные операции
    Negate, // Унарный минус (-a)

    // Функции
    Sin, // sin(a)
    Cos, // cos(a)
    Ln,  // ln(a) (натуральный логарифм)
    Exp  // exp(a) (экспонента)
};

std::string ExprTypeToString(ExprType type)
{
    switch (type)
    {
    case ExprType::Constant:
        return "Const";
    case ExprType::Variable:
        return "Var";
    case ExprType::Add:
        return "+";
    case ExprType::Subtract:
        return "-";
    case ExprType::Multiply:
        return "*";
    case ExprType::Divide:
        return "/";
    case ExprType::Power:
        return "^";
    case ExprType::Negate:
        return "-";
    case ExprType::Sin:
        return "sin";
    case ExprType::Cos:
        return "cos";
    case ExprType::Ln:
        return "ln";
    case ExprType::Exp:
        return "exp";
    default:
        return "Unknown";
    }
}

/*=========*/
/*ConstNode*/
/*=========*/

template <Numeric T>
ConstNode<T>::ConstNode(T val) : value(val), type(ExprType::Constant) {}

template <Numeric T>
T ConstNode<T>::eval(const std::map<std::string, T> &vars) const
{
    return value;
}

template <Numeric T>
std::string ConstNode<T>::to_string() const
{
    return std::to_string(value);
}

template <Numeric T>
std::shared_ptr<Node<T>> ConstNode<T>::clone() const
{
    return std::make_shared<ConstNode<T>>(value);
}

template <Numeric T>
std::shared_ptr<Node<T>> ConstNode<T>::diff(const std::string &dvar) const
{
    return std::make_shared<ConstNode<T>>(0);
}

/*=========*/
/*VarNode*/
/*=========*/

template <Numeric T>
VarNode<T>::VarNode(std::string s) : var(s), type(ExprType::Variable) {}

template <Numeric T>
T VarNode<T>::eval(const std::map<std::string, T> &vars) const
{
    if (vars.find(var) == vars.end())
        throw std::runtime_error("Variable '" + var + "' is not provided");

    return vars.find(var)->second;
}

template <Numeric T>
std::string VarNode<T>::to_string() const
{
    return var;
}

template <Numeric T>
std::shared_ptr<Node<T>> VarNode<T>::clone() const
{
    return std::make_shared<VarNode<T>>(var);
}

template <Numeric T>
std::shared_ptr<Node<T>> VarNode<T>::diff(const std::string &dvar) const
{
    return std::make_shared<ConstNode<T>>(var == dvar ? 1 : 0);
}

/*=========*/
/*BinaryOpNode*/
/*=========*/

template <Numeric T>
std::shared_ptr<Node<T>> make(ExprType op, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r)
{
    return std::static_pointer_cast<Node<T>>(std::make_shared<BinaryOpNode<T>>(op, l, r));
}

template <Numeric T>
std::shared_ptr<Node<T>> del_zero(ExprType type, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r)
{
    if (is_zero(l))
        return (((type == ExprType::Subtract ? Expression<T>(-1) : Expression<T>(1))) * Expression(r)).clone();
    if (is_zero<T>(r))
        return l;
    if (l->getType() == ExprType::Constant && r->getType() == ExprType::Constant)
        return std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(l->eval({}) + (type == ExprType::Add ? r->eval({}) : -r->eval({}))));
    return make<T>(type, l, r);
}

template <Numeric T>
std::shared_ptr<Node<T>> del_mult(ExprType type, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r)
{
    if (is_zero<T>(l) || is_zero<T>(r))
        return std::make_shared<ConstNode<T>>(0);
    if (is_one<T>(l))
        return r;
    if (is_one<T>(r))
        return l;
    if (l->getType() == ExprType::Constant && r->getType() == ExprType::Constant)
        return std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(l->eval({}) * r->eval({})));
    return make<T>(type, l, r);
}

template <Numeric T>
std::shared_ptr<Node<T>> del_div(ExprType type, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r)
{
    if (is_one<T>(r))
        return l;
    if (is_zero<T>(l))
        return std::make_shared<ConstNode<T>>(0);
    if (l->getType() == ExprType::Constant && r->getType() == ExprType::Constant)
        return std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(l->eval({}) / r->eval({})));
    return make<T>(type, l, r);
}

template <Numeric T>
std::shared_ptr<Node<T>> del_pow(ExprType type, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r)
{
    if (is_one<T>(r))
        return l;
    if (is_zero<T>(r))
        return std::make_shared<ConstNode<T>>(1);
    if (l->getType() == ExprType::Constant && r->getType() == ExprType::Constant)
        return std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(std::pow(l->eval({}), r->eval({}))));
    return make<T>(type, l, r);
}

template <Numeric T>
BinaryOpNode<T>::BinaryOpNode(ExprType op, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r) : type(op), left(l), right(r) {}

template <Numeric T>
T BinaryOpNode<T>::eval(const std::map<std::string, T> &vars) const
{
    T left_val = left->eval(vars);
    T right_val = right->eval(vars);

    switch (type)
    {
    case ExprType::Add:
        return left_val + right_val;
    case ExprType::Subtract:
        return left_val - right_val;
    case ExprType::Multiply:
        return left_val * right_val;
    case ExprType::Divide:
        return left_val / right_val;
    case ExprType::Power:
        return std::pow(left_val, right_val);
    }

    throw std::runtime_error("Doesn`t exist operation");
}

template <Numeric T>
std::string BinaryOpNode<T>::to_string() const
{
    return "(" + left->to_string() + ExprTypeToString(type) + right->to_string() + ")";
}

template <Numeric T>
std::shared_ptr<Node<T>> BinaryOpNode<T>::clone() const
{
    return std::make_shared<BinaryOpNode<T>>(type, left->clone(), right->clone());
}

template <Numeric T>
std::shared_ptr<Node<T>> BinaryOpNode<T>::diff(const std::string &dvar) const
{
    auto left_diff = left->diff(dvar);
    auto right_diff = right->diff(dvar);

    switch (type)
    {
    case ExprType::Add:
    {
        return del_zero(ExprType::Add, left_diff->clone(), right_diff->clone());
    }
    case ExprType::Subtract:
    {
        return del_zero(ExprType::Subtract, left_diff->clone(), right_diff->clone());
    }
    case ExprType::Multiply:
    {

        std::shared_ptr<Node<T>> new_left = del_mult(ExprType::Multiply, left_diff->clone(), right->clone());
        std::shared_ptr<Node<T>> new_right = del_mult(ExprType::Multiply, left->clone(), right_diff->clone());

        return del_zero(ExprType::Add, new_left->clone(), new_right->clone());
    }
    case ExprType::Divide:
    {
        auto new_sub_left = del_mult(ExprType::Multiply, left_diff->clone(), right->clone());

        auto new_sub_right = del_mult(ExprType::Multiply, left->clone(), right_diff->clone());

        auto num = del_zero(ExprType::Subtract, new_sub_left->clone(), new_sub_right->clone());

        auto den = del_pow(ExprType::Power, right->clone(), std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(2)));

        return del_div(ExprType::Divide, num->clone(), den->clone());
    }
    case ExprType::Power:
    {
        auto big_left_node = del_pow(ExprType::Power, left->clone(), right->clone());

        auto small_left_node = del_mult(ExprType::Multiply, left_diff->clone(), del_div(ExprType::Divide, right->clone(), left->clone()));

        auto small_right_node = del_mult(ExprType::Multiply, right_diff->clone(), std::static_pointer_cast<Node<T>>(std::make_shared<FunctionNode<T>>(ExprType::Ln, left)));

        auto big_right_node = del_zero(ExprType::Add, small_left_node, small_right_node);

        return del_mult(ExprType::Multiply, big_left_node, big_right_node);
    }
    }
    throw std::runtime_error("Doesn`t exist operation");
}

/*=========*/
/*FunctionNode*/
/*=========*/

template <Numeric T>
FunctionNode<T>::FunctionNode(ExprType type, std::shared_ptr<Node<T>> arg) : type(type), arg(arg) {}

template <Numeric T>
T FunctionNode<T>::eval(const std::map<std::string, T> &vars) const
{
    T arg_val = arg->eval(vars);
    switch (type)
    {
    case ExprType::Sin:
        return std::sin(arg_val);
    case ExprType::Cos:
        return std::cos(arg_val);
    case ExprType::Exp:
        return std::exp(arg_val);
    case ExprType::Ln:
        return std::log(arg_val);
    }
    throw std::runtime_error("Fuction doesn`t exist");
}

template <Numeric T>
std::string FunctionNode<T>::to_string() const
{
    return ExprTypeToString(type) + "(" + arg->to_string() + ")";
}

template <Numeric T>
std::shared_ptr<Node<T>> FunctionNode<T>::clone() const
{
    return std::make_shared<FunctionNode<T>>(type, arg->clone());
}

template <Numeric T>
std::shared_ptr<Node<T>> FunctionNode<T>::diff(const std::string &dvar) const
{
    auto arg_diff = arg->diff(dvar);
    std::shared_ptr<Node<T>> new_node;
    switch (type)
    {
    case ExprType::Sin:
    {
        new_node = std::make_shared<FunctionNode<T>>(ExprType::Cos, arg->clone());
        return del_mult(ExprType::Multiply, new_node->clone(), arg_diff->clone());
    }
    case ExprType::Cos:
    {
        auto new_node = std::make_shared<FunctionNode<T>>(ExprType::Sin, arg->clone());
        auto neg_node = del_mult(ExprType::Multiply, std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(-1)), new_node->clone());

        return del_mult(ExprType::Multiply, neg_node->clone(), arg_diff->clone());
    }
    case ExprType::Exp:
    {
        new_node = std::make_shared<FunctionNode<T>>(ExprType::Exp, arg->clone());

        return del_mult(ExprType::Multiply, new_node->clone(), arg_diff->clone());
    }
    case ExprType::Ln:
    {
        auto reciprocal = del_div(ExprType::Divide, std::static_pointer_cast<Node<T>>(std::make_shared<ConstNode<T>>(1)), arg->clone());

        return del_mult(ExprType::Multiply, reciprocal->clone(), arg_diff->clone());
    }
    }
    throw std::runtime_error("Fuction doesn`t exist");
}

/*=========*/
/*Expression*/
/*=========*/

template <Numeric T>

Expression<T>::Expression(T val) : root(std::make_shared<ConstNode<T>>(val))
{
}

template <Numeric T>

Expression<T>::Expression(const std::string &var) : root(std::make_shared<VarNode<T>>(var))
{
}

template <Numeric T>

Expression<T>::Expression(const char var[])
{
    root = std::make_shared<VarNode<T>>(std::string(var));
}
template <Numeric T>

Expression<T>::Expression(const Expression<T> &other) : root(other.root->clone())
{
}
template <Numeric T>

Expression<T>::Expression(Expression<T> &&other) noexcept
    : root(std::move(other.root))
{
}
template <Numeric T>

Expression<T>::Expression(std::shared_ptr<Node<T>> node) : root(node)
{
}
template <Numeric T>

Expression<T> &Expression<T>::operator=(const Expression<T> &other)
{
    root = other.root->clone();
    return *this;
}

template <Numeric T>
Expression<T> &Expression<T>::operator=(Expression<T> &&other) noexcept
{
    root = std::move(other.root);
    return *this;
}

template <Numeric T>
Expression<T> Expression<T>::operator+(const Expression<T> &other) const
{
    auto newNode = del_zero(ExprType::Add, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator-(const Expression<T> &other) const
{
    auto newNode = del_zero(ExprType::Subtract, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}
template <Numeric T>

Expression<T> Expression<T>::operator*(const Expression<T> &other) const
{
    auto newNode = del_mult(ExprType::Multiply, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator/(const Expression<T> &other) const
{
    auto newNode = del_div(ExprType::Divide, this->root->clone(), other.clone());
    return Expression(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator^(const Expression<T> &other) const
{
    auto newNode = del_pow(ExprType::Power, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}

template <Numeric T>
T Expression<T>::eval(const std::map<std::string, T> &vars) const
{
    return root->eval(vars);
}

template <Numeric T>
std::string Expression<T>::to_string() const
{
    return root->to_string();
}

template <Numeric T>
std::shared_ptr<Node<T>> Expression<T>::clone() const
{
    return root->clone();
}

template <Numeric T>
std::ostream &operator<<(std::ostream &out, const Expression<T> &expr)
{
    out << expr.to_string();
    return out;
}

template <Numeric T>
bool is_one(std::shared_ptr<Node<T>> node)
{
    auto const_node = std::dynamic_pointer_cast<ConstNode<T>>(node);
    return (const_node && const_node->eval({}) == 1);
}

template <Numeric T>
bool is_zero(std::shared_ptr<Node<T>> node)
{
    auto const_node = std::dynamic_pointer_cast<ConstNode<T>>(node);
    return (const_node && const_node->eval({}) == 0);
}

template <Numeric T>
Expression<T> Expression<T>::sin() const
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Sin, this->clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::cos() const
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Cos, this->clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::exp() const
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Exp, this->clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::ln() const
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Ln, this->clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::diff(const std::string &dvar) const
{
    return Expression<T>(root->diff(dvar));
}

int priority(char op)
{
    if (op == '^')
        return 3;
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return -1;
}

bool is_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

template <Numeric T>
Expression<T> make_expression(const std::string &t)
{
    std::string s = "";
    for (char c : t)
    {
        if (c == ' ')
            continue;
        if (std::isalpha(c))
            s += std::tolower(c);
        else
            s += c;
    }
    std::stack<Expression<T>> vals;
    std::stack<char> ops;

    std::map<std::string, std::function<Expression<T>(Expression<T>)>>
        functions = {
            {"sin", [](Expression<T> arg)
             {
                 return arg.sin();
             }},
            {"cos", [](Expression<T> arg)
             {
                 return arg.cos();
             }},
            {"ln", [](Expression<T> arg)
             {
                 return arg.ln();
             }},
            {"exp", [](Expression<T> arg)
             {
                 return arg.exp();
             }}};

    for (int i = 0; i < (int)s.size(); ++i)
    {
        if (s[i] == ' ')
            continue;

        if (std::isdigit(s[i]))
        {
            std::string num = "";
            while (i < (int)s.size() && (std::isdigit(s[i]) || s[i] == '.'))
            {
                num += s[i];
                i++;
            }
            --i;
            vals.push(Expression<T>(std::stold(num)));
        }

        else if (std::isalpha(s[i]))
        {
            std::string token = "";
            while (i < s.size() && std::isalpha(s[i]))
            {
                token += s[i];
                i++;
            }

            if (functions.count(token))
            {
                if (s[i] != '(')
                {
                    throw std::runtime_error("Expected '('");
                }
                std::string arg = "";
                int balance = 1;
                ++i;
                while (i < s.size() && balance > 0)
                {
                    if (s[i] == '(')
                        balance++;
                    if (s[i] == ')')
                        balance--;
                    if (balance > 0)
                        arg += s[i];
                    ++i;
                }
                --i;
                if (arg.empty())
                    throw std::runtime_error("Expected argument\n");
                auto expr_arg = make_expression<T>(arg);
                vals.push(functions[token](expr_arg));
            }
            else
            {
                --i;
                vals.push(Expression<T>(token));
            }
        }
        else if (is_operator(s[i]))
        {
            if (i == 0 || is_operator(s[i - 1]))
            {
                if (s[i] == '-')
                {
                    std::string tmp = "";
                    while (i < s.size() && !is_operator(s[i]))
                    {
                        tmp += s[i];
                        i++;
                    }
                    --i;
                    vals.push(Expression<T>(-1) * make_expression<T>(tmp));
                }
                else
                {
                    throw std::runtime_error("Incorrect expression");
                }
            }
            else
            {
                while (!ops.empty() && priority(ops.top()) >= priority(s[i]))
                {
                    char op = ops.top();
                    ops.pop();

                    auto right = vals.top();
                    vals.pop();
                    auto left = vals.top();
                    vals.pop();

                    if (op == '+')
                        vals.push(left + right);
                    if (op == '-')
                        vals.push(left - right);
                    if (op == '*')
                        vals.push(left * right);
                    if (op == '/')
                        vals.push(left / right);
                    if (op == '^')
                        vals.push(left ^ right);
                }
                ops.push(s[i]);
            }
        }
        else if (s[i] == '(')
        {
            ops.push(s[i]);
        }

        else if (s[i] == ')')
        {
            while (!ops.empty() && ops.top() != '(')
            {
                char op = ops.top();
                ops.pop();

                Expression<T> right = vals.top();
                vals.pop();
                Expression<T> left = vals.top();
                vals.pop();

                if (op == '+')
                    vals.push(left + right);
                else if (op == '-')
                    vals.push(left - right);
                else if (op == '*')
                    vals.push(left * right);
                else if (op == '/')
                    vals.push(left / right);
                else if (op == '^')
                    vals.push(left ^ right);
            }
            ops.pop();
        }
    }

    while (!ops.empty())
    {
        char op = ops.top();
        ops.pop();

        Expression<T> right = vals.top();
        vals.pop();
        Expression<T> left = vals.top();
        vals.pop();

        if (op == '+')
            vals.push(left + right);
        else if (op == '-')
            vals.push(left - right);
        else if (op == '*')
            vals.push(left * right);
        else if (op == '/')
            vals.push(left / right);
        else if (op == '^')
            vals.push(left ^ right);
    }

    return vals.top();
}

#endif // Expression_HPP
