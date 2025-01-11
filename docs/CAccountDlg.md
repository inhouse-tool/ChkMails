# `CAccountDlg`

## 構成

ソースコード上は,
[`AccountDlg.h`](../ChkMails/ChkMails/AccountDlg.h) と
[`AccountDlg.cpp`](../ChkMails/ChkMails/AccountDlg.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CAccountDlg`](#caccountdlg-1)

#### Public

[`SetAccounts`](#setaccounts)
[`GetAccounts`](#getaccounts)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnOK`](#onok)

#### 標準 Window Message ハンドラー

[`OnGetMinMaxInfo`](#ongetminmaxinfo)
[`OnTimer`](#ontimer)

#### コマンドハンドラー

[`OnHelp`](#onhelp)
[`OnCheckHide`](#oncheckhide)
[`OnButtonAdd`](#onbuttonadd)
[`OnButtonDelete`](#onbuttondelete)
[`OnChangeEdit`](#onchangeedit)
[`OnClickRadio`](#onclickradio)
[`OnChangeList`](#onchangelist)

#### 固有関数

[`OnSelectItem`](#onselectitem)
[`EnableAdd`](#enableadd)


## 概要

メールアカウント設定のための下記のようなダイアログを動かしている class です.

![](../Examples/Accounts3.png)

どうということはないありふれた独立 dialog ですが,
[`CListCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class)
や
[`CEdit`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cedit-class)
のフォントは,
Arial のようなプロポーショナルなフォントだと何かと間違いのもと ( "rn" と "m" が見分けづらいとか ) なので,
[Main Window](CMainWnd.md) ご指定の等幅フォント
( [`GetCommonFont()`](CChkMailsApp.md#getcommonfont) ) に入れ替えています.

あと, スクリーンの中央ではなく, タスクバー ( 上のアイコン ) に寄った端っこに出てくるのも,
[Main Window](CMainWnd.md) の差し金 ( [`NotifyPopup()`](CChkMailsApp.md#notifypopup) ) です.

このダイアログは resizable ( マウスでダイアログの隅っこを引っ張るとサイズが変えられる ) なので,
ユーザー名やプロバイダー名が長くて収まらない場合は適当に引き延ばしてお使いください.

以下, 本 class 内に実装された関数それぞれの説明です.


## `CAccountDlg`

この class の constructor です.

ソースコードを見ると, 何もやっていないように見えて「要るの？これ。」という感じですが,
リソース ID を指定して基底クラスの constructor を呼ぶという大事な仕事をしています.


## `SetAccounts`

アカウントを設定されます.

[Main Window](CMainWnd.md) が持っているアカウントの配列を受け取ります.
ダイアログが表示される前に受け取ります.
当 class が
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
動作を始めた後では何の出し入れもできないので, 事前に済ませておくしかないのです.


## `GetAccounts`

アカウントを取得されます.

当 class の
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
動作終了後,
当 class が持っているアカウントの配列を [Main Window](CMainWnd.md) が取得していきます.
<br>
<sub>
情報を吸い上げられたあと, 本 class は [Main Window](CMainWnd.md) に棄てられます.
( こういう言い方をすると, なんか Main Window がヒドい人みたいですが. )
</sub>


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
の override です.

まずは普通に
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
を呼んだ後, 以下の独自処理を挟みます.

1. ダイアログのアイコンをセットする.
1. ダイアログの最初のサイズを憶えておく.
1. [`GetCommonFont`](CChkMailsApp.md#getcommonfont) で共通の等幅フォントを得る.
1. Edit Box に等幅フォントをセットする.
1. List Control を, 全カラムで選択表示するように設定.
1. List Control を, グリッドを表示するように設定.
1. カラム 0 にダミーを挟んで, カラム 1 に右詰めの `User`, カラム 2 に左詰めの `Host` を追加.
1. カラム 0 のダミーを削除.
1. [`SetAccounts`](#setaccounts) で設定されていたアカウントを List Control に追加.
1. 1つでもアカウントがあったら List Control のカラム幅を自動調節.
1. ダイアログ上のアイテムをデフォルトの状態にする.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが現われたことを通知.
1. `OK` ボタンにフォーカスを移す.
1. フォーカスを設定したので, `FALSE` を返す.

このうち, 7. と 8. にある「ダミー」は一見不要そうな段取りですが,
[「カラム 0 は左詰めにしかできない」](https://learn.microsoft.com/ja-jp/windows/win32/api/commctrl/ns-commctrl-lvcolumnw#remarks)
という List Control の情けない事情を回避するためのものです.
<br>
<sup>
いや, 「回避策」を説明するんじゃなくて, 「修正」してくれんかね?
</sup>


## `DestroyWindow`

`CDialog::DestroyWindow` の override です.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが引っ込んだことを通知する,
という手順を挟むためだけに override しています.


## `OnOK`

`CDialog::OnOK` の override です.

入力されたアカウントの追加は [`OnButtonAdd`](#onbuttonadd) で済ませているので,
特に `OnOK` ですることもないのですが,

* せっかくアカウント情報を入力したのに `Add` ボタンを押すのを忘れて `OK` ボタンを押した

といううっかりさんが居たので, 「`Add` を押し忘れて `OK` が押された」という気配を察知したら,
代わりに `Add` を押しておくという心遣いを加えてあります.

あと,

* せっかく `Server`, `User`, `Password` まで入れたのに `POP3` か `POP3S` を選ぶのを忘れて `OK` ボタンを押した

といううっかりさんも居たので, 「どっちか選べよ」とツッコんで `CDialog::OnOK` はしないという心遣いも加えてあります.


## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
に対するハンドラーです.

[`OnInitDialog`](#oninitdialog) で憶えておいたダイアログの最初のサイズを,
「最小サイズ」として返します.
これにより, ダイアログの端を drag しても元のサイズより小さくはできなくなります.
小さくしても意味はないので入れておきました.
大きくする分には意味があるので不介入です.

## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
に対するハンドラーです.

下記のタイマー値を取り扱っています.

| タイマー値 | 状況 | 処理内容 |
| --- | --- | --- |
| `TID_SELCHANGE` | 「選択対象変更」 | [`OnSelectItem`](#onselectitem) 呼び出し. |


## `OnHelp`

`Help` ボタンが押された時に飛んでくる `IDHELP` コマンドのハンドラーです.

[`ShowHelp`](CChkMailsApp.md#showhelp) に仕事を丸投げです.


## `OnCheckHide`

`Hide` Check Box が click された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

Check Box が&#x2611;されたか否かによって,
`Password` の Edit Box の伏字／通常表示を切り替えます.


## `OnButtonAdd`

`Add` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

`Server`, `User` に入力されたそれぞれの文字列を,
List Control に追加します.

ただし, すでに List Control 内に全く同じ組み合わせがエントリー済みの場合は,
エントリー済みの方を一旦削除してから List の最後尾に追加します.
( この動きを使って, エントリー済みの項目を手動で並べ替えできます.
順序を入れ替える手段の実装をサボったので, どーしても入れ替えたい場合は, このパズルみたいな方法でお願いします. )


## `OnButtonDelete`

`Delete` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

今 List Control 上で選択されている行のエントリーを削除します.

## `OnChangeEdit`

`Server`, `User`, `Password` のいずれかで文字列が変更された時に飛んでくる `EN_CHANGE` コマンドのハンドラーです.

[`EnableAdd`](#enableadd) を呼ぶだけです.


## `OnClickRadio`

`POP3`, `POP3S` のいずれかがクリックされた時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

[`EnableAdd`](#enableadd) を呼ぶだけです.


## `OnChangeList`

List Control 上の選択が変えられた時に飛んでくる `LVN_ITEMCHANGED` コマンドのハンドラーです.

待ち時間ゼロで `TID_SELCHANGE` のタイマーをしかけてこの場はやり過ごします.

なぜこんなことをしているかというと,
A から B に選択し直すだけで

* A が選択状態から非選択状態になったという `LVN_ITEMCHANGED`
* B が非選択状態から選択状態になったという `LVN_ITEMCHANGED`

がいちいち飛んでくるのでめんどくさかったんです.
やり過ごしてから「結局、何が選ばれたんだ?」と構える方が世話がなかったので.
( めんどくさがらずにマジメに対応した例は [`CDomainPage::OnChangeList`](CDomainPage.md#onchangelist) にあります. )


## `OnSelectItem`

List Control 上の選択が変えられたことに対応します.

いずれかの有効な行が選択された場合：

* `Server`, `User`, `Password` の各 Edit Box に選択されたアカウントの内容を詰める.
* `POP3` か `POP3S` かの選択を選択されたアカウントの選択通りに改める.
* `Delete` ボタンを有効にする.

無効だった場合:

* `Server`, `User`, `Password` の各 Edit Box を空にする.
* `POP3` か `POP3S` かの選択を両方無効にする.
* `Delete` ボタンを無効にする.


## `EnableAdd`

`Add` ボタンを有効または無効にします.

`Add` ボタンを有効にする条件が整っている場合,
つまり `Server`, `User`, `Password` のいずれも文字列が入力されており,
なおかつ `POP3` か `POP3S` の選択も済んでいる場合,
`Add` ボタンを有効にします.

条件が整っていない場合は `Add` ボタンを無効にします.

[`OnChangeEdit`](#onchangeedit) と [`OnClickRadio`](#onclickradio) から呼ばれています.