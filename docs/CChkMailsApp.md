# `CChkMailsApp`

## 構成

ソースコード上は,
[`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) と
[`ChkMailsApp.cpp`](../ChkMails/ChkMails/ChkMails.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Macros

[`NotifyPopup`](#notifypopup)
[`GetCommonFont`](#getcommonfont)
[`ShowHelp`](#showhelp)

#### Public

[`DeleteProfileValue`](#deleteprofilevalue)
[`GetVersionInfo`](#getversioninfo)

#### Override

[`InitInstance`](#initinstance)
[`PreTranslateMessage`](#pretranslatemessage)
[`DoMessageBox`](#domessagebox)

## 概要

[MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library)
ベースなので,
プログラムの始まりは
[`CWinApp`](https://learn.microsoft.com/en-us/cpp/mfc/reference/cwinapp-class)
派生 class からです.

この `CChkMailsApp` class の特色としては.

1. MFC オススメの *Document & View Architecture* とかダイアログベースなどの構成ではなく,
独自の [`CWnd` 派生 class](CMainWnd.md) を作って,
それをメインウインドウとしている.
1. [`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox) の行き着く先である
[`DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
 に何やら独自実装をあてがっている.

というところでしょうか.

1. は, ウインドウとして見える必要がない常駐アプリなので, 
[`CView`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cview-class)
も
[`CDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class)
も要らないから,<br>
2. は, アプリ中から呼ばれた
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
の表示位置をタスクバーに寄せたいから,<br>

という割とどうでもいい動機で行われているカスタマイズです.

あと, 特色としては, 同じアプリを2回以上起動すると,
後輩が先輩を押し退けて唯一のプロセスの座に着くというところですかね.
複数起動しても意味のないアプリなので.

