# `CAuthPage`

## 構成

ソースコード上は,
[`AuthPage.h`](../ChkMails/ChkMails/AuthPage.h) と
[`AuthPage.cpp`](../ChkMails/ChkMails/AuthPage.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CAuthPage`](#cauthpage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)


## 概要

[Authentication](../README.md#authentication) の設定を行うための「Page」です.<br>
[`CProperSheet`](CProperSheet.md) に乗っかった
[`CFilterSheet`](CFilterSheet.md) が
束ねている page のうちの 1つです.

![](../Examples/Filter.Authentication.png)

これまでの運用で, メールサーバーからどの `Authentication-Results:` が来た実績があるのかを,
[Main Window](CMainWnd.md) から bitmap 値でもらい, その bitmap 値に応じて各 authentication の有無を表現しています.

設定値としては「いくつ以上の authentication が pass しなかったらメールを破棄するか」
という解りづらい値が 1つあるだけです.
[`CComboBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ccombobox-class)
で表現と選択を行っています.

以下, 本 class 内に実装された関数それぞれの説明です.

## `CAuthPage`

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

1. Combo Box の「設定値番目」を選ぶ.
1. 「Authentication 実績 bitmap」の bit が立っていない項目の表示を disable にする.
1. 特に入力フォーカスをどこかに設定していないので, `TRUE` を返す.

妙に簡単にやっていますが,

* `0` も含めて「Combo Box の何番目の選択肢か」が, 設定値と一致している
* 項目名の `static` の ID の順序が bitmap の bit 位置と一致している

という風に仕向けられているからです.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
の override です.

基底 class の実装である
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
も呼ばずに,
「Combo Box の何番目の選択肢が選ばれているか」
を「設定値」として格納するだけです.

この `public` な「設定値」は `OnOK` の後,
[Main Window](CMainWnd.md) が勝手に回収していく手筈になっています.

「Authentication 実績 bitmap」は上からもらうだけで当 class では変更しない一方通行の値なので,
特に何もしません.
[`OnInitDialog`](#oninitdialog) で項目の enable / disable に使われているだけです.