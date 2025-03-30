# `CProperSheet`

## 構成

ソースコード上は,
[`ProperSheet.h`](../ChkMails/ChkMails/ProperSheet.h) と
[`ProperSheet.cpp`](../ChkMails/ChkMails/ProperSheet.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CProperSheet`](#cpropersheet-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`BuildPropPageArray`](#buildproppagearray)

#### 標準 Window Message ハンドラー

[`OnGetMinMaxInfo`](#ongetminmaxinfo)
[`OnSize`](#onsize)

#### 固有関数

[`OnCallback`](#oncallback)

#### 固有 class

[`CDialogTemple`](#cdialogtemple)


## 概要

[`CFilterSheet`](CFilterSheet.md) の下敷きになっている基底 class です.
そのまた基底が元祖
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
なんですが, 元祖のデフォルトの動きがいろいろダメなので,
そのダメさ加減を軽減するために間に挟んでいます.

どう軽減したのかというと:

* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
では, ものすごくちっちゃい字しか出せないのを, もう少し見やすい字に置き換えてある.
* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
では, シートのサイズは変えられない ( resizable じゃない ) のを, 変えられるようにしてある.

の2点です.

これらの軽減により, 元祖より「Proper」( 適切な ) 「Sheet」( シート ) になったものと自負しております.<br>
以上, 「なぜそんなパチモンくさいネーミングにしたのか？」でした.<br>
<sup>( 逆に元祖 [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class) が,
どうしてあんなダメダメな仕様のまま世に出されているのか理解に苦しみます. )</sup>


## `CProperSheet`

この class の constructor です.

基底 class
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
の constructor を呼ぶという大事な仕事をしたあと,
メンバー変数を初期化しています.
<br>
<sub>
そうしておかないと[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)がうるさいので.
</sub>

## `OnInitDialog`

[`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog)
の override です.

処理内容は以下の通りです.

