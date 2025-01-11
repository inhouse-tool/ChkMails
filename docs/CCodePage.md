# `CCodePage`

## 構成

ソースコード上は,
[`CodePage.h`](../ChkMails/ChkMails/CodePage.h) と
[`CodePage.cpp`](../ChkMails/ChkMails/CodePage.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CCodePage`](#ccodepage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)


## 概要

[Coding](../README.md#coding) の設定を行うための「Page」です.<br>
[`CProperSheet`](CProperSheet.md) に乗っかった
[`CFilterSheet`](CFilterSheet.md) が
束ねている page のうちの 1つです.

![](../Examples/Filter.Coding.png)

チェックボックスの ON/OFF 具合を,
bitmap 値として [Main Window](CMainWnd.md) とやり取りしています.

以下, 本 class 内に実装された関数それぞれの説明です.


## `CCodePage`

この class の constructor です.

リソース ID を指定して基底クラスの constructor を呼ぶという大事な仕事を果たした後,
メンバー変数を初期化しています.
<sub>
そうしておかないと[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)がうるさいので.
</sub>


## `OnInitDialog`

[`CPropertyPage::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
の override です.

まずは普通に
`CPropertyPage::OnInitDialog` を呼んだ後,
( `CPropertyPage` は `OnInitDialog` を override していないようなので,
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
にスルーパスされた後, )
 以下の独自処理を挟みます.

1. 「&#x2611;された項目 bitmap」の bit が立っている Check Box を&#x2611;にする.
1. 特に入力フォーカスをどこかに設定していないので, `TRUE` を返す.

妙に簡単にやっていますが,

* Check Box の ID の順序が bitmap の bit 位置と一致している

という風に仕向けられているからです.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
の override です.

基底 class の実装である
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
も呼ばずに,
「どの項目が&#x2611;されているかの bitmap」
を「設定値」として格納するだけです.

この `public` な「設定値」は `OnOK` の後,
[Main Window](CMainWnd.md) が勝手に回収していく手筈になっています.

