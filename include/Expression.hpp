#ifndef Expression_HPP
#define Expression_HPP

#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>
#include <complex>
#include <iostream>
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
};

template <Numeric T>
class ConstNode : public Node<T>
{
    T value;

public:
    ConstNode(T val);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &var) const override;
};

template <Numeric T>
class VarNode : public Node<T>
{
    std::string var;

public:
    VarNode(std::string var) : var(var) {}
    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;
};

template <Numeric T>
class BinaryOpNode : public Node<T>
{
private:
    ExprType op;
    std::shared_ptr<Node<T>> left, right;

public:
    BinaryOpNode(ExprType op, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;
};

template <Numeric T>
class FunctionNode : public Node<T>
{
private:
    ExprType func;
    std::shared_ptr<Node<T>> arg;

public:
    FunctionNode(ExprType func, std::shared_ptr<Node<T>> arg);

    T eval(const std::map<std::string, T> &vars) const override;

    std::string to_string() const override;

    std::shared_ptr<Node<T>> clone() const override;

    std::shared_ptr<Node<T>> diff(const std::string &dvar) const override;
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
ConstNode<T>::ConstNode(T val) : value(val) {}

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
BinaryOpNode<T>::BinaryOpNode(ExprType op, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r) : op(op), left(l), right(r) {}

template <Numeric T>
T BinaryOpNode<T>::eval(const std::map<std::string, T> &vars) const
{
    T left_val = left->eval(vars);
    T right_val = right->eval(vars);

    switch (op)
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
    return "(" + left->to_string() + ExprTypeToString(op) + right->to_string() + ")";
}

template <Numeric T>
std::shared_ptr<Node<T>> BinaryOpNode<T>::clone() const
{
    return std::make_shared<BinaryOpNode<T>>(op, left->clone(), right->clone());
}

template <Numeric T>
std::shared_ptr<Node<T>> BinaryOpNode<T>::diff(const std::string &dvar) const
{
    auto left_diff = left->diff(dvar);
    auto right_diff = right->diff(dvar);

    switch (op)
    {
    case ExprType::Add:
        return std::make_shared<BinaryOpNode<T>>(ExprType::Add, left_diff->clone(), right_diff->clone()); // проверить что кто-то равен 0
    case ExprType::Subtract:
        return std::make_shared<BinaryOpNode<T>>(ExprType::Subtract, left_diff->clone(), right_diff->clone());
    case ExprType::Multiply:
    {
        auto new_left = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, left_diff->clone(), right->clone());
        auto new_right = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, left->clone(), right_diff->clone());

        return std::make_shared<BinaryOpNode<T>>(ExprType::Add, new_left->clone(), new_right->clone()); // проверить что кто-то равен
    }
    case ExprType::Divide:
    {
        auto new_sub_left = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, left_diff->clone(), right->clone());
        auto new_sub_right = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, left->clone(), right_diff->clone());

        auto num = std::make_shared<BinaryOpNode<T>>(ExprType::Subtract, new_sub_left->clone(), new_sub_right->clone());
        auto den = std::make_shared<BinaryOpNode<T>>(ExprType::Power, right->clone(), std::make_shared<ConstNode<T>>(2));

        return std::make_shared<BinaryOpNode<T>>(ExprType::Divide, num, den);
    }
    case ExprType::Power:
    {
        auto big_left_node = std::make_shared<BinaryOpNode<T>>(ExprType::Power, left->clone(), right->clone());
        auto small_left_node = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, left_diff->clone(), std::make_shared<BinaryOpNode<T>>(ExprType::Divide, right->clone(), left->clone()));
        auto small_right_node = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, right_diff->clone(), std::make_shared<FunctionNode<T>>(ExprType::Ln, left));
        auto big_right_node = std::make_shared<BinaryOpNode<T>>(ExprType::Add, small_left_node, small_right_node);
        return std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, big_left_node, big_right_node);
    }
    }
    throw std::runtime_error("Doesn`t exist operation");
}

/*=========*/
/*FunctionNode*/
/*=========*/

template <Numeric T>
FunctionNode<T>::FunctionNode(ExprType func, std::shared_ptr<Node<T>> arg) : func(func), arg(arg) {}

template <Numeric T>
T FunctionNode<T>::eval(const std::map<std::string, T> &vars) const
{
    T arg_val = arg->eval(vars);
    switch (func)
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
    return ExprTypeToString(func) + "(" + arg->to_string() + ")";
}

template <Numeric T>
std::shared_ptr<Node<T>> FunctionNode<T>::clone() const
{
    return std::make_shared<FunctionNode<T>>(func, arg->clone());
}

template <Numeric T>
std::shared_ptr<Node<T>> FunctionNode<T>::diff(const std::string &dvar) const
{
    auto arg_diff = arg->diff(dvar);
    std::shared_ptr<Node<T>> new_node;
    switch (func)
    {
    case ExprType::Sin:
    {
        new_node = std::make_shared<FunctionNode<T>>(ExprType::Cos, arg->clone());
        return std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, new_node->clone(), arg_diff->clone());
    }
    case ExprType::Cos:
    {
        auto new_node = std::make_shared<FunctionNode<T>>(ExprType::Sin, arg->clone());
        auto neg_node = std::make_shared<BinaryOpNode<T>>(
            ExprType::Multiply,
            std::make_shared<ConstNode<T>>(-1),
            new_node);
        return std::make_shared<BinaryOpNode<T>>(
            ExprType::Multiply,
            neg_node,
            arg_diff->clone());
    }
    case ExprType::Exp:
    {
        new_node = std::make_shared<FunctionNode<T>>(ExprType::Exp, arg->clone());
        return std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, new_node->clone(), arg_diff->clone());
    }
    case ExprType::Ln:
    {
        auto reciprocal = std::make_shared<BinaryOpNode<T>>(
            ExprType::Divide,
            std::make_shared<ConstNode<T>>(1),
            arg->clone());
        return std::make_shared<BinaryOpNode<T>>(
            ExprType::Multiply,
            reciprocal,
            arg_diff->clone());
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
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Add, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator-(const Expression<T> &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Subtract, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}
template <Numeric T>

Expression<T> Expression<T>::operator*(const Expression<T> &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, this->root->clone(), other.clone());
    return Expression<T>(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator/(const Expression<T> &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Divide, this->root->clone(), other.clone());
    return Expression(newNode);
}

template <Numeric T>
Expression<T> Expression<T>::operator^(const Expression<T> &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Power, this->root->clone(), other.clone());
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

#endif // Expression_HPP
