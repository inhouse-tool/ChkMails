# `CWordPage`

## 構成

ソースコード上は,
[`WordPage.h`](../ChkMails/ChkMails/WordPage.h) と
[`WordPage.cpp`](../ChkMails/ChkMails/WordPage.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CWordPage`](#cwordpage-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`OnOK`](#onok)
[`PreTranslateMessage`](#pretranslatemessage)

#### コマンドハンドラー

[`OnButtonAdd`](#onbuttonadd)
[`OnButtonDelete`](#onbuttondelete)
[`OnChangeEdit`](#onchangeedit)
[`OnChangeList`](#onchangelist)

#### 固有関数

[`SortWord`](#sortname)


## 概要

[Word](../README.md#word) の設定を行うための「Page」です.<br>
[`CProperSheet`](CProperSheet.md) に乗っかった
[`CFilterSheet`](CFilterSheet.md) が
束ねている page のうちの 1つです.

![](../pics/Filter.Word4.png)

「検閲」する単語を入力して,
`Add` ボタンでリストに登録します.
<br>
<sup>
( ここでは「単語」と呼んでいますが, 上記の例のように空白を含む文字列でも構いません. )
</sup>

以下, 本 class 内に実装された関数それぞれの説明です.


## `CWordPage`

この class の constructor です.

ソースコードを見ると, 何もやっていないように見えて「要るの？これ。」という感じですが,
リソース ID を指定して基底クラスの constructor を呼ぶという大事な仕事をしています.


## `OnInitDialog`

[`CPropertyPage::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
の override です.

まずは普通に
`CPropertyPage::OnInitDialog` を呼んだ後,
( `CPropertyPage` は `OnInitDialog` を override していないようなので,
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
にスルーパスされた後, )
 以下の独自処理を挟みます.

1. [`GetCommonFont`](#CChkMailsApp.md#getcommonfont) で共通の等幅フォントを得る.
1. Edit Box に等幅フォントをセットする.
1. List Control に等幅フォントをセットする.
1. List Control を全カラムで選択表示するように設定.
1. List Control をグリッド表示するように設定.
1. List Control に 2カラム追加.
1. [`SortWord`](#sortname) で単語配列をソート.
1. 「単語配列」をList Control に追加.
1. 印の付いていた文字列なら List Control を☑.
1. List Control のカラム幅を自動調整.
1. フォーカスを設定していないので, `TRUE` を返す.


## `OnOK`

[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
の override です.

基底 class の実装である
[`CPropertyPage::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class#onok)
も呼ばずに, 以下の処理だけ行います.

* List Control にある文字列を「単語配列」に格納.
* ☑されていた単語は文字列の末尾に印を追加.

この `public` な配列は `OnOK` の後,
[Main Window](CMainWnd.md) が勝手に回収していく手筈になっています.


## `PreTranslateMessage`

`CProperSheet::PreTranslateMessage` の override です.

`WM_KEYDOWN` が来たとき, `New word:` の Edit Box に入力フォーカスがあって入力キーが `Enter` だったら,
その事象を「`Add` ボタンが押された」にすり替えて
[`OnButtonAdd`](#onbuttonadd) が呼ばれるように仕向けています.

せっかく Edit Box に何か入れたのに,
`Add` ボタンを押さずにイキオイで `Enter` キーを叩いちゃううっかりさんって, 結構いらっしゃるようです.
しかし page 上で `Enter` キーを叩くと `OK` ボタンが押されたことになっちゃうので,
せっかくの入力がパーです.
ということで, その救済策として挟んでいる心遣いです.


## `OnButtonAdd`

`Add` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

`New word:` の Edit Box に入力された文字列を List Control に追加します.

ただし, すでに List Control 内に全く同じ文字列がエントリー済みの場合は,
追加はしません.
同じものを複数登録しても意味がないので.

`Add` 直後の文字列は, List Control 上では「選択されている」状態になっています.
「間違えた」と思ったらすかさず `Delete` ボタンを押しましょう.
今 `Add` したばかりの文字列が登録抹消されます.

Edit Box への入力を終えた時点で, ついイキオイで `Enter` キーを叩いてしまったときは,
[`PreTranslateMessage`](#pretranslate) の心遣いで当関数が呼ばれています.


## `OnButtonDelete`

`Delete` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

今 List Control 上で選択されている行のエントリーを削除します.

この「選択されている」を常に表現するため,
List Control の元ネタになっているリソース上では,
`Always Show Selection` が `True` に設定されています.
このお陰で List Control に入力フォーカスが当たっていない状況でも,
それなりに「選択されている」行がアピールされます.


## `OnChangeEdit`

Edit Box で文字列が変更された時に飛んでくる `EN_CHANGE` コマンドのハンドラーです.

* `New word:` の文字列が空なら `Add` ボタンを disable.
* `New word:` の文字列が空でなければ `Add` ボタンを enable.

としています.


## `OnChangeList`

List Control 上の選択が変えられた時に飛んでくる `LVN_ITEMCHANGED` コマンドのハンドラーです.

下記のように状態変化を整理した上で対応しています.

| チェック | 意味 | 対応 |
| --- | --- | --- |
| `-1` | 変化なし | 何もしない |
| `0` | &#x2611;されていたものから&#x2611;が外れた | &#x2611; を OFF する. |
| `1` | &#x2611;されていなかったものが&#x2611;された | &#x2611; を ON する. |

| 選択状態 | 意味 | 対応 |
| --- | --- | --- |
| `-1` | 変化なし | 何もしない |
| `0` | 選択されていたものから選択が外れた | `Delete` ボタンを disable. |
| `1` | 選択されていなかったものが選択された | `Delete` ボタンを enable. |

選択状態への対応は最後の状態にだけ対応すれば良いので少々冗長と言えますが,
チェックへの対応は両方の状態変化に対応する必要があるので,
上記のように対応しています.


## `SortWord`

文字列をソートして `CStringArray` に格納します.

`\n` 区切りで `CString` に収められたドメイン文字列をソートして `CStringArray` に格納します.

設定値を registry に格納する際は 1本の文字列だと都合が良いのですが,
List に並べて扱う時は配列の方が扱いやすいので,
こうして「文字列→配列」の処理を構えています.

[`OnInitDialog`](#oninitdialog) から呼ばれています.