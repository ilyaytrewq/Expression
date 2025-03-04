#ifndef Expression_HPP
#define Expression_HPP

#include <AST.hpp>
#include <memory>
#include <map>

template <typename T>
class Expression
{
private:
    std::shared_ptr<Node<T>> root;

    explicit Expression(std::shared_ptr<Node<T>> node);

public:
    Expression(T val);
    Expression(const std::string &var);
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

    friend std::ostream &operator<<(std::ostream &out, const Expression &expr) const;
};

template <typename T>
Expression<T> sin(const Expression<T> &expr);

template <typename T>
Expression<T> cos(const Expression<T> &expr);

template <typename T>
Expression<T> exp(const Expression<T> &expr);

template <typename T>
Expression<T> ln(const Expression<T> &expr);

#endif // Expression_HPP