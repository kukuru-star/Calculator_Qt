#include "MainWindow.h"
#include "Calculator.h"
#include "CalculatorState.h"
#include <QLocale>
#include <qregularexpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    QList<QPushButton*> buttons = findChildren<QPushButton*>();

    for (auto btn : buttons) {
        connect(btn, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
    }

}

MainWindow::~MainWindow()
{}

//画面描画処理
void MainWindow::updateView()
{
    ui.lblDisplay->setText(m_state.display);
    ui.lblLeftDisplay->setText(m_state.leftDisplay);

    if (!m_state.errorMsg.isEmpty()) {
        ui.lblDisplay->setText(m_state.errorMsg);
    }
    updateButtons();
}

//エラー状態ボタン非活性
void MainWindow::updateButtons()
{
    bool isError = !m_state.errorMsg.isEmpty();

    for (auto btn : findChildren<QPushButton*>()) {
        if (btn == ui.btnClearEntry ||
            btn == ui.btnClear ||
            btn == ui.btnDel)
        {
            btn->setEnabled(true); // 常に有効
        }
        else {
            btn->setEnabled(!isError); // エラー時は無効
        }
    }
}


//値の正確性doubleでは不正確
//整数3桁区切りを付ける
//5×6×3
void MainWindow::onButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString text = btn->text();

    if (text.size() == 1 && text[0].unicode() == toSymbolEql(InputState::Eql)) {
        handleEql();
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Add)) {
        handleOpe(Operator::Add);
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Sub)) {
        handleOpe(Operator::Sub);
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Mul)) {
        handleOpe(Operator::Mul);
    }
    else if (text.size() == 1 && text[0].unicode() == toSymbol(Operator::Div)) {
        handleOpe(Operator::Div);
    }
    else if (text == "C") {
        //全て消す
        handleC();
    }
    else if (text == "CE") {
        //現在入力中だけ消す
        handleCE();
    }
    else if (text == "DEL") {
        handleDel();
    }
    else {
        //数字入力
        handleNum(text);
    }
}

//イコール処理
void MainWindow::handleEql() {
    //右辺確定
    if (m_state.input != InputState::Eql) {
        m_state.right = parseDisplay(m_state.display);
    }

    //計算
    double result = 0;
    bool isError = false;

    if (m_state.op == Operator::Div && m_state.right == 0) {
        m_state.errorMsg = "------";
        isError = true;
    }
    else if (m_state.op != Operator::Nan) {
        result = Calculator::calc(m_state.left, m_state.right, m_state.op);
    }

    //エラー処理
    if (isError) {
        m_state.display = m_state.errorMsg;
        m_state.leftDisplay.clear();
        m_state.left = 0;
    }
    else {
        //表示用文字列生成
        if (m_state.op != Operator::Nan) {
            m_state.leftDisplay =
                displayFormat(m_state.left) +
                toSymbol(m_state.op) +
                displayFormat(m_state.right) +
                toSymbolEql(InputState::Eql);

            m_state.display = displayFormat(result);
            m_state.left = result;
        }
        else {
            m_state.leftDisplay =
                displayFormat(m_state.right) +
                toSymbolEql(InputState::Eql);
        }
    }

    //状態更新
    m_state.input = InputState::Eql;

    //UI反映
    updateView();
}

void MainWindow::handleOpe(Operator op) {
    m_state.left = parseDisplay(m_state.display);
    m_state.op = op;

    m_state.leftDisplay = m_state.display + toSymbol(op);
    m_state.input = InputState::Ope;

    updateView();
}

void MainWindow::handleC() {
    if (m_state.errorMsg.isEmpty() == false) {
        resetUiState();;
    }
    m_state = CalculatorState();
    updateView();
}

void MainWindow::handleCE() {
    if (m_state.errorMsg.isEmpty() == false) {
        m_state.errorMsg.clear();
        resetUiState();
        updateView();
        return;
    }

    if (m_state.input == InputState::Ope) {
        //現状維持
        m_state.display = "0";
    }
    else {
        m_state.display = "0";

        m_state.left = 0;
        m_state.leftDisplay = "";
        m_state.op = Operator::Nan;
        m_state.input = InputState::Nan;
        m_state.isDot = false;
    }
    updateView();
}

void MainWindow::handleDel() {
    if (m_state.errorMsg.isEmpty() == false) {
        m_state.display = "0";
        m_state.errorMsg.clear();
        resetUiState();
        updateView();
        return;
    }

    // = の後なら左表示だけ消す
    if (m_state.input == InputState::Eql) {
        m_state.leftDisplay.clear();
        //m_state.input = InputState::Nan;
        updateView();
        return;
    }

    //現在の表示を一文字削除
    m_state.display = deleteLastChar(m_state.display);

    //DEL後ドットが含まれているか
    m_state.isDot = m_state.display.contains(".");
    updateView();
}

void MainWindow::handleNum(QString text)
{
    //= の後は新規入力
    if (m_state.input == InputState::Eql) {
        m_state.left = 0;
        m_state.right = 0;
        m_state.leftDisplay.clear();
        m_state.display = "0";
    }

    //演算子直後は新規入力
    if (m_state.input == InputState::Ope) {
        m_state.display = "0";
    }

    //数字組み立て
    m_state.display = appendNumber(m_state.display, text);

    m_state.input = InputState::Nan;

    updateView();
}

//末尾の1文字削除
QString MainWindow::deleteLastChar(const QString& text) {
    if (text.length() <= 1) {
        return "0";
    }
    return text.chopped(1);
}

//画面初期化処理
void MainWindow::resetUiState() {
    for (auto btn : findChildren<QPushButton*>()) {
        btn->setEnabled(true);
    }
}

QString MainWindow::appendNumber(QString current, QString input)
{
    // "." 入力
    if (input == ".") {
        if (current.contains(".")) {
            return current; // 2回目のdotは無視
        }
        return current + ".";
    }

    // "00" 入力
    if (input == "00") {
        if (current == "0") {
            return "0";
        }
        return current + "00";
    }

    // 通常数字
    if (current == "0") {
        return input; // 先頭0置き換え
    }

    return current + input;
}

//演算子変換
QString MainWindow::toSymbol(Operator op) {
    switch (op) {
    case Operator::Add: return QChar(0xFF0B); // ＋
    case Operator::Sub: return QChar(0xFF0D); // －
    case Operator::Mul: return QChar(0x00D7); // ×
    case Operator::Div: return QChar(0x00F7); // ÷
    default: return "";
    }
}

QString MainWindow::toSymbolEql(InputState state) {
    switch (state) {
    case InputState::Eql: return QChar(0xFF1D); // ＝
    default: return "";
    }
}

//画面表示用三桁区切り変換
QString MainWindow::displayFormat(double value)
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
double MainWindow::parseDisplay(const QString& text) {
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




