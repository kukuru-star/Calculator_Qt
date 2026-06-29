#include "Calculator.h"
#include <QLocale>
#include <qregularexpression>

void Calculator::initSignal()
{
    QList<QPushButton*> buttons = findChildren<QPushButton*>();

    for (auto btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    }
}


//例えば、下の処理を行えるように変更することを想定
// 5×6×3
// 
//改善版のソフトでは、CalculatorState.cppに下記を追加のみ
//if (op != Operator::Nan && input == InputState::Nan) {
//    //何かしらの演算子あり　&　前の処理が数字の時
//    inputEql();
//}
//
//しかし、こちらだと、各演算子処理の下に = の処理を記載しないといけない...

void Calculator::onButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString text = btn->text();

    //qDebug() << "[" << text << "]";
    //qDebug() << text.length();
    //for (auto c : text) {
    //    qDebug() << c.unicode();
    //}

    if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Eql)) {
        double result = 0;

        //演算子の右側の値
        if (m_isNewInput == Operator::Add ||
            m_isNewInput == Operator::Sub ||
            m_isNewInput == Operator::Mul ||
            m_isNewInput == Operator::Div ||
            m_isNewInput == Operator::Nan)
        {
            m_rightValue = parseDisplay(ui.lblDisplay->text());
        }
        else if(m_isNewInput == Operator::Eql) {
            //処理無し
        }

        //演算結果
        if (m_operator == Operator::Add) {
            result = m_leftValue + m_rightValue;
        }
        else if (m_operator == Operator::Sub) {
            result = m_leftValue - m_rightValue;
        }
        else if (m_operator == Operator::Mul) {
            result = m_leftValue * m_rightValue;
        }
        else if (m_operator == Operator::Div) {
            if (m_leftValue == 0 && m_rightValue == 0) {
                //m_errorMsg = QStringLiteral("結果が定義されていません。");
                m_errorMsg = QStringLiteral("------");
            }
            else if (m_rightValue == 0) {
                //m_errorMsg = QStringLiteral("0で割ることはできません。");
                m_errorMsg = QStringLiteral("------");
            }
            else {
                result = m_leftValue / m_rightValue;
            }
        }

        //エラーの有無
        if (m_errorMsg.isEmpty()) {
            //演算処理の有無
            if (m_operator == Operator::Add ||
                m_operator == Operator::Sub ||
                m_operator == Operator::Mul ||
                m_operator == Operator::Div)
            {
                ui.lblLeftDisplay->setText(displayFormat(m_leftValue) +
                    toSymbol(m_operator) + displayFormat(m_rightValue) +
                    toSymbol(Operator::Eql));

                ui.lblDisplay->setText(displayFormat(result));

                m_leftValue = result;
            }
            else {
                ui.lblLeftDisplay->setText(displayFormat(m_rightValue) + toSymbol(Operator::Eql));
            }
        }
        else {
            ui.lblLeftDisplay->setText("");
            ui.lblDisplay->setText(m_errorMsg);

            m_leftValue = 0;

            for (auto btn : findChildren<QPushButton*>()) {
                if (btn == ui.btnClearEntry ||
                    btn == ui.btnClear ||
                    btn == ui.btnDel) {
                    //処理しない
                }
                else {
                    btn->setEnabled(false);
                }
            }
        }
     
        m_isNewInput = Operator::Eql;
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Add)) {
        m_leftValue = parseDisplay(ui.lblDisplay->text());
        ui.lblLeftDisplay->setText(ui.lblDisplay->text() + toSymbol(Operator::Add));

        m_operator = Operator::Add;
        m_isNewInput = Operator::Add;
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Sub)) {
        m_leftValue = parseDisplay(ui.lblDisplay->text());
        ui.lblLeftDisplay->setText(ui.lblDisplay->text() + toSymbol(Operator::Sub));

        m_operator = Operator::Sub;
        m_isNewInput = Operator::Sub;
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Mul)) {
        m_leftValue = parseDisplay(ui.lblDisplay->text());
        ui.lblLeftDisplay->setText(ui.lblDisplay->text() + toSymbol(Operator::Mul));

        m_operator = Operator::Mul;
        m_isNewInput = Operator::Mul;
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Div)) {
        m_leftValue = parseDisplay(ui.lblDisplay->text());
        ui.lblLeftDisplay->setText(ui.lblDisplay->text() + toSymbol(Operator::Div));

        m_operator = Operator::Div;
        m_isNewInput = Operator::Div;
    }
    else if (text == "C") {
        //全て消す
        ui.lblDisplay->setText("0");
        ui.lblLeftDisplay->setText("");

        m_leftValue = 0;
        m_rightValue = 0;
        m_operator = Operator::Nan;
        m_isNewInput = Operator::Nan;
        m_isDot = false;

        if (m_errorMsg.isEmpty() == false) {
            resetUiState();
        }
    }
    else if (text == "CE") {
        //現在入力中だけ消す
        if (m_isNewInput == Operator::Add ||
            m_isNewInput == Operator::Sub ||
            m_isNewInput == Operator::Mul ||
            m_isNewInput == Operator::Div) {
            //現状維持
        }
        else {
            m_leftValue = 0;
            ui.lblLeftDisplay->setText("");
            m_operator == Operator::Nan;
            m_isNewInput = Operator::Nan;
            m_isDot = false;
        }
        ui.lblDisplay->setText("0");
        if (m_errorMsg.isEmpty() == false) {
            resetUiState();
        }
    }
    else if (text == "DEL") {
        if (m_isNewInput == Operator::Nan) {
            if (ui.lblDisplay->text().length() == 0 || ui.lblDisplay->text().length() == 1) {
                ui.lblDisplay->setText("0");
            }
            else {
                ui.lblDisplay->setText(ui.lblDisplay->text().chopped(1));
            }
        }
        else if (m_isNewInput == Operator::Eql) {
            ui.lblLeftDisplay->setText("");
        }
        if (m_errorMsg.isEmpty() == false) {
            resetUiState();
        }

        //DEL後ドットが含まれているか
        if (ui.lblDisplay->text().contains(".")) {
            m_isDot = true;
        }
        else {
            m_isDot = false;
        }
    }
    else {
        //前の入力が数字 or = or それ以外
        if (m_isNewInput == Operator::Nan) {
            if (parseDisplay(ui.lblDisplay->text()) == 0) {
                if (text == ".") {
                    ui.lblDisplay->setText("0.");
                    m_isDot = true;
                }
                else if (ui.lblDisplay->text().contains(".")) {
                    ui.lblDisplay->setText(ui.lblDisplay->text() + text);
                }
                else {
                    if (text == "00") {
                        ui.lblDisplay->setText("0");
                    }
                    else {
                        ui.lblDisplay->setText(text);
                    }
                }
            }
            else {
                if (m_isDot == false || text != ".") {
                    ui.lblDisplay->setText(ui.lblDisplay->text() + text);
                }
                if (text == ".") {
                    m_isDot = true;
                }
            }
        }
        else if (m_isNewInput == Operator::Eql) {
            m_leftValue = 0;
            m_rightValue = 0;
            m_isDot = false;
            ui.lblLeftDisplay->setText("");
            if (text == "00") {
                ui.lblDisplay->setText("0");
            }
            else if (text == ".") {
                ui.lblDisplay->setText("0.");
                m_isDot = true;
            }
            else {
                ui.lblDisplay->setText(text);
            }
        }
        else {
            if (text == "00") {
                ui.lblDisplay->setText("0");
            }
            else if (text == ".") {
                ui.lblDisplay->setText("0.");
                m_isDot = true;
            }
            else {
                ui.lblDisplay->setText(text);
            }
        }
        m_isNewInput = Operator::Nan;

    }
}

//画面初期化処理
void Calculator::resetUiState() {
    ui.lblDisplay->setText("0");
    m_errorMsg.clear();
    for (auto btn : findChildren<QPushButton*>()) {
        btn->setEnabled(true);
    }
}

QString Calculator::toSymbol(Operator op) {
    switch (op) {
    case Operator::Add: return QChar(0xFF0B); // ＋
    case Operator::Sub: return QChar(0xFF0D); // －
    case Operator::Mul: return QChar(0x00D7); // ×
    case Operator::Div: return QChar(0x00F7); // ÷
    case Operator::Eql: return QChar(0xFF1D); // ＝
    default: return "";
    }
}

//画面表示用三桁区切り変換
QString Calculator::displayFormat(double value)
{
    QLocale locale(QLocale::English);
    QString normal = locale.toString(value, 'f', 10);

    // 不要な0削除
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

//計算可能な値に変換
double Calculator::parseDisplay(const QString& text) {
    QString raw = text;
    raw.remove(",");

    bool ok = false;
    double value = raw.toDouble(&ok);

    if (!ok) {
        // 不正入力対策（例：0にする or 前回値維持）
        return 0.0;
    }

    return value;
}