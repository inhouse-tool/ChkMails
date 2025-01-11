# `CNotifyWnd`

## 構成

ソースコード上は,
[`NotifyWnd.h`](../ChkMails/ChkMails/NotifyWnd.h) と
[`NotifyWnd.cpp`](../ChkMails/ChkMails/NotifyWnd.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CNotifyWnd`](#cnotifywnd-1)

#### Public

[`SetText`](#settext)

#### 標準 Window Message ハンドラー

[`OnCreate`](#oncreate)
[`OnEraseBkgnd`](#onerasebkgnd)
[`OnLButtonDown`](#onlbuttondown)
[`OnPaint`](#onpaint)
[`OnTimer`](#ontimer)
[`OnDisplayChange`](#ondisplaychange)

#### 固有関数

[`PlaceWindow`](#placewindow)
[`ToastWindow`](#toastwindow)
[`Draw`](#draw)


## 概要

[サマリー](../README.md#notification) の表示に用いられている通知用ウィンドウの class です.

![](../Examples/Summary.png)
<p>

Windows にはいわゆる
「[トースト](https://learn.microsoft.com/ja-jp/windows/apps/design/shell/tiles-and-notifications/toast-notifications-overview)<sup>*</sup>」
と呼ばれている通知用の仕組みがあるので,
これを使おうと最初は考えたのですが,
あまりに使いづらい<sup>**</sup>ので断念しました.

その代わりに自前で作ったのがこの class です.
「トースト」での反省点を踏まえ, なるべく小さなウィンドウをなるべく邪魔にならないように出すことを心掛けています.

この class は contructor で window の生成まで行います.
なので, メンバー変数としてこの class を持つだけでも window 表示の準備ができている状態になります.
ただし, 初期状態では window は引っ込んでいます.

Window が現われるのは,
この class が提供している [`SetText( CString strText )`](#settext) という関数に文字列を与えたときです.
与えられた文字列が収まるサイズの window が表示されます.

文字列は複数行あっても構いません. その場合は行間を `'\n'` で区切ります.
2つの改行 `"\n\n"` で区切ると, 表示項目の区切りと見なし, 前後の行の間に横線を引きます.
1つ行の中にタブ文字 `'\t'` があると, タブ文字の前のテキストを左寄せに, タブ文字の後のテキストを右寄せに表示します.

表示された window はマウスでクリックすると, サクッと引っ込みます.

あと, プライマリーモニターが入れ替わるような使い方をしている人
( 会議から帰ってきたらノートPCを自席のモニターにつないでそちらをプライマリーに設定しているような人とか )
向けの手当てとして,
window 表示中にモニターの変化があったら,
新しいプライマリーモニター上に表示を移動する仕掛けも入っています.

なお, 表示をお好みに合わせる際にソースコードをいじらずに済むよう,
好みの分かれそうな設定値は registry から読むようにしています.
ただし, その registry 値を変更するための GUI は本アプリでは用意しておりません.
どーしても変更したい人は, 各自勝手に Registry Editor でいじってください.

<sub>* MS は「トースト」というネーミングを後悔しているのか,
[「アプリ通知」という検索に苦労しそうな名前に替えようとしているらしいです.](https://learn.microsoft.com/ja-jp/windows/apps/design/shell/tiles-and-notifications/adaptive-interactive-toasts)
<br>
** なんというか, 仕掛けが大げさすぎるというか, 説明が C# しか眼中にないというか……. <sub>


## `CNotifyWnd`

この class の constructor です.

まず, メンバー変数を初期化しています.
そうしないと
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
が指摘されてウザいから, という理由でやっているのですが,
カスタマイズ可能な設定値に関してはここで registry から読み込んでいます.

その registry キーは

`HKEY_CURRENT_USER\Software\ChkMails\NotifyWnd\`<p>

にあり ( 本アプリをデフォルトでお使いの場合は, 上記のキー自体が存在しません. ),
デフォルトではそれぞれの設定値が下表のようになっています.

| Name | Type | Data |
| --- | --- | --- |
| `FontName`	| `REG_SZ`	| `"Arial"`	|
| `FontSize`	| `REG_DWORD`	| `14`[pix]	|
| `Toast`	| `REG_DWORD`	| `1` (`true`)	|
| `Popup`	| `REG_DWORD`	| `400`[ms]	|
| `crText`	| `REG_DWORD`	| `0x00ffffff` (`RGB( 255, 255, 255 )`)	|
| `crBack`	| `REG_DWORD`	| `0x00000000` (`RGB( 0, 0, 0 )`)	|
| `crLine`	| `REG_DWORD`	| `0x007f7f7f` (`RGB( 127, 127, 127 )`)	|
| `Alpha`	| `REG_DWORD`	| `168` (なんとか背景が透かして見える程度の透過度)	|

このうち `Toast` は「トースト動作(勝手にそう命名)をするか否か」のスイッチです.<br>
`0` ならそんな動作はせず, window が普通に表示されます.<br>
`1` なら window が焼き上がったトーストのごとく, 下から上にポンとせり上がってきます.

`Popup` はトーストが上がりきるまでにかかる時間 [ms] です.
この時間を長くするとトーストはゆっくりとせり上がってくるようになり,
あまり「トースト感」がなくなってきます.
なお, 表示が何行あっても上がりきる時間は同じ設定値なので,
表示行数が多い方がこの window のせり上がる速度は高くなります.

で, メンバー変数の初期化が終わると,
自分の window を create します.
行儀の良い window は, constructor がついでに create までするという横着はせず,
外から create される形式をとるのが普通だと思いますが,
当 class では横着しています.
ソフトウェア部品としての利用の手軽さを優先しました.
<br>
<sub>
( 理論上, 失敗することもあり得る create を, 「失敗した」と戻り値で返すこともできない constructor でやるのは,
少々お行儀が悪いんですけどね. )
</sub>

## `SetText`

表示する文字列を設定します.

その処理手順は以下の通りです:

1. 与えられた文字列の前後の空白を
[`Trim`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/cstringt-class?#trim)
する.
1. 文字列が空だったら, window を引っ込める.
1. Device context を get してフォントを選ぶ.
1. 文字列の `\n` までを切り出す. 改行がなかったら文字列を最後までを取り込む.
1. 文字列のサイズを得る.
1. 文字列が `\t` で仕切られていたら, 仕切りの分としてマージンを横幅に加える.
1. 横幅がこれまでの最大値だったら, 最大値を入れ替える.
1. サイズの縦幅を window の高さに加える.
1. 与えられた文字列の末端に達するまで 4. からを繰り返す.
1. Device conntext を開放する.
1. Window のサイズにマージンを加える.
1. [`PlaceWindow`](#placewindow) を呼んで window の位置を確定する.
1. 「トースト動作」をする設定ならその動作をしかけ, しない設定ならそのまま window を表示.

「トースト動作」をする場合は, ここではまだ window を表示しない ( 隠したまま ) というのがミソで,
[`PlaceWindow`](#placewindow) で window の位置が定まっても隠したまま,
実際に表示されるのは「トースト動作」に入ってからです.


## `OnCreate`

Window Message [`WM_CREATE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-create)
に対するハンドラーです.

