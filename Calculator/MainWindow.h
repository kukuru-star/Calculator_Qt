#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "CalculatorState.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    Ui::MainWindowClass ui;

    CalculatorState m_state;

    void onButtonClicked();
    void resetUiState();

    QString toSymbol(Operator op);
    QString toSymbolEql(InputState state);

    QString displayFormat(double result);
    double parseDisplay(const QString& text);
    QString deleteLastChar(const QString& text);
    void updateButtons();
    QString appendNumber(QString current,QString input);
    void updateView();

    void handleEql();
    void handleOpe(Operator op);
    void handleC();
    void handleCE();
    void handleDel();
    void handleNum(QString text);
};

