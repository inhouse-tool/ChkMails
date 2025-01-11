# `CMessageBox`

## 構成

ソースコード上は,
[`MessageBox.h`](../ChkMails/ChkMails/MessageBox.h) と
[`MessageBox.cpp`](../ChkMails/ChkMails/MessageBox.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor & Destructor

[`MessageBox`](#cmessagebox-1)
[`~MessageBox`](#cmessagebox-2)

#### Public

[`CreateModal`](#createmodal)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnCommand`](#oncommand)

#### 標準 Window Message ハンドラー

[`OnShowWindow`](#onshowwindow)

#### 固有関数

[`FillDialog`](#filldialog)
[`FillIcon`](#fillicon)
[`FillText`](#filltext)
[`FillButtons`](#fillbuttons)
[`SelectButtons`](#selectbuttons)
[`SelectIcon`](#selecticon)
[`Align`](#align)


## 概要

[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
に成り代わって
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
を表示する class です.

![](../Examples/Exit.png)

なぜそんなものをわざわざ手作りしてあるのかというと,
`MessageBox`をタスクバー ( 上のアイコン ) に寄った端っこに出したかったから」
という割とどうでも良さそうな動機からです.
このアプリ, 全てのユーザー操作がタスクバーの通知領域にあるアイコンをいじるところから始まるので,
なるべくその近所で操作を済ませたかったんです.

この成り代わりが意外と面倒なのが,

* [`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログは, 呼び出すと必ず
[`DoModal()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) に入る.

* [`DoModal()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) に入ると,
外から働きかけてダイアログを動かす手段がない.

* [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) に入る前は,
そもそも操作すべきダイアログが存在しない.

という事情で, ダイアログの派生 class を作って内側から操作しなければならないところです.

もうひとつ面倒なのが,
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
で出るダイアログには[ダイアログリソース](https://learn.microsoft.com/ja-jp/cpp/mfc/creating-the-dialog-resource)がない
( リソースエディタでダイアログの定義を用意しておく必要がない )
という点です.
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログは,
呼び出されると「メモリー上にダイアログテンプレートを形成する」というよく解らないワザを使って,
リソースに定義されていないダイアログを勝手に表示します.

なのでこの class の中でも, 元祖
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
と同じような処理を進めて,
元祖と同じようにダイアログをリソースなしで出しています.

元祖と異なる点は,

* ダイアログが初期化された際と閉じた際に
[Main Window](CMainwnd.md)
にその旨を通知する.<br>
( すると [Main Window](CMainwnd.md) の方で, 表示位置を寄せてくれたり, 他のダイアログと競合しないようにしてくれます. )

* 見やすいフォントで大きめに表示する.<br>
( といってもただの「Arial」ですが, Windows10/11 双方で確実に使えるフォントというと, コレかと. )

* [`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
が[出してくれるアイコン](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#remarks)
以外に, アプリのメインのアイコン (`IDR_MAINFRAME`) も出せる.<br>
( [導入時のごあいさつ](../README.md#運用) で出している例がソレです. )

というところです.


以下, 本 class 内に実装された関数それぞれの説明です.


## `CMessageBox`

この class の constructor です.

メンバー変数を初期化しています.
そうしないと
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
が指摘されてウザいから, という理由で付けているにすぎません.

「基底 class の constructor を呼び出す」などといった大事な仕事を果たしているわけでもありません.
基底 class の constructor である
[`CDialog::CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#cdialog)
は, ダイアログボックステンプレートリソースありきの構え方なので,
そんなものがないこの 
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
もどきの class からは呼びようがありません.
マジで
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
避けなだけです.

この段階では意味のない仮の初期値でメンバー変数を初期化するだけで,
意味のある値でメンバー変数が埋められるのは,
[`CreateModal`](#createmodal)
での話となります.


## `~CMessageBox`

この class の destructor です.

稼働中に構築した
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を収めたメモリーを開放するだけです.


## `CreateModal`

Message Box 風に Modal なダイアログを create します.

当関数への引数は,
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
や
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
と同様ですが,
ヘルプの ID はありません.
テキストとタイプの指定のみです.
<sub>
( そもそも
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
でヘルプの ID って指定したことないんですよね. デフォルトでなくてもいいことになってるし. )
</sub>

それらの引数に基づいて
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を構築し, その
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を使ってダイアログを初期化するというのが当関数の仕事です.
その仕事を下記のラインナップで果たします.

| 担当 | 仕事内容 |
| --- | --- |
| [`FillDialog`](#filldialog) | ダイアログの `DLGTEMPLATE` の構築 |
| [`FillIcon`](#fillicon) | アイコンの `DLGITEMTEMPLATE` の構築 |
| [`FillText`](#filltext) | テキストの `DLGITEMTEMPLATE` の構築 |
| [`FillButtons`](#fillbuttons) | ボタンの `DLGITEMTEMPLATE` の構築 |

まず
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を構築し, その下に各アイテムの
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
を加えていくというのがこれらの仕事の段取りですが,
上記のラインナップの関数はそれぞれ 2度呼び出されます.
1度目はそれぞれのテンプレートのサイズを求めるため,
2度目はそれぞれのテンプレートに中身を詰めていくためです.
この 1度目と 2度目の間に, 求められたサイズ分のメモリーを確保する作業が挟まっています.

最後に, メモリー上に確保され, 中身も詰められた
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を引数に
[`CDialog::InitModalIndirect`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#initmodalindirect)
を呼び出して, Modal ダイアログのオブジェクトを初期化します.

Modal ダイアログのオブジェクトが初期化されてしまえば,
あとは [`CDialog::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
を呼び出すばかりとなります.


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
の override です.

まずは普通に
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
を呼んだ後, 以下の独自処理を挟みます.

1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが現われたことを通知.
1. フォーカスを設定していないので, `TRUE` を返す.

ただこれだけです.
ただこれだけのことがしたくて, わざわざこんな class を作ったようなものです.
素の
[`CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class)
に,

* ダイアログの window 確立後に `CWnd*` ( か `HWND` ) を通知する ( またはコールバックを呼ぶ )

という機能が元から付いていれば, こんな苦労をすることもなかったんです.

この独自処理のおかげで,
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
しているダイアログを好きな位置に移動できるようになります. <sup>*</sup>
<br>
<sub>
*
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
って気の利かない仕様ですよね。と言っているようなもんですが.
</sub>


## `DestroyWindow`

`CDialog::DestroyWindow` の override です.

普通に
`CDialog::DestroyWindow`
を呼ぶ前に, 以下の独自処理を挟みます.

1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが消えたことを通知.

ただこれだけです.

この独自処理のおかげで, ダイアログが
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
している期間を把握できるようになります. <sup>*</sup>
<br>
<sub>
*
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
って気の利かない仕様ですよね。と言っているようなもんですが.
</sub>


## `OnCommand`

`CDialog::OnCommand` の override です.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログで, `CDialog::OnCommand` が呼び出される局面とは,
ユーザーがいずれかのボタンを押したことを意味します.
つまり
[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログを閉じるべきときを意味します.

ここでは,
そのコマンドの ID で
[`CDialog::EndDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#enddialog)
を呼び出し,
[`CDialog::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
の戻り値がコマンドの ID となるようにしています.
例えば, `IDOK` のボタンが押されたら, `DoModal` の戻り値は `IDOK` となる, といったことです.

[`CDialog::OnOK`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#onok)
や
[`CDialog::OnCancel`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oncancel)
などを, 個別ではなく一網打尽に override しているものとご理解ください.
これをやっておかないと `Yes` / `No` などの「`OK` / `Cancel` 以外のボタン」に正しく反応できないんです.

## `OnShowWindow`

Window Message [`WM_SHOWWINDOW`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-showwindow)
に対するハンドラーです.

表示されたときに, デフォルトのボタンに
[`CDialog::SetDefID`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#setdefid) し,
入力フォーカスを与えます.


## `FillDialog`

ダイアログの
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
を構築します.

まず,
確保すべきメモリーのサイズを求める呼び出しと,
確保したメモリーにテンプレートの中身を書き込むための呼び出しを切り分けます.
メモリーサイズを求める呼び出しの場合は,
アイテム 1つ分は確実に収まる仮置き用のメモリーを自前で確保します.

どちらの呼び出しの場合も, 以降の処理は変わりません.
中身を書き込む処理を,
引数として与えられたメモリーに行うのか,
自前で確保した仮置き用のメモリーに行うのかだけの違いです.
その書き込み処理は, 以下の手順で行います.

1. タイプ引数からアイコンの種別を抽出し, [`SelectIcon`](#selecticon) でアイコンの有無とリソース ID を得る.
1. タイプ引数からボタンの種別を抽出し, [`SelectButtons`](#selectbuttons) でボタンの個数とリソース ID を得る.
1. とりあえず最小限のサイズで, `DLGTEMPLATE` の中身を詰める.
1. `DLGTEMPLATE` の直後に追加の配列を置く.

[これらの配列](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate#remarks)は
`WORD` の配列で,
以下のように要素が並べられています.

| `0x0000`| メニュー配列 (は無い) |
| --: | :-- |
| `0x0000`| クラス配列 (も無い) |
| `L"ChkMails\0"`| タイトル配列 |
| `9`| フォントサイズ |
| `L"Arial\0"`| フォント名 |

メニュー配列やクラス配列は使っていないので決め打ちで `0` としています.
タイトル配列はアプリケーションの名前を,
フォントサイズは当 class で定義した `9` を決め打ちで入れ,
フォント名は当 class で定義した `"Arial"` の文字列を埋め込んでいます.

最後に, この配列が 32bit ( 4byte ) 境界に乗るように [`Align`](#align) で調整します.

以上の書き込みが終わったアドレスと書き込みを始めたアドレスの差分を,
戻り値として返すバイト数とします.

そして最後に, メモリーのサイズを求める呼び出しの場合は仮置きのメモリーを開放します.


## `FillIcon`

アイコンの
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
を構築します.

まず,
確保すべきメモリーのサイズを求める呼び出しと,
確保したメモリーにテンプレートの中身を書き込むための呼び出しを切り分けます.
メモリーサイズを求める呼び出しの場合は,
アイテム 1つ分は確実に収まる仮置き用のメモリーを自前で確保します.

どちらの呼び出しの場合も, 以降の処理は変わりません.
中身を書き込む処理を,
引数として与えられたメモリーに行うのか,
自前で確保した仮置き用のメモリーに行うのかだけの違いです.
その書き込み処理は, 以下の手順で行います.

1. タイプ引数からアイコンの種別を抽出し, [`SelectIcon`](#selecticon) でアイコンの有無とリソース ID を得る.
1. アイコンがある場合は, そのアイコンの `DLGITEMTEMPLATE` の中身を詰める.
1. アイコンがある場合は, `DLGITEMTEMPLATE` の直後に「クラス配列」, 「リソース配列」, 「タイトル配列」を置く.

[これらの配列](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)は
`WORD` の配列で,
アイコンの場合は以下のように要素が並べられています.

| `0xffff`| クラス配列: 序数表現 |
| --: | :-- |
| `0x0082`| 序数: 'Static contol' |
| `0xffff`| リソース配列: 序数表現 |
| `0x0080`| 序数: `IDI_QUESTION` |
| `\0`| タイトル配列: 文字列表現(空文字列) |

* 「クラス配列」は冒頭の `0xffff` で文字列ではなく序数値であることを示し,<br>
次の `0x0082` が static control であることを示す序数となっています ( アイコンも static の一種 ).
* 「リソース配列」は冒頭の `0xffff` で文字列ではなく序数値であることを示し,<br>
次に Resource ID で `IDI_QUESTION` や `IDR_MAINFRAME` などのアイコンの ID を示す序数が置かれています.
* 「タイトル配列」は冒頭が `0xffff` ではないので序数値ではなく文字列であることを示していますが,<br>
冒頭から `\0` なので空文字列となっています.

最後に, この配列が 32bit ( 4byte ) 境界に乗るように [`Align`](#align) で調整します.

以上の書き込みが終わったアドレスと書き込みを始めたアドレスの差分を,
戻り値として返すバイト数とします.

そして最後に, メモリーのサイズを求める呼び出しの場合は仮置きのメモリーを開放します.


### `FillText`

テキストの
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
を構築します.

まず,
確保すべきメモリーのサイズを求める呼び出しと,
確保したメモリーにテンプレートの中身を書き込むための呼び出しを切り分けます.
メモリーサイズを求める呼び出しの場合は,
アイテム 1つ分は確実に収まる仮置き用のメモリーを自前で確保します.

どちらの呼び出しの場合も, 以降の処理は変わりません.
中身を書き込む処理を,
引数として与えられたメモリーに行うのか,
自前で確保した仮置き用のメモリーに行うのかだけの違いです.
その書き込み処理は, 以下の手順で行います.

1. [Main Window](CMainwnd.md) から Device Context を拝借して「見やすいフォント」での Dialog Unit を求める.
1. 与えられたテキストを表示するために縦横何 Dialog Unit 必要か求める.
1. アイコンがある場合は, そのアイコンの幅を横幅に加える.
1. [`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)の横幅・縦幅を表示に必要な分拡大する.
1. 拝借した Device Context を開放する.
1. テキストの `DLGITEMTEMPLATE` の中身を詰める.
1. `DLGITEMTEMPLATE` の直後にテキストの「クラス配列」, 「リソース配列」, 「タイトル配列」を置く.

[これらの配列](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)は
`WORD` の配列で,
テキストの場合は以下のように要素が並べられています.

| `0xffff`| クラス配列: 序数表現 |
| --: | :-- |
| `0x0082`| 序数: 'Static contol' |
| `"Quit?\0"`| リソース配列: 文字列表現 |
| `\0`| タイトル配列: 文字列表現(空文字列) |

* 「クラス配列」は冒頭の `0xffff` で文字列ではなく序数値であることを示し,<br>
次の `0x0082` が static control であることを示す序数となっています.
* 「リソース配列」は冒頭が `0xffff` ではないので序数値ではなく文字列であることを示し,<br>
続く要素は `\0` までがその文字列であることを示します.
* 「タイトル配列」は冒頭が `0xffff` ではないので序数値ではなく文字列であることを示していますが,<br>
冒頭から `\0` なので空文字列となっています.

最後に, この配列が 32bit ( 4byte ) 境界に乗るように [`Align`](#align) で調整します.

以上の書き込みが終わったアドレスと書き込みを始めたアドレスの差分を,
戻り値として返すバイト数とします.

そして最後に, メモリーのサイズを求める呼び出しの場合は仮置きのメモリーを開放します.

ところで……
<br>
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
に仕込む幅や高さの単位は「Dialog Unit (DLU)」という「デバイスに依存しない(というフレコミの)」単位なのですが,
[Microsoft&reg; の説明](https://learn.microsoft.com/en-us/previous-versions/windows/desktop/bb226789(v=vs.85))によると,
> About DLUs: In unmanaged code, the system measures the placement of controls on a surface in dialog units (DLUs), not pixels. A dialog unit is the device-independent measure to use for layout. One horizontal dialog unit is equal to one-fourth of the average character width for the current system font. One vertical dialog unit is equal to one-eighth of an average character height for the current system font.

ということで,
横方向の 1 [Dialog Unit] は
> one-fourth of the average character width for the current system font

つまり「システムフォントの文字幅の平均値の 1/4」となります.
「平均」とか言われてもピンときませんが,
( プロポーショナルなフォントでは )
大文字の 'W' と小文字の 'i' とか色々幅が異なるのでこういうことにしたのでしょう.

なので, 必然的に

* 全部大文字の文字列をあてがうと、幅の見積もりが足りなくなる

ということになります.
<sub>
( 素の
[`AfxMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
で試してみたら, そうなることが確認できました. )
</sub>

案の定, [ライセンス宣言](../README.md#license-announcement)の全部大文字部分が引っかかったので,
この実装では Dialog Unit に少し補正をかけて
「少なくともライセンス宣言が収まる程度」には広げています.

### `FillButtons`

ボタンの
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
を構築します.

まず,
確保すべきメモリーのサイズを求める呼び出しと,
確保したメモリーにテンプレートの中身を書き込むための呼び出しを切り分けます.
メモリーサイズを求める呼び出しの場合は,
アイテム 1つ分は確実に収まる仮置き用のメモリーを自前で確保します.

どちらの呼び出しの場合も, 以降の処理は変わりません.
中身を書き込む処理を,
引数として与えられたメモリーに行うのか,
自前で確保した仮置き用のメモリーに行うのかだけの違いです.

その書き込み処理は,
まずタイプ引数から [`SelectButtons`](#selectbuttons) でボタンの個数とその種別を得て,
ボタンの個数だけ以下の手順を繰り返します.

1. ボタンの `DLGITEMTEMPLATE` の中身を詰める.
1. `DLGITEMTEMPLATE` の直後にボタンの「クラス配列」, 「リソース配列」, 「タイトル配列」を置く.

[これらの配列](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate#remarks)は
`WORD` の配列で,
ボタンの場合は以下のように要素が並べられています.

| `0xffff`| クラス配列: 序数表現 |
| --: | :-- |
| `0x0080`| 序数: 'Button contol' |
| `"OK\0"`| リソース配列: 文字列表現 |
| `\0`| タイトル配列: 文字列表現(空文字列) |

* 「クラス配列」は冒頭の `0xffff` で文字列ではなく序数値であることを示し,<br>
次の `0x0080` が button control であることを示す序数となっています.
* 「リソース配列」は冒頭が `0xffff` ではないので序数値ではなく文字列であることを示し,<br>
続く要素は `\0` までがその文字列であることを示します.
* 「タイトル配列」は冒頭が `0xffff` ではないので序数値ではなく文字列であることを示していますが,<br>
冒頭から `\0` なので空文字列となっています.

最後に, この配列が 32bit ( 4byte ) 境界に乗るように [`Align`](#align) で調整します.

以上の書き込みが終わったアドレスと書き込みを始めたアドレスの差分を,
戻り値として返すバイト数とします.

そして最後に, メモリーのサイズを求める呼び出しの場合は仮置きのメモリーを開放します.


## `SelectButtons`

タイプ引数に応じたボタンを選びます.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログには
[タイプ](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#parameters)
という引数があって,
この中に「どういう `MessageBox` を出すか」を指定するあれこれが入っているのですが,
そのあれこれに応じて「どういうボタンを出すか」を決めています.

ボタンは複数 ( 最大 3つ ) あるので,
この関数を呼び出す側でそれ全部が収まる配列を用意してもらって,
この関数ではその配列の中に答えを書き込む方式にしています.

書き込む答えは,
ボタンを表示する順番通りに

* ボタンの ID
* ボタンのキートップに表示する文字列

としています.

当関数を呼び出しているのは以下のみなさんです:

| 呼び出し元 | 目的 |
| --- | --- |
| [`FillDialog`](filldialog) | ボタンの個数を `DLGTEMPLATE` のアイテム個数に反映させるため |
| [`FillButtons`](fillicon) | ボタンの Resource ID を `DLGITEMTEMPLATE` に仕込むため |


## `SelectIcon`

タイプ引数に応じたアイコンを選びます.

[`MessageBox`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox)
系のダイアログには
[タイプ](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-messagebox#parameters)
という引数があって,
この中に「どういう `MessageBox` を出すか」を指定するあれこれが入っているのですが,
そのあれこれに応じて「どういうアイコンを出すか」を決めています.
( アイコンがない場合もあります. )

戻り値は, アイコンのリソース ID ( `IDI_QUESTION` とか `IDR_MAINFRAME` とか ) です.
戻り値の型が `LPWSTR` となっていて, なにか文字列を表現しているように見えますが,
実際に格納されているのはリソース ID を示す序数値です.

当関数を呼び出しているのは以下のみなさんです:

| 呼び出し元 | 目的 |
| --- | --- |
| [`FillDialog`](filldialog) | アイコンの有無を `DLGTEMPLATE` のアイテム個数に反映させるため |
| [`FillIcon`](fillicon) | アイコンの Resource ID を `DLGITEMTEMPLATE` に仕込むため |
| [`FillText`](filltext) | アイコンの有無でテキストの配置が変わるため |

## `Align`

与えられたポインターを 4バイトアライメントに乗った値にして返します.

なんでこんなことをやっているのかというと,
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
や
[`DLGITEMTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgitemtemplate)
は, 4バイト境界にのった位置に置かなければならないからです.