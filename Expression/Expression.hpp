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

enum class ExprType;

inline std::string ExprTypeToString(ExprType type);

template <Numeric T>
class Node
{
public:
    virtual ~Node() = default;
    virtual T eval(const std::map<std::string, T> &vars) const = 0;
    virtual std::string to_string() const = 0;
    virtual std::shared_ptr<Node<T>> clone() const = 0;
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
};

template <Numeric T>
std::ostream &operator<<(std::ostream &out, const Expression<T> &expr);

#endif // Expression_HPP