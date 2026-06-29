#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "application/CalculatorState.h"

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
    void handleC();
    void handleCE();
    void handleDel();

    void resetUiState();
    void updateButtons();
    void updateView();
    QString toSymbolEql(InputState state);
};

