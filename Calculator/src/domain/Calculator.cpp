#include "domain/Calculator.h"
#include <stdexcept>

double Calculator::calc(double left, double right, Operator op)
{
    switch (op) {
    case Operator::Add: return left + right;
    case Operator::Sub: return left - right;
    case Operator::Mul: return left * right;
    case Operator::Div: return left / right;
    default: return 0;
    }
}