#pragma once
#include "application/CalculatorState.h"

class DisplayFormatter {
public:
    static QString makeLeftDisplay(const CalculatorState& s);

    static QString makeDisplay(const CalculatorState& s);

    static QString toSymbol(Operator op);

    static QString formatNumber(double value);

};