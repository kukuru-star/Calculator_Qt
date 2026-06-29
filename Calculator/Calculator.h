#pragma once
#include "MainWindow.h"
#include "CalculatorState.h"

class Calculator {
public:
	static double calc(double left, double right, Operator op);
};

double Calculator::calc(double left, double right, Operator op)
{
    switch (op) {
    case Operator::Add: return left + right;
    case Operator::Sub: return left - right;
    case Operator::Mul: return left * right;
    case Operator::Div:
        if (right == 0) throw std::runtime_error("div0");
        return left / right;
    default: return 0;
    }
}