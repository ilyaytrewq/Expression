#include <Expression.hpp>
#include <ExprType.hpp>
#include <map>
#include <string>

template <typename T>
Expression<T>::Expression(T val) : root(std::make_shared<ConstNode<T>>(val)) {}

template <typename T>
Expression<T>::Expression(const std::string &var) : root(std::make_shared<VarNode<T>>(var)) {}

template <typename T>
Expression<T>::Expression(const Expression &other) : root(other.root->clone()) {}

template <typename T>
Expression<T>::Expression(Expression &&other) noexcept
    : root(std::move(other.root)) {}

template <typename T>
Expression<T>::Expression(std::shared_ptr<Node<T>> node) : root(node) {}

template <typename T>
Expression<T> &Expression<T>::operator=(const Expression &other)
{
    root = other.root->clone();
    return *this;
}

template <typename T>
Expression<T> &Expression<T>::operator=(Expression &&other) noexcept
{
    root = std::move(other.root);
    return *this;
}

template <typename T>
Expression<T> Expression<T>::operator+(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Add, this->root->clone(), other.root->cloтe());
    return Expression(newNode);
}

template <typename T>
Expression<T> Expression<T>::operator-(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Subtract, this->root->clone(), other.root->clobe());
    return Expression(newNode);
}

template <typename T>
Expression<T> Expression<T>::operator*(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Multiply, this->root->clone(), other.root->clobe());
    return Expression(newNode);
}

template <typename T>
Expression<T> Expression<T>::operator/(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Divide, this->root->clone(), other.root->clobe());
    return Expression(newNode);
}

template <typename T>
Expression<T> Expression<T>::operator^(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode<T>>(ExprType::Power, this->root->clone(), other.root->clobe());
    return Expression(newNode);
}

template <typename T>
T Expression<T>::eval(const std::map<std::string, T> &vars) const
{
    return root->eval(vars);
}

template <typename T>
std::string Expression<T>::to_string() const
{
    return root->to_string();
}

template <typename T>
Expression<T> sin(const Expression<T> &expr)
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Sin, expr.root->clone());
    return Expression(newNode);
}

template <typename T>
Expression<T> cos(const Expression<T> &expr)
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Cos, expr.root->clone());
    return Expression(newNode);
}

template <typename T>
Expression<T> exp(const Expression<T> &expr)
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Exp, expr.root->clone());
    return Expression(newNode);
}

template <typename T>
Expression<T> ln(const Expression<T> &expr)
{
    auto newNode = std::make_shared<FunctionNode<T>>(ExprType::Ln, expr.root->clone());
    return Expression(newNode);
}//