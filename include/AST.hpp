#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <memory>
#include <cmath>
#include <map>
#include <ExprType.hpp>
#include <stdexcept>

template <typename T>
class Node
{
public:
    virtual ~Node() = default;
    virtual T eval(const std::map<std::string, T> &vars) const = 0;
    virtual std::string to_string() const = 0;
    virtual std::shared_ptr<Node<T>> clone() const = 0;
};

template <typename T>
class ConstNode : public Node<T>
{
    T value;

public:
    ConstNode(T val): value(val) {}

    T eval(const std::map<std::string, T> &vars) const override
    {
        return value;
    }

    std::string to_string() const override
    {
        return std::to_string(value);
    }

    std::shared_ptr<Node<T>> clone() const override
    {
        return std::make_shared<ConstNode<T>>(value);
    }
};

template <typename T>
class VarNode : public Node<T>
{
    std::string var;

public:
    VarNode(std::string var) : var(var) {}
    T eval(const std::map<std::string, T> &vars) const override
    {
        if (vars.find(var) == vars.end())
            throw std::runtime_error("Variable '" + var + "' is not provided");

        return vars[var];
    }

    std::string to_string() const override
    {
        return var;
    }

    std::shared_ptr<Node<T>> clone() const override
    {
        return std::make_shared<VarNode<T>>(var);
    }
};

template <typename T>
class BinaryOpNode : public Node<T>
{
private:
    ExprType op;
    std::shared_ptr<Node<T>> left, right;

public:
    BinaryOpNode(ExprType op, std::shared_ptr<Node<T>> l, std::shared_ptr<Node<T>> r) : op(op), left(l), right(r) {}

    T eval(const std::map<std::string, T> &vars) const override
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

        return std::runtime_error("Doesn`t exist operation");
    }

    std::string to_string() const override
    {
        return left->to_string() + exprTypeToString(op) + right->to_string();
    }

    std::shared_ptr<Node<T>> clone() const override
    {
        return std::make_shared<BinaryOpNode<T>>(op, left->clone(), right->clone());
    }
};

template <typename T>
class FunctionNode : public Node<T>
{
private:
    ExprType func;
    std::shared_ptr<Node<T>> arg;

public:
    FunctionNode(ExprType func, std::shared_ptr<Node<T>> arg): func(func), arg(arg) {}

    T eval(const std::map<std::string, T> &vars) const override
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
        return std::runtime_error("Fuction doesn`t exist");
    }

    std::string to_string() const override
    {
        return exprTypeToString(func) + "(" + arg->to_stirng() + ")";
    }

    std::shared_ptr<Node<T>> clone() const override
    {
        return std::make_shared<FunctionNode<T>>(func, arg->clone());
    }
};

#endif // AST_HPP