まず基底 class の `OnCreate` を済ませた後,
下記のように window の拡張スタイルを設定します.

| 拡張スタイル		| 目的 |
| --- | --- |
| `WS_EX_LAYERED`	| 背景が透けて見える半透明な window に. |
| `WS_EX_TOPMOST`	| 他の window にカブって隠れたりしないように |
| `WS_EX_TOOLWINDOW`	| タスクバーにこの window のタイトルがお邪魔しないように |

そして
[`SetCursor`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setcursor)
で, この window 上のマウスカーソルの形状をありふれた矢印に設定した後,
[`SetLayeredWindowAttributes`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setlayeredwindowattributes)
で, この window の透明度 ( 不透明度 ) を設定します.

最後に, この window で文字列表示を行うためのフォントを作っておいて完了です.

せっかく `lpCreateStruct` をもらっているのだから,
拡張スタイルの設定は `lpCreateStruct->dwExStyle` にやっておけば良さそうなものですが,
いっぺん素のまま `CWnd::OnCreate` を通しておいてカスタマイズは後から施す,
というのがなんというか, 様式美みたいなもんです.


## `OnEraseBkgnd`

Window Message [`WM_ERASEBKGND`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-erasebkgnd)
に対するハンドラーです.

何もせず即 `return` です.

この `CWnd::OnEraseBkgnd` の無効化は「画面を全部自前で描画している window 業界」においてよく知られている
「画面のちらつきを抑える方法」なのですが,
業界の一員として慣習に従ってみました.

ま, この window の場合ほぼ効果はないはずですが.


## `OnLButtonDown`

Window Message [`WM_LBUTTONDOWN`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-lbuttondown)
に対するハンドラーです.

無条件で window を引っ込めます.
どこがつつかれたのかにかかわらずにです.


## `OnPaint`

Window Message [`WM_PAINT`](https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-paint)
に対するハンドラーです.

