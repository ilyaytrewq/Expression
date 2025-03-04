#ifndef ExpreType_HPP
#define ExpreType_HPP

#include <string>

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

std::string exprTypeToString(ExprType type)
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

#endif //ExpreType_HPP