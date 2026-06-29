#pragma once
#include <qregularexpression>
#include <QLocale>
#include <QString>

#include "domain/Operator.h"

enum class InputState {
    Ope,
    Eql,
    Nan,
};

struct CalculatorState {
    //現在入力中の値
    double currentValue = 0;
    QString inputBuffer = "0";
    //確定済みの左辺
    double left = 0;
    //確定済みの右辺
    double right = 0;
    Operator op = Operator::Nan;
    InputState input = InputState::Nan;
    bool isDot = false;
    bool isError = false;

    void inputNumber(const QString& text);
    void appendNumber(const QString& input);

    void inputOpe(Operator op);

    void clearAll();

    void clearEntry();

    void deleteLast();

    void inputEql();

    bool hasError()const;

    double compute();

    QString getCurrentInput() const;

    double getLeftInput() const;

    double getRightInput() const;
};

