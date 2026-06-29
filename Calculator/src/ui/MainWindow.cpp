#include "ui/MainWindow.h"
#include "application/CalculatorState.h"
#include "presentation/DisplayFormatter.h"

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

//値の正確性doubleでは不正確
//5×6×3
void MainWindow::onButtonClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString text = btn->text();

    if (text.size() == 1 && text[0].unicode() == toSymbolEql(InputState::Eql)) {
        m_state.inputEql();
    }
    else if (text.size() == 1 && text[0].unicode() == DisplayFormatter::toSymbol(Operator::Add)) {
        m_state.inputOpe(Operator::Add);
    }
    else if (text.size() == 1 && text[0].unicode() == DisplayFormatter::toSymbol(Operator::Sub)) {
        m_state.inputOpe(Operator::Sub);
    }
    else if (text.size() == 1 && text[0].unicode() == DisplayFormatter::toSymbol(Operator::Mul)) {
        m_state.inputOpe(Operator::Mul);
    }
    else if (text.size() == 1 && text[0].unicode() == DisplayFormatter::toSymbol(Operator::Div)) {
        m_state.inputOpe(Operator::Div);
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
        m_state.inputNumber(text);
    }
    //UI反映
    updateView();
}


void MainWindow::handleC() {
    if (m_state.hasError()) {
        resetUiState();
    }
    m_state.clearAll();
}

void MainWindow::handleCE() {
    if (m_state.hasError()) {
        m_state.clearAll();
        resetUiState();
    }
    else {
        m_state.clearEntry();
    }
}

void MainWindow::handleDel() {
    if (m_state.hasError()) {
        m_state.clearAll();
        resetUiState();
    }
    else {
        m_state.deleteLast();
    }
}

//画面描画処理
void MainWindow::updateView()
{
    if (m_state.hasError()) {
        ui.lblDisplay->setText("-----");
    }
    else {
        ui.lblDisplay->setText(DisplayFormatter::makeDisplay(m_state));
    }

    ui.lblLeftDisplay->setText(DisplayFormatter::makeLeftDisplay(m_state));

    updateButtons();
}

QString MainWindow::toSymbolEql(InputState state) {
    switch (state) {
    case InputState::Eql: return QChar(0xFF1D); // ＝
    default: return "";
    }
}

//エラー状態ボタン非活性
void MainWindow::updateButtons()
{
    for (auto btn : findChildren<QPushButton*>()) {
        if (btn == ui.btnClearEntry ||
            btn == ui.btnClear ||
            btn == ui.btnDel)
        {
            btn->setEnabled(true); // 常に有効
        }
        else {
            btn->setEnabled(!m_state.hasError()); // エラー時は無効
        }
    }
}

//画面初期化処理
void MainWindow::resetUiState() {
    for (auto btn : findChildren<QPushButton*>()) {
        btn->setEnabled(true);
    }
}