更新された rectangle を背景色で塗りつぶした後,
[`Draw`](#draw) に描画を委ねます.


## `OnTimer`

Window Message [`WM_TIMER`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-timer)
に対するハンドラーです.

下記のタイマー値を取り扱っています.

| タイマー値 | 状況 | 処理内容 |
| --- | --- | --- |
| `TID_TOAST` | 「トースト動作中」 | [`ToastWindow`](#toastwindow) でトースト動作. トースト完了ならタイマー解除. |
| `TID_MONITOR` | 「モニター切替後」 | [`PlaceWindow`](#placewindow) で window を移動. 1回きりでタイマー解除.|


## `OnDisplayChange`

Window Message [`WM_DISPLAYCHANGE`](https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-displaychange)
に対するハンドラーです.

Window が引っ込んでいなければ,
一旦 window を引っ込めた上で,
タイマー `TID_MONITOR` を `5`秒後にしかけてこの場は終わります.
なんで `5`秒も待たせるのかというと,
モニター切替直後のごたごたをやり過ごすには, これぐらいのゆとりが欲しいように見えたからです.<sup>※</sup>
<br>
<sub>
※ 個人の観察です。
</sub>


## `PlaceWindow`

Window をしかるべき位置に配置します.

この「しかるべき位置」というのは,
タスクバーの通知領域<sup>* </sup>に寄り添うような位置です.
その通知領域<sup>* </sup>はプライマリーモニターにあるので,
まず
[`MonitorFromPoint`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-monitorfrompoint)
で, プライマリーモニターのハンドル<sup>**</sup>を得ます.

で, プライマリーモニターのハンドルで
[`GetMonitorInfo`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-getmonitorinfow)
すると,

* モニターの表示領域
* モニターの作業領域

が得られます.
この両領域の差分が, タスクバーで占められている領域ということになります.

あとは, この差分に基づいてタスクバーがモニターの四辺のいずれにあるのかを求め,
そのありかに応じて window の位置を決定します.

最後に
[`SetWindowPos`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setwindowpos)
で, window を決定した位置に移動して完了です.

<sub>* [この「通知領域」を「タスクトレイ」と呼ぶ人は世代が古いそうです.](https://www.yrl.com/column/wazaari_pc/taskbar.html)<br>
　しかし「タスクトレイ」→「通知領域」といい, 「トースト」→「アプリ通知」といい, 最近の Microsoft&reg; の言葉選びは検索しづらくなる一方です.
<br>
** 座標 ( 0, 0 ) を含むモニターをチョイスすればそれがプライマリーモニターです.
[Windows&reg; に詳しいおじさんがそう断言していました](https://devblogs.microsoft.com/oldnewthing/20070809-00/?p=25643). <sub>


## `ToastWindow`

Window を「トースト」します.

どこまで「トースト」したかに基づき, window の位置を移動します.

* 「トースト」の始まりの場合:<br>
[`PlaceWindow`](#placewindow)直後の「最終位置」にあるはずなのでその位置を記憶し,
タスクバーにきれいに隠れる位置を選んでから
[`ShowWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-showwindow)
します.
* 「トースト」の終わりの場合:<br>
始まりのときに記憶しておいた「最終位置」を選びます.
* 「トースト」の途中の場合:<br>
どこまで「トースト」したかに応じた位置を選びます.

で, 選んだ位置に
[`SetWindowPos`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-setwindowpos)
します.

少し特殊なのが, タスクバーがモニターの上にある場合で,
この場合は window が上から下に降りてくる動きになります.
またこの場合だけ, 動かすたびに画面の再描画が必要になります.
タスクバーをこういう設定にしている人, あんまり居ないんですけどね.
いちおうは対応しておきませんと.


## `Draw`

Window を描画します.

まず, window の幅に応じて「最初の 1行」の rectangle を求めておきます.
次に描画モードを「背景なし」にして文字の色を設定します.

あとは, 与えられていたテキストの末端まで, 以下の処理を繰り返します.

1. 1行分のテキストを切り出す.
1. 空白行なら, window の左端から右端まで線を引く.
1. 空白行でなければ,<br>
`\t` で区切られた行なら, `\t` の前(左)を左寄せ, `\t` の後(右)を右寄せでそれぞれテキスト描画.<br>
`\t` で区切られていない行なら, 行全体をテキスト描画.
1. rectangle を 1行分下に移動.

少し特殊なのが, タスクバーがモニターの上にある場合で,
描画処理の原点を上方向にずらして, テキストの底の方が window の底の方に描かれるようにしています.
タスクバーをこういう設定にしている人, あんまり居ないんですけどね.
いちおうは対応しておきませんと.