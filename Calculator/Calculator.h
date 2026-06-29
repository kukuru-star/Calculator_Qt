#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Calculator.h"

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    enum class Operator {
        Add,
        Sub,
        Mul,
        Div,
        Eql,
        Nan,
    };

private:
    Ui::CalculatorClass ui;
    //m_operator‚ÉEql‚Í“ü‚ê‚È‚¢‚±‚Æ
    Operator m_operator = Operator::Nan;
    double m_leftValue = 0;
    double m_rightValue = 0;
    bool m_isDot = false;
    Operator m_isNewInput = Operator::Nan;
    QString m_errorMsg = "";
    void initSignal();
    void onButtonClicked();
    void resetUiState();
    QString toSymbol(Operator op);
    QString displayFormat(double result);
    double parseDisplay(const QString& text);


};