1. 基底 class の [`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog) を呼ぶ.
1. シートの最初のサイズを憶えておく.
1. シート上の [`CTabCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ctabctrl-class) の rectangle を得て,
シートとの間のマージン ( 右・底 ) を算出しておく.
1. シート上の [`CPropertyPage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertypage-class) の
rectangle を得て, `CTabCtrl` との間のマージン ( 右・底 ) を算出しておく.
1. シート上の button の window handle を全て記録しておく.
1. 「初期化済み」とのフラグを立てておく.
1. [`CProperSheet::OnInitDialog`](CProperSheet.md#oninitdialog) の戻り値を返す.

としています.

[`CPropertySheet::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#oninitdialog) の戻り値に関わらず,
上記 2.～6. の処理を進めてしまっているところが,
ぱっと見「基底 class の戻り値を無視して進めてしまっていいのか?」という印象を与えますが,
この戻り値は

* `TRUE`: 入力フォーカスをどこに設定するかは「おまかせ」で.
* `FALSE`: 入力フォーカスはこっちで設定したから, 勝手に変えないでね.

という程度のものなので,
`OnInitDialog` が失敗したにもかかわらず突っ走っているわけではありません.

マージンと Button の記録は, [サイズ変更](#onsize)のための布石です.

## `DoModal`

`CPropertySheet::DoModal` の override です.

[`CPropertySheet::DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class#domodal)
を呼ぶ前に
[`OnCallback`](#oncallback) へのコールバックをしかけておきます.

`DoModal` に入ってしまった後は手出しができなくなりますが,
このコールバックをしかけておいたおかげで,

* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
では, ものすごくちっちゃい字しか出せない.
* [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
では, シートのサイズは変えられない ( resizable じゃない ).

の 2点を改善するよう, 介入することができるようになります.


## `BuildPropPageArray`

`CPropertySheet::BuildPropPageArray` の override です.

`CPropertySheet::BuildPropPageArray` の存在は
[Microsoft のページ](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
には開示されていません. <sub>( なぜ? )</sub>,

しかし,
`C:\Program Files\Microsoft Visual Studio\2022\`_product_`\VC\Tools\MSVC\`_version_`\atlmfc\include\`
あたりに ( たぶん ) ある
`afxdlgs.h` を見ると,
確かに [`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
のメンバーの仮想関数として実在していることが確認できると思います.
手っ取り早く見るんだったら
[github 上のココ](https://github.com/mirror/winscp/blob/master/libs/mfc/source/dlgprop.cpp)
ですかね.

こうした状況からも
[`CPropertySheet`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cpropertysheet-class)
のダメさ加減が臭ってきていますが,
めげずに先へ進みましょう.

で, その基底 class の `CPropertySheet::BuildPropPageArray` を呼んで, まずは義理を果たします.
それから:

1. [Main Window](CMainWnd.md) に `GetFont` して,「見やすいフォント」をいただいてくる.
1. いただいたフォントの [`LOGFONT`](https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfontw) を得る.
1. `m_psh.ppsp` を起点に全 page 分, 以下の処理を行う.<br>
その page の [`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) を得る.<br>
`DLGTEMPLATE` に対して [`CDialogTemple`](#cdialogtemple) を `Attach`.<br>
`CDialogTemple` で「見やすいフォント」を `SetFont`.<br>
これでミッションコンプリートなので, `CDialogTemple` を `Detach`.

という処理を挟んであります.

手っ取り早く,
「[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
中のメモリーを直接いじくってフォント名だけ書き換える」という手もありますが,
オリジナルより長いフォント名にすることはできません.
今回は正攻法(?)の

* `CDialogTemplate` ( の[派生品](#cdialogtemple) ) をあてがって `SetFont` する

という手段を採用しました.

この処理により,
シートの中の各 page のフォントは「見やすいフォント」に置き換わります.<br>
シート自体のフォント ( tab や `OK` ボタンに表れるフォント ) は,
[`OnCallback`](#oncallback) で置き換えています.


## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
に対するハンドラーです.

[`OnInitDialog`](oninitdialog) で憶えておいたシートの最初のサイズを,
「最小サイズ」として返します.
これにより, シートの端を drag しても元のサイズより小さくはできなくなります.
小さくしても意味はないので入れておきました.
大きくする分には意味がある ( 場合もある ) ので不介入です.


## `OnSize`

Window Message [`WM_SIZE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-size)
に対するハンドラーです.

「初期化済み」つまり [`OnInitDialog`](#oninitdialog) 済みの場合は以下の処理を行います.

1. シートの rectangle を得ます.
1. シート上の [`CTabCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/ctabctrl-class) の rectangle を得ます.
1. `CTabCtrl` のサイズを合わせます.
1. シートの button の位置を合わせます.
1. シートのいま選ばれている page のサイズを合わせます.

そして, いずれにせよ現在のシートの rectangle を憶えておきます.

この rectangle は 4. の過程で,
左側のマージンから位置を算出する左寄りの button か,
右側のマージンから位置を算出する右寄りの button か
を切り分けるために, 次回の `OnSize` で用いられます.

シートの端っこを掴んだまま,
マウスパッドをゴシゴシするイキオイで resize を繰り返すと,
ときおり描画がチラつくように見えますが,
素の resizable なダイアログでも同じ状況だったので,
この辺が [DirectX&reg;](https://ja.wikipedia.org/wiki/Microsoft_DirectX) とかを使っていない
素の Windows の素の描画の限界なのでしょう.
もちろん実用に何ら支障はありません.


## `OnCallback`

シートで動きがあった際に呼ばれるコールバック関数です.

[このコールバック関数](https://learn.microsoft.com/ja-jp/windows/win32/api/prsht/nc-prsht-pfnpropsheetcallback)は,
3種類の局面で呼ばれることになっていますが,
ここではシートが作成されようとしていることを示す `PSCB_PRECREATE` に介入しています.
その処理内容は以下の通り:

1. [`AfxGetMainWnd`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/application-information-and-management#afxgetmainwnd) で [Main Window](CMainWnd.md) へのポインターを得る.
1. [Main Window](CMainWnd.md) に `GetFont` して,「見やすいフォント」をいただいてくる.
1. いただいたフォントの [`LOGFONT`](https://learn.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-logfontw) を得る.
1. `lParam` を [`DLGTEMPLATE*`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) として, 以下の処理を行う.<br>
`DLGTEMPLATE` に対して [`CDialogTemple`](#cdialogtemple) を `Attach`.<br>
[`CDialogTemple`](#cdialogtemple) で「見やすいフォント」を `SetFont`.<br>
これでミッションコンプリートなので, `CDialogTemple` を `Detach`.
1. ついでに `DLGTEMPLATE` のスタイルを 「resize 可能な `WS_THICKFRAME`」にしておく.

という処理を挟んであります.

この処理により,
シート自体のフォント ( tab や `OK` ボタンに表れるフォント ) は
「見やすいフォント」に置き換わります.<br>
シートの中の page のフォントは
[`BuildPropPageArray`](#buildproppagearray) で置き換えています.

あと,「見やすいフォント」をこの class に持たせず
[Main Window](CMainWnd.md) を供給源にした理由の 1つにこのコールバック関数があります.
コールバック関数なのでクラスのメンバーにアクセスできません.
しかし [Main Window](CMainWnd.md) はコールバック関数からでも
[`AfxGetMainWnd`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/application-information-and-management#afxgetmainwnd)
経由でアクセスできるので,
もういっそ「見やすいフォント」の供給源は [Main Window](CMainWnd.md) に統一しよう! となったわけです.


## `CDialogTemple`

[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate)
に介入するための独自 class です.

[`BuildPropPageArray`](#buildproppagearray) や
[`OnCallback`](#oncallback) で使用しています.
[`DLGTEMPLATE`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/ns-winuser-dlgtemplate) に
`Attach` したら `SetFont` だけして `Detach` するという hit and away な攻撃(?) のためだけに使われています.
そもそも `Attach` 動作に必要な `GetTemplateSize` が <sub>( どういうわけか )</sub> `protected` メンバーなので,
`CDialogTemplate` の派生 class として宣言するしかないという背景事情もあり,
当 class 内部でのみ使う独自 class として構えてあります.

あー, 派生元である
`CDialogTemplate` も説明している Microsoft のページがありませんねえ. <sub>( なぜ? )</sub>

その存在は
`C:\Program Files\Microsoft Visual Studio\2022\`_product_`\VC\Tools\MSVC\`_version_`\atlmfc\include\`
あたりに ( たぶん ) ある
 `afxpriv.h` で確認できるはずです.
手っ取り早く見るんだったら
[github 上のココ](https://github.com/mirror/winscp/blob/master/libs/mfc/source/dlgtempl.cpp)
ですかね.

当 class 内部でのみ使う独自 class として宣言してあるので,
Visual Studio&reg; の `Class View` にはデフォルトで出てきません.
`CProperSheet` の tree を開くと, その傘下として `CDialogTemple` は姿を現します.
<br>
<br>
<sup>
( 日本人なら「シーダイアログテンプレ」と呼びたいところかもしれませんが,
当方としての心づもりは「シーダイアログ[テンプル](https://ja.wikipedia.org/wiki/テンプル_(眼鏡))」でした.
ダイアログに「引っ掛ける」的な意味で. )
</sup>

