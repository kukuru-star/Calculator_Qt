# Calculator

Qt6 / C++ を用いて作成したシンプルな電卓アプリです。

本バージョンでは、初版で一つのクラスに集約していた処理を見直し、役割ごとにクラスを分割するリファクタリングを実施しました。

---

## 動作環境

- Windows 10 / 11
- Qt 6
- Visual Studio 2022
- C++17

---

## 主な機能

- 四則演算
- 小数入力
- CE
- C
- DEL
- 桁区切り表示
- 0 除算時のエラー表示

---

## 使用技術

- C++
- Qt6 Widgets
- Visual Studio 2022
- Git / GitHub

---

## プロジェクト構成

```
Calculator
├── Calculator.h
├── CalculatorState.h
├── Main.cpp
├── MainWindow.cpp
├── MainWindow.h
└── MainWindow.ui
```

---

## このバージョンで改善した点

- 演算処理を `Calculator` クラスへ分離
- 計算状態を `CalculatorState` に集約
- ボタン処理を `handleEql()`、`handleOpe()`、`handleNum()` などの関数へ分割
- 画面更新処理を `updateView()` に集約
- ボタンの有効／無効制御を `updateButtons()` に分離
- 各クラス・各関数の責務を明確化し、保守性・可読性を向上

---

## 今後の改善予定

- 表示処理を `DisplayFormatter` クラスへ分離
- フォルダ構成の整理
- 状態管理の改善

---

## スクリーンショット

![alt text](image.png)