[`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) は,
class 内の定義のみならず, 本アプリを構成する全てのソースに共通の定義を提供する役目を負っています.
<p>
以下の独自 Window Message が定義されています.

| Message | 用途 |
| --- | --- |
| `WM_USER_TRAY_NOTIFY`	| タスクバーの通知領域に常駐させたアイコンからの通知<br>[`CMainWnd`](CMainWnd.md) の中でしか使っていない Window Message なので [`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) で定義すれば良さそうなものだが, `WM_APP`+x を一か所に並べたいという理由だけでここで定義. |
| `WM_POPUP_NOTIFY`	| 各ダイアログが表示されるときにそれぞれから送られてくる Window Message.<br>[`CMainWnd`](CMainWnd.md) はこれを受けて, 送り元の window の表示位置をアイコンに寄せる. |
| `WM_GET_COMMONFONT`	| 共通の等幅フォントを得るために各ダイアログから送られてくる Window Message.<br>[`CMainWnd`](CMainWnd.md) はこれを受けて, あらかじめ作っておいた等幅フォントを返す. |
| `WM_GET_SENDER`	| メールの送信者を得るために[`CWhitePage`](#cwhitepage)から送られてくる Window Message.<br>[`CMainWnd`](CMainWnd.md) はこれを受けて, あらかじめ取得しておいた情報を横流しする. |
| `WM_SHOW_HELP`	| Help を表示するために配下のダイアログから送られてくる Window Message.<br>[`CMainWnd`](CMainWnd.md) はこれを受けて, 送信元 window に対応した help 表示を行う. |

結局, 上記全ての Window Message が [`CMainWnd`](CMainWnd.md) がらみなので,
やっぱり [`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) で定義すれば良さそうなもんですが,
[`MainWnd.h`](../ChkMails/ChkMails/MainWnd.h) だと情報の開示量が多すぎて何を共有しているのかフォーカスがぼやけるので,
あえて [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) で定義して「これだけはみんな共通の定義」という感じを出しています.

以下, 本 class 内に実装された関数それぞれの説明です.


## `NotifyPopup`

Dialog がポップアップしたことを知らせるマクロ.

実体を持った関数ではなく, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) に定義されたマクロです.
( なので実際には class とも無関係ですが. )

ユーザー定義の Window Message である `WM_POPUP_NOTIFY` を
[`CMainWnd`](CMainWnd.md) に
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
します.
`lParam` には, マクロの引数として渡された呼び出し元ダイアログの window handle が載っています.

この Window Message を受けた[`CMainWnd`](CMainWnd.md)の[`OnPopupNotify`](CMainWnd.md#onpopupnotify)は,
`lParam` に載せられた window handle を使って,
ダイアログの表示位置をアイコンの傍に寄せる処理を追加します.

あと, このマクロは「いずれかのダイアログが
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
中であること」を
[Main Window](CMainWnd.md) に知らせる役目も持っています.
なので, 各ダイアログは `DoModal` から抜けてきたら
`lParam` を空にしてもう一度このマクロを呼ぶルールにしています.


## `GetCommonFont`

[`CMainWnd`](CMainWnd.md) から「共通の等幅フォント」を受け取るマクロ.

実体を持った関数ではなく, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) に定義されたマクロです.
( なので実際には class とも無関係ですが. )

ユーザー定義の Window Message である `WM_GET_COMMONFONT` を
[`CMainWnd`](CMainWnd.md) に
[send](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-sendmessagew)
します.

この Window Message を受けた[`CMainWnd`](CMainWnd.md)の[`OnGetCommonFont`](CMainWnd.md#ongetcommonfont)は,
あらかじめ作っておいた共通の等幅フォントを返します.


## `ShowHelp`

Help を表示してもらうマクロ.

実体を持った関数ではなく, [`ChkMails.h`](../ChkMails/ChkMails/ChkMails.h) に定義されたマクロです.
( なので実際には class とも無関係ですが. )

ユーザー定義の Window Message である `WWM_SHOW_HELP` を
[`CMainWnd`](CMainWnd.md) に
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew)
します.

この Window Message を受けた[`CMainWnd`](CMainWnd.md)の[`OnShowHelp`](CMainWnd.md#onshowhelp)は,
通知された window handle から送信元 window のタイトル ( caption ) を得て,
それを添えた URL で web ページを参照します.


## `DeleteProfileValue`

不要になった registry 値を削除するための public 関数.
<p>

機能としては汎用ですが,
本アプリでは結果的に [`CMainWnd`](CMainWnd.md) からしか使っていません.
だったら [`CMainWnd`](CMainWnd.md) の中に実装すれば良さそうなものですが,
「“`Profile`” な関数は `CWinApp` に」という「様式美」に付き合ってこちらに配置されています.
この関数がこの実装の先頭に配置されているのも,
「`public` はものは `protected` なものより先に」という「様式美」によるものです.
<p>

削除に成功したら `TRUE` を, 与えられた名前の registry 値が存在しなかったら `FALSE` を返します.


## `GetVersionInfo`

与えられた種別の情報を与えられた `.exe` ファイルのバージョン情報の中から引き出して文字列で返します.

以下の種別が指定可能です.

| 種別指定 | 引き出される情報 | 例 |
| --- | --- | --- |
| `%C` | `CompanyName` | `In-house Tool` |
| `%D` | `FileDescription` | `Mail Checker` |
| `%V` | `FileVersion` | `1.0.0.1` |
| `%L` | `LegalCopyright` | `Copyright (C) 2025 In-house Tool` |
| `%P` | `ProductName` | `ChkMails` |

例えば下記左のような文字列で指定すると, 下記右のような文字列が返ります.

| 引数 | 戻り値 |
| --- | --- |
| `%P\n%D\nVersion %V\n` | `ChkMails`<br>`Mail Checker`<br>`Version 1.0.0.1` |

あと, `.exe` ファイルを指定する引数を省略すると,
デフォルトで本アプリの `.exe` ファイルを対象とします.
<br>
<sub>
ところで,
上記でやっている「本アプリの `.exe` へのパス」の求め方を意外に感じる方もいらっしゃるかもしれません.
「え、そういうのは
[`GetModuleFileName`](https://learn.microsoft.com/ja-jp/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew)
でやるもんじゃないの?」と.
でも, これで十分なんです. [実際に確かめた方もいらっしゃいます](https://hirokio.jp/visualcpp/getmodulefilename/)し.
</sub>

## `InitInstance`

一般的な
[`CWinApp::InitInstance`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#initinstance) の override です.

[「コレってホントに必要なの？」](https://stackoverflow.com/questions/726841/is-it-necessary-to-use-initcommoncontrolsex-and-initcommoncontrols)
と一部で疑われている
「[`InitCommonControlsEx`](https://learn.microsoft.com/ja-jp/windows/win32/api/commctrl/nf-commctrl-initcommoncontrolsex) を呼び出す」
というお約束を果たしてから
( [`COMCTL32.DLL` の今後の呼び出しのためにやっておくべき](https://devblogs.microsoft.com/oldnewthing/20050718-16/?p=34913)だそうなので )
, registry 名を定義しています.

それから,
<sub>
( project を新規作成する際に Windows sockets にチェックを入れ忘れると
あとからの追加が面倒なことで知られている初期化のひとつの )
</sub>
`AfxSocketInit()` の呼び出しを行っています.
<sub>
( もうひとつの追加作業は
[`framework.h`](../ChkMails/ChkMails/framework.h)
に　`#include <afxsock.h>` と書き加えること )
</sub>

以上がありがちな処理内容ですが, ここから先がユニークです.

まず, ユニークな window class 名とタイトルを生成し,
同じ window class 名とタイトルを持つ window を探して, あったらその window に
[`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) を
[post](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-postmessagew) 
して終了させます.

ありがちな「多重起動防止」手順ですが,
この手を使う際はせめて window class 名をユニークなものにしておかないと,
[無関係な window のハングに巻き込まれるリスクがある](https://www.codeproject.com/Articles/538/Avoiding-Multiple-Instances-of-an-Application)
ことでも知られています.
( 厳密には「同時起動」時の競合を考慮すべきですが, 本アプリをそんな風に起動する人は居ないので,
window class を登録するついでの処理で済ませています. )

で, 先輩に ( たぶん ) ご退場願った後,
自らが同じ window class を登録し,
その window slass 名を持つ [`CMainWnd`](CMainWnd.md) を `new` して,
`CWinApp::m_pMainWnd` として保持します.

最後に [`CMainWnd`](CMainWnd.md) を `Create()` します.
`Create()` の引数にやる気が感じられない
( style の指定はないわ, window のサイズが 0 だわ, ID も 0 だわ )
のは,
[`CMainWnd`](CMainWnd.md) が自らを表示する気がない window だからです.

常に `TRUE` ( 初期化成功 ) を返しています.


## `PreTranslateMessage`

Window message が dispatch される前に介入して独自の処理を挟み込むために用いられる
[`CWinApp::PreTranslateMessage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#pretranslatemessage)
の override です.

まず, [`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) には介入して,
[`CMainWnd`](CMainWnd.md) を
[`DestroyWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-destroywindow) した後,
[`CMainWnd`](CMainWnd.md) を `delete` します.

この処理を挟まないと,
なぜか `m_pMainWnd` が
`DestroyWindow` されず,
プロセス終了時に `m_pMainWnd` の持ち物が leak するという現象が,
本アプリのような modeless な動作をする window にはつきものです.

「じゃあ, 何のためにあるんだ? `m_pMainWnd`」という気もしますが,
`m_pMainWnd->DestroyWindow();` とやると,
なぜか `m_pMainWnd` は NULL にクリアされてしまうので,
わざわざ `CWnd* pWnd` にコピーしてからやっています.
<br>
<sup>
( それほど `m_pMainWnd` を意識しているなら, なぜ自動で `m_pMainWnd` を `DestroyWindow` しない? > MFC )
</sup>

[`WM_CLOSE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-close) に対しては
常に `TRUE` ( 処理済み ) を返します.

あと, [`WM_KEYDOWN`](https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-keydown)
等のキー入力関係の Window message が来た場合,
入力フォーカスを持っているのが Edit Box だったら,
そちらに丸投げします.
「`Ctrl`+`V` だったら paste だ」とかなんとかは,
Edit Box に判断してもらいましょう.
ウチの window で引き取っても面倒なんで.

ただし, `Enter` キー関係の Window Message だけは Edit Box 送りではなく,
通常の経路に乗せます.
「`Enter` が叩かれたら入力の完了扱いにする」なんて `PreTranslateMessage` もありがちですから.
<br>
( 現にこの実装でも
[`CDomainPage::PreTranslateMessage`](CDomainPage.md#pretranslatemessage) が似たようなことやってます. )

その他の Window Message に対しては介入する気はないので,
[`CWinApp::PreTranslateMessage`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#pretranslatemessage)
に丸投げした結果を返しています.


## `DoMessageBox`

アプリ内で
[`AfxMessageBox()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cstring-formatting-and-message-box-display#afxmessagebox)
を呼ぶと, 巡り巡って呼び出される約束になっている
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
の override です.

[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
とそっくりな動き ( ダイアログリソースなしにダイアログを表示 ) をする
[CMessageBox](CMessageBox.md)
を初期化してから
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) します.
この実装のおかげで, 本アプリの `AfxMessageBox()` は一網打尽に同じ動作をします.
その「同じ動作」とは, 「タスクバーの通知領域内のアイコンに近い場所にダイアログを表示する」というもので,
デフォルトの
[`CWinApp::DoMessageBox`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cwinapp-class#domessagebox)
のデフォルト動作である「モニターの中央にダイアログを表示する」より, 視線を動かさずに済むようになっています.

戻り値は, ユーザーがチョイスしたボタンの ID です.

