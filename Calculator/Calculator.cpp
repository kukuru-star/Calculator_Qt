#include "Calculator.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    initSignal();

    toSymbol(m_operator);

}

Calculator::~Calculator()
{}



