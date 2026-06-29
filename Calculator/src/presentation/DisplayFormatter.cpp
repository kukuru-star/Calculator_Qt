#include "presentation/DisplayFormatter.h"

QString DisplayFormatter::makeLeftDisplay(const CalculatorState& s)
{
    if (s.hasError()) return "";

    if (s.input == InputState::Eql) {
        if (s.op == Operator::Nan) {
            return QString::number(s.right) +
                "=";
        }
        else {
            return QString::number(s.left) +
                toSymbol(s.op) +
                QString::number(s.right) +
                "=";
        }
    }

    if (s.op != Operator::Nan) {
        return QString::number(s.getLeftInput()) + toSymbol(s.op);
    }
    return "";
}

QString DisplayFormatter::makeDisplay(const CalculatorState& s)
{
    if (s.hasError()) return "-----";

    if (s.input == InputState::Eql) {
        return formatNumber(s.currentValue);
    }
    return s.getCurrentInput();
}

QString DisplayFormatter::toSymbol(Operator op)
{
    switch (op) {
    case Operator::Add: return QChar(0xFF0B);
    case Operator::Sub: return QChar(0xFF0D);
    case Operator::Mul: return QChar(0x00D7);
    case Operator::Div: return QChar(0x00F7);
    default: return "";
    }
}

QString DisplayFormatter::formatNumber(double value)
{
    QLocale locale(QLocale::English);
    QString normal = locale.toString(value, 'f', 10);

    // •s—v‚È0íœ
    normal.remove(QRegularExpression("\\.?0+$"));

    QString raw = normal;
    raw.remove(",");

    if (raw.length() > 10) {
        QString exp = QString::number(value, 'e', 6);
        exp.replace(QRegularExpression("e\\+?0*"), "e");
        return exp;
    }

    return normal;
}