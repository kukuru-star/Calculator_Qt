#pragma once
#include <qstring.h>


enum class Operator {
    Add,
    Sub,
    Mul,
    Div,
    Nan,
};

enum class InputState {
    Ope,
    Eql,
    Nan,
};

struct CalculatorState {
    double left = 0;
    double right = 0;
    Operator op = Operator::Nan;
    InputState input = InputState::Nan;
    bool isDot = false;
    QString display = "0";
    QString leftDisplay = "";
    QString errorMsg = "";
};
