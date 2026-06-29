#include "domain/Calculator.h"
#include "application/CalculatorState.h"

void CalculatorState::inputNumber(const QString& text)
{
    // = の後はリセット
    if (input == InputState::Eql) {
        left = 0;
        right = 0;
        inputBuffer = "0";
        isDot = false;
        op = Operator::Nan;
    }

    // 演算子直後
    if (input == InputState::Ope) {
        inputBuffer = "0";
        isDot = false;
    }

    appendNumber(text);
    currentValue = inputBuffer.toDouble();
    right = currentValue;
    input = InputState::Nan;
}

void CalculatorState::inputOpe(Operator opTemp)
{
    //演算子の連続計算
    if (op != Operator::Nan && input == InputState::Nan) {
        //何かしらの演算子あり　&　前の処理が数字の時
        inputEql();
    }

    left = currentValue;
    op = opTemp;
    input = InputState::Ope;
}

void CalculatorState::clearAll() {
    *this = CalculatorState();
}

void CalculatorState::clearEntry() {
    // = 直後の時
    if (input == InputState::Eql) {
        clearAll();
        return;
    }

    //演算子が残っているとき
    if (op != Operator::Nan) {
        inputBuffer = "0";
        currentValue = 0;
        isDot = false;
        return;
    }

    clearAll();

}

void CalculatorState::deleteLast() {
    // = の後
    if (input == InputState::Eql) {
        return;
    }

    // 演算子直後は何もしない or 0に戻す
    if (input == InputState::Ope) {
        return;
    }

    // 1文字削除（inputBuffer基準）
    if (inputBuffer.length() <= 1) {
        inputBuffer = "0";
        isDot = false;
    }
    else {
        inputBuffer.chop(1);
        if (!inputBuffer.contains(".")) {
            isDot = false;
        }
    }

    currentValue = inputBuffer.toDouble();
}

void CalculatorState::inputEql() {
    if (input != InputState::Eql) {
        right = currentValue;
    }
    else {
        left = currentValue;
    }
    //計算
    double result = compute();

    //エラー処理
    if (isError) {
        left = 0;
    }
    else {
        //表示用文字列生成
        if (op != Operator::Nan) {
            currentValue = result;
            inputBuffer = QString::number(currentValue);
        }
    }

    //状態更新
    input = InputState::Eql;

}

void CalculatorState::appendNumber(const QString& input)
{
    // "." 入力
    if (input == ".") {
        if (!isDot) {
            isDot = true;
            inputBuffer += ".";
        }
        return;
    }

    // "00" 入力
    if (input == "00") {
        if (inputBuffer != "0") {
            inputBuffer += "00";
        }
        return;
    }

    // 通常数字
    if (inputBuffer == "0") {
        inputBuffer = input;
    }
    else {
        inputBuffer += input;
    }
}

bool CalculatorState::hasError() const {
    return isError;
}

double CalculatorState::compute()
{
    if (op == Operator::Div && currentValue == 0) {
        isError = true;
        return 0;
    }
    return Calculator::calc(left, right, op);
}

QString CalculatorState::getCurrentInput() const {
    return inputBuffer;
}

double CalculatorState::getLeftInput() const {
    return left;
}

//使っていないか
double CalculatorState::getRightInput() const {
    return right;
}
