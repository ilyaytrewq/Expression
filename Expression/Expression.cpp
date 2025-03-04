#include <Expression.hpp>
#include <map>
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

Expression::Expression(Type val) : root(std::make_shared<ConstNode>(val)) {}


Expression::Expression(const std::string &var) : root(std::make_shared<VarNode>(var)) {}


Expression::Expression(const char var[])
{
    root = std::make_shared<VarNode>(std::string(var));
}

Expression::Expression(const Expression &other) : root(other.root->clone()) {}

Expression::Expression(Expression &&other) noexcept
    : root(std::move(other.root)) {}

Expression::Expression(std::shared_ptr<Node> node) : root(node) {}

Expression &Expression::operator=(const Expression &other)
{
    root = other.root->clone();
    return *this;
}

Expression &Expression::operator=(Expression &&other) noexcept
{
    root = std::move(other.root);
    return *this;
}

Expression Expression::operator+(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode>(ExprType::Add, this->root->clone(), other.clone());
    return Expression(newNode);
}

Expression Expression::operator-(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode>(ExprType::Subtract, this->root->clone(), other.clone());
    return Expression(newNode);
}

Expression Expression::operator*(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode>(ExprType::Multiply, this->root->clone(), other.clone());
    return Expression(newNode);
}

Expression Expression::operator/(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode>(ExprType::Divide, this->root->clone(), other.clone());
    return Expression(newNode);
}

Expression Expression::operator^(const Expression &other) const
{
    auto newNode = std::make_shared<BinaryOpNode>(ExprType::Power, this->root->clone(), other.clone());
    return Expression(newNode);
}

Type Expression::eval(const std::map<std::string, Type> &vars) const
{
    return root->eval(vars);
}

std::string Expression::to_string() const
{
    return root->to_string();
}

std::shared_ptr<Node> Expression::clone() const
{
    return root->clone();
}

std::ostream &operator<<(std::ostream &out, const Expression &expr)
{
    out << expr.to_string();
    return out;
}

Expression sin(const Expression &expr)
{
    auto newNode = std::make_shared<FunctionNode>(ExprType::Sin, expr.clone());
    return Expression(newNode);
}

Expression cos(const Expression &expr)
{
    auto newNode = std::make_shared<FunctionNode>(ExprType::Cos, expr.clone());
    return Expression(newNode);
}

Expression exp(const Expression &expr)
{
    auto newNode = std::make_shared<FunctionNode>(ExprType::Exp, expr.clone());
    return Expression(newNode);
}

Expression ln(const Expression &expr)
{
    auto newNode = std::make_shared<FunctionNode>(ExprType::Ln, expr.clone());
    return Expression(newNode);
}

int main()
{

    std::string s = "x";
    Expression b("x");
    Expression a(5);
    auto c = sin(a + b/a^a);
    std::cout << a.to_string() << '\n';
    std::cout << b.to_string() << '\n';
    std::cout << c.to_string() << '\n';
}