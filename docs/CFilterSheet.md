# `CFilterSheet`

## 構成

ソースコード上は,
[`FilterSheet.h`](../ChkMails/ChkMails/FilterSheet.h) と
[`FilterSheet.cpp`](../ChkMails/ChkMails/FilterSheet.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CFilterSheet`](#cfiltersheet-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`PreTranslateMessage`](#pretranslatemessage)
[`OnCommand`](#oncommand)

#### 固有関数

[`OnHelp`](#onhelp)
[`AddPages`](#addpages)


## 概要

[Filter](../README.md#filter) 配下の設定項目を束ねている「シート」です.

一般的な [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
の上に乗っかった実装のように見えますが,
ソースをよく見ると `CPropertySheet` ではなく [`CProperSheet`](CProperSheet.md) という,
何やらパチモンくさい class に乗っています.
この「パチモン」については[そちら](CProperSheet.md)に譲るとして,
この class がやっている独自要素としては:

* `Help` ボタンを追加している.
* `Help` ボタンを画面の左端に寄せている.
* `OK` ボタンを画面の右端に寄せている.
* `Cancel` ボタンは省略している.

といったところです.

あと, このシート上で `Ctrl` + `V` と押すと,
一般的なメニューの `Edit` (編集) カテゴリーの `Paste` (ペースト) コマンドに置き換えて,
今選ばれている page に「おい、Paste だってよ。」と伝えます.
もっとも「Paste ですね。わっかりましたー!」という反応を見せてくれるのは,
[Whitelist](../README.md#whitelist) の
[page](CWhitePage.md)だけで, 他の page では無視されていますが.

このシートを `OK` ボタンで閉じると,
選ばれていた page を記憶しておきます.
同じ page の設定を試行錯誤で繰り返すことが多いので.

以下, 本 class 内に実装された関数それぞれの説明です.


## CFilterSheet

この class の constructor です.

基底 class [`CProperSheet`](CProperSheet.md) の constructor を呼んだ後,
[`AddPages`](#addpages) を呼んでいるだけです.


## OnInitDialog

[`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog)
の override です.

処理内容は以下の通りです.

1. 基底 class の [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) を呼ぶ.
1. 前回 `OK` ボタンが押された page を
[`SetActivePage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#setactivepage).
1. `OK` ボタンを `Help` ボタンの位置に移動.
1. `Help` ボタンをシートの左隅に移動.
1. `Help` ボタンの ID を `IDHELP` から `ID_HELP` に書き換えてイネーブル.
1. `Cancel` ボタンは隠しておく.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでシートが現われたことを通知.
1. [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) の戻り値を返す.

としています.

[`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) の戻り値にかかわらず,
上記 2.～6. の処理を進めてしまっているところが,
ぱっと見「基底 class の戻り値を無視して進めてしまっていいのか?」という印象を与えますが,
この戻り値は

* `TRUE`: 入力フォーカスをどこに設定するかは「おまかせ」で.
* `FALSE`: 入力フォーカスはこっちで設定したから, 勝手に変えないでね.

という程度のものなので,
`OnInitDialog` が失敗したにもかかわらず突っ走っているわけではありません.

あと, 5. でやっている ID の書き換えですが,
`IDHELP` のままにしておくと
[ものすごく面倒なしきたりに巻き込まれる](https://jeffpar.github.io/kbarchive/kb/132/Q132715/)からです.
当 class では `ID_HELP` に書き換えて, この呪縛から逃れています.
おかげですなおな処理になりました.
<br>
<sub>
( [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
の設計には納得がいきません.
自動でいろいろ処理してくれるのはありがたいですが,
妙なルールを押し付けないでいただきたい. )
</sub>

## `DestroyWindow`

`CProperSheet::DestroyWindow` の override です.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでシートが引っ込んだことを通知する,
という手順を挟むためだけに override しています.

ちなみに,
[`CProperSheet`](CProperSheet.md) は `DestroyWindow` を override しておらず,
`CPropertySheet::DestroyWindow` にスルーパスです.
( が, [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class) も
`DestroyWindow` を override していないようなので,
`CWnd::DestroyWindow` にスルーパスしているようです. )


## `PreTranslateMessage`

`CProperSheet::PreTranslateMessage` の override です.

`WM_KEYDOWN` が来たとき,
`Ctrl` + `V` だったら「いま選ばれている page への `ID_EDIT_PASTE` コマンドの発行」にすり替えています.

このコマンドを受けるか無視するかは,
いま選ばれている page 次第ですが.


## `OnCommand`

`CProperSheet::OnCommand` の override です.

* `IDOK` コマンドが来た場合は, いま選ばれている page を憶えておく.
* `ID_HELP` コマンドが来た場合は, [`OnHelp`](#onhelp) の呼び出しに代えさせていただく.

の 2点以外は, 基底 class に丸投げです.


## `OnHelp`

`Help` ボタンが押された時に飛んでくる `ID_HELP` コマンドのハンドラーです.
<br>
( 正確にはハンドラーではなく [`OnCommand`](#oncommand) に直接呼び出されているただの関数ですが. )

いま選ばれている page の window handle を割り出して, あとは
[`ShowHelp`](CChkMailsApp.md#showhelp) に仕事を丸投げです.


## `AddPages`

シートに page を加えます.

まず, シートの振る舞いを:

* `Help` ボタン付き
* `Apply` ボタンなし
* アイコンは ![](../pics/Icon.White.png)

とします.

あとは傘下の page 全てを
[`AddPage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#addpage) して終わりです.

