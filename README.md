# Calculator

Qt6 / C++ を用いて作成したシンプルな電卓アプリです。

C++および Qt の学習を目的として作成した最初のバージョンであり、基本的な四則演算機能を実装しています。

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
├── Calculator.cpp
├── Calculator.h
├── Calculator.ui
├── InitData.cpp
└── Main.cpp
```

---

## このバージョンの特徴

- まずは動作することを重視して実装
- ボタンイベントや演算処理を `Calculator` クラスに集約
- UI イベントから直接演算を実行する構成

---

## 今後の改善予定

- 演算処理を別クラスへ分離
- 状態管理クラスの作成
- 表示フォーマット処理の分離
- クラスごとの責務を整理し、保守性を向上

---

## スクリーンショット

![alt text](image.png)
