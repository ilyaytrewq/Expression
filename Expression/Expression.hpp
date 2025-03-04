#ifndef Expression_HPP
#define Expression_HPP

#include <memory>
#include <map>
#include <stdexcept>
#include <cmath>

using Type = long double;

enum class ExprType
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

inline std::string exprToString(ExprType type)
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

class Node
{
public:
    virtual ~Node() = default;
    virtual Type eval(const std::map<std::string, Type> &vars) const = 0;
    virtual std::string to_string() const = 0;
    virtual std::shared_ptr<Node> clone() const = 0;
};

class ConstNode : public Node
{
    Type value;

public:
    ConstNode(Type val);

    Type eval(const std::map<std::string, Type> &vars) const override
    {
        return value;
    }

    std::string to_string() const override
    {
        return std::to_string(value);
    }

    std::shared_ptr<Node> clone() const override
    {
        return std::make_shared<ConstNode>(value);
    }
};

class VarNode : public Node
{
    std::string var;

public:
    VarNode(std::string var) : var(var) {}
    Type eval(const std::map<std::string, Type> &vars) const override
    {
        if (vars.find(var) == vars.end())
            throw std::runtime_error("Variable '" + var + "' is not provided");

        return vars.find(var)->second;
    }

    std::string to_string() const override
    {
        return var;
    }

    std::shared_ptr<Node> clone() const override
    {
        return std::make_shared<VarNode>(var);
    }
};

class BinaryOpNode : public Node
{
private:
    ExprType op;
    std::shared_ptr<Node> left, right;

public:
    BinaryOpNode(ExprType op, std::shared_ptr<Node> l, std::shared_ptr<Node> r) : op(op), left(l), right(r) {}

    Type eval(const std::map<std::string, Type> &vars) const override
    {
        Type left_val = left->eval(vars);
        Type right_val = right->eval(vars);

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

    std::string to_string() const override
    {
        return left->to_string() + exprToString(op) + right->to_string();
    }

    std::shared_ptr<Node> clone() const override
    {
        return std::make_shared<BinaryOpNode>(op, left->clone(), right->clone());
    }
};

class FunctionNode : public Node
{
private:
    ExprType func;
    std::shared_ptr<Node> arg;

public:
    FunctionNode(ExprType func, std::shared_ptr<Node> arg): func(func), arg(arg) {}

    Type eval(const std::map<std::string, Type> &vars) const override
    {
        Type arg_val = arg->eval(vars);
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

    std::string to_string() const override
    {
        return exprToString(func) + "(" + arg->to_string() + ")";
    }

    std::shared_ptr<Node> clone() const override
    {
        return std::make_shared<FunctionNode>(func, arg->clone());
    }
};



/*
=====================
EXPRESSION CLASS
=====================
*/



class Expression
{
private:
    std::shared_ptr<Node> root;


public:
    Expression(Type val);
    Expression(const std::string &var);
    Expression(const char var[]);
    explicit Expression(std::shared_ptr<Node> node);
    Expression(const Expression &other);     // Копирование
    Expression(Expression &&other) noexcept; // Перемещение

    Expression &operator=(const Expression &other);
    Expression &operator=(Expression &&other) noexcept;

    Expression operator+(const Expression &other) const;
    Expression operator-(const Expression &other) const;
    Expression operator*(const Expression &other) const;
    Expression operator/(const Expression &other) const;
    Expression operator^(const Expression &other) const;

    Type eval(const std::map<std::string, Type> &vars) const;

    std::string to_string() const;

    std::shared_ptr<Node> clone() const; 
};

template<typename Type>
std::ostream &operator<<(std::ostream &out, const Expression &expr);

template <typename Type>
Expression sin(const Expression &expr);

template <typename Type>
Expression cos(const Expression &expr);

template <typename Type>
Expression exp(const Expression &expr);

template <typename Type>
Expression ln(const Expression &expr);

#endif // Expression_HPP