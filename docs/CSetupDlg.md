# `CSetupDlg`

## 構成

ソースコード上は,
[`CSetupDlg.h`](../ChkMails/ChkMails/SetupDlg.h) と
[`CSetupDlg.cpp`](../ChkMails/ChkMails/SetupDlg.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CSetupDlg`](#csetupdlg-1)

#### Override

[`OnInitDialog`](#oninitdialog)
[`DestroyWindow`](#destroywindow)
[`OnOK`](#onok)

#### 標準 Window Message ハンドラー

[`OnNcActivate`](#onncactivate)
[`OnGetMinMaxInfo`](#ongetminmaxinfo)

#### コマンドハンドラー

[`OnHelp`](#onhelp)
[`OnButtonRefTone`](#onbuttonreftone)
[`OnButtonRefLog`](#onbuttonreflog)
[`OnChangeEdit`](#onchangeedit)

#### 固有関数

[`IsInStartup`](#isinstartup)
[`PutInStartup`](#putinstartup)
[`GetLinkPath`](#getlinkpath)
[`CreateLink`](#createlink)
[`IsExisting`](#isexisting)


## 概要

その他の設定のための下記のようなダイアログを動かしている class です.

![](../Examples/Setup.png)

どうということはないありふれた独立ダイアログですが,
[`CListCtrl`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/clistctrl-class)
や
[`CEdit`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cedit-class)
のフォントは,
Arial のようなプロポーショナルなフォントだと何かと間違いのもと ( "rn" と "m" が見分けづらいとか ) なので,
[Main Window](CMainWnd.md) ご指定の等幅フォント ( [`GetCommonFont()`](CChkMailsApp.md) ) に入れ替えています.

あと, スクリーンの中央ではなく, タスクバー ( 上のアイコン ) に寄った端っこに出てくるのも,
[Main Window](CMainWnd.md) の差し金 ( [`NotifyPopup()`](CChkMailsApp.md#notifypopup) ) です.

このダイアログは resizable ( マウスでダイアログの隅っこを引っ張るとサイズが変えられる ) なので,
ファイル名やフォルダー名が長くて収まらない場合は適当に引き延ばしてお使いください.

以下, 本 class 内に実装された関数それぞれの説明です.

## `CSetupDlg`

この class の constructor です.

リソース ID を指定して基底クラスの constructor を呼ぶという大事な仕事を果たした後,
メンバー変数を初期化しています.
<sub>
そうしておかないと[C26495](https://learn.microsoft.com/ja-jp/cpp/code-quality/c26495)がうるさいので.
</sub>


## `OnInitDialog`

[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
の override です.

まずは普通に
[`CDialog::OnInitDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#oninitdialog)
を呼んだ後, 以下の独自処理を挟みます.

1. ダイアログのアイコンをセットする.
1. ダイアログの最初のサイズを憶えておく.
1. [`GetCommonFont`](#CChkMailsApp.md#getcommonfont) で共通の等幅フォントを得る.
1. Edit Box に等幅フォントをセットする.
1. ダイアログ上のアイテムに設定値を反映させる.
1. [`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが現われたことを通知.
1. `OK` ボタンにフォーカスを移す.
1. フォーカスを設定したので, `FALSE` を返す.

このうち, 5. で反映させている設定値は, 事前に
( この class が [`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) に入る前に )
[Main Window](CMainWnd.md) から設定されたものです.
設定値はこの class の ( 何の隠蔽も保護もない ) `public` 変数として宣言されているので,
[Main Window](CMainWnd.md) から書き放題です.

ただし, 1つだけ「上からの指示」ではなくこの class で勝手に決めているのが,
`Launch at startup` の check box です.
この設定値だけは Windows&reg; の Startup に登録されているかを当 class が
[`IsInStartup`](#isinstartup) で独自に読み取り,
その結果で ON/OFF しています.


## `DestroyWindow`

`CDialog::DestroyWindow` の override です.

[`NotifyPopup`](CChkMailsApp.md#notifypopup) を呼んでダイアログが引っ込んだことを通知する,
という手順を挟むためだけに override しています.


## `OnOK`

`CDialog::OnOK` の override です.

なんやかんや ダイアログ ( 上の各 control ) に入力された設定をかき集めて,
この class の ( 何の隠蔽も保護もない ) `public` 変数に格納します.
あとはこの class を
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal) させた
[Main Window](CMainWnd.md) が, 勝手に持って行くでしょう.
<br>
<sub>
情報を吸い上げられたあと, 本 class は [Main Window](CMainWnd.md) に棄てられます.
( こういう言い方をすると, なんか Main Window がヒドい人みたいですが. )
</sub>

ただし, 1つだけ「上への報告」なしにこの class で勝手にやっているのが,
`Launch at startup` の check box です.
この設定値だけは Windows&reg; の Startup の登録／解除を当 class が
[`PutInStartup`](#putinstartup) で独自にやっています.


## `OnNcActivate`

Window Message [`WM_NCACTIVATE`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-ncactivate)
に対するハンドラーです.

[Main Window](CMainWnd.md) のような modeless な top level window の傘下の dailog である当 class では,
`>>` ボタンを押して表示させる孫請けダイアログの
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
中でも, 自 window が操作できてしまいます.

なので, `>>` ボタンの handler でどういうタイトルのダイアログを `DoModal` させたかを憶えておき,
親 window の操作の起点となる `WM_NCACTIVATE` をキャッチして,

* 孫請けダイアログで `DoModal` 中なら, 自ダイアログへの操作は遮断し, 孫請けダイアログに注意喚起.

という段取りを挟んであります.
[Main Window](CMainWnd.md) の [`OnIconNotify`](CMainWnd.md#oniconnotify)
がやっているのと同様な「modal化」と言えます.

なお, なぜ孫請けダイアログへのポインターやハンドルではなく, タイトル ( caption ) で憶えておくのかというと,
孫請けダイアログが `DoModal` するまではポインターはアテにならず, ハンドルは存在しないからです.
`DoModal` に入ってしまうと, もう外からは介入できません.
タイトルで憶えておいて, `WM_NCACTIVATE` が来た時には
憶えておいたタイトル
( とせめてもの[リスク](https://www.codeproject.com/Articles/538/Avoiding-Multiple-Instances-of-an-Application)回避で ダイアログの window class である「#32770」も付けて ) で
[`FindWindow`](https://learn.microsoft.com/ja-jp/windows/win32/api/winuser/nf-winuser-findwindoww)
して見つけた window に注意を促しています. 
<br>
<sub>
いや, すなおに「出てきたダイアログを操作する」としていただければこんなことしなくても済むんですが,
出てきたダイアログ以外に「浮気」すると, 「そっちじゃない。こっちだ。」と注意されるわけです.
</sub>

## `OnGetMinMaxInfo`

Window Message [`WM_GETMINMAXINFO`](https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo)
に対するハンドラーです.

[`OnInitDialog`](oninitdialog) で憶えておいたダイアログの最初のサイズを,
「最小サイズ」として返します.
これにより, ダイアログの端を drag しても元のサイズより小さくはできなくなります.
小さくしても意味はないので入れておきました.
大きくする分には意味があるので不介入です.


## `OnHelp`

`Help` ボタンが押された時に飛んでくる `IDHELP` コマンドのハンドラーです.

[`ShowHelp`](CChkMailsApp.md#showhelp) に仕事を丸投げです.


## `OnButtonRefTone`

`Ring` の右の `>>` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

現在の設定に基づいてその親フォルダーを開いたところから始めます.

なにがしかの `.wav` ファイルが選ばれ, `OK` ボタンが押されると,
その `.wav` ファイルの音を鳴らすというデモンストレーションを挟みますが,
本格的に設定値に反映されるのは, 本ダイアログの `OK` ボタンが押されたときです.
<br>
<sub>
あと, 親フォルダーを開いた際にファイルの表示がみっともない ( 中途半端な幅しか選択されていない ) のは,
[前述](../README.md#notification)の通り,
[Windows&reg; のポカ](https://stackoverflow.com/questions/64424601/cmfceditbrowsectrl-is-cropping-the-file-name)
です.
</sub>

ここで開いている
[`CFileDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfiledialog-class) は
「孫請け dialog」に相当するので, その
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
中に「浮気」されないよう
[`OnNcActivate`](#onncactivate)
で見張っています.


## `OnButtonRefLog`

`Folder:` の右の `>>` ボタンが押された時に飛んでくる `BN_CLICKED` コマンドのハンドラーです.

現在の設定フォルダーを開いたところから始めます.

なにがしかのフォルダーが選ばれ, `Select Folder` ボタンが押されると,
暫定値として取り込まれますが,
本格的に設定値に反映されるのは, 本ダイアログの `OK` ボタンが押されたときです.
<br>
<sub>
[`CFolderPickerDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfolderpickerdialog-class)
というフォルダー専門のダイアログを使っているので,
[`OnButtonRefTone`](#onbuttonreftone) のように
[Windows&reg; のポカ](https://stackoverflow.com/questions/64424601/cmfceditbrowsectrl-is-cropping-the-file-name)
が露見しません.
</sub>

ここで開いている
[`CFolderPickerDialog`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfolderpickerdialog-class) は
「孫請け dialog」に相当するので, その
[`DoModal`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cdialog-class#domodal)
中に「浮気」されないよう
[`OnNcActivate`](#onncactivate)
で見張っています.


## `OnChangeEdit`

Edit Box の中身を編集すると飛んでくる `EN_CHANGE` コマンドのハンドラーです.

全ての Edit Box に何らかの文字列が入力された時点で,
つまり空っぽの欄がなくなった時点で,
`OK` ボタンをイネーブルしています.


## `IsInStartup`

本アプリが Windows&reg; の Startup に登録されているか否かを返します.

1. [`GetLinkPath`](#getlinkpash) で Startup のフォルダーを得る.
1. [`IsExisting`](#isexisting) で上記フォルダーが実在するかを得る.

としてフォルダーの有無を返すだけです.


## `PutInStartup`

本アプリを Windows&reg; の Startup で登録／登録抹消します.

処理手順は下記の通り:


1. [`GetLinkPath`](#getlinkpash) で Startup のフォルダーを得る.
1. [`IsExisting`](#isexisting) で上記フォルダーが実在するかを得る.
1. 「登録依頼」で「登録済み」なら終了.
1. 「登録抹消依頼」で「未登録」なら終了.
1. 「登録依頼」なら, 本アプリの `.exe` へのパスを Startup のフォルダーに [`CreateLink`](#createlink).
1. 「登録抹消依頼」なら,  Startup のフォルダーから本アプリへのリンクを [`DeleteFile`](https://learn.microsoft.com/ja-jp/windows/win32/api/winbase/nf-winbase-deletefile).
<br>
<sub>
ところで,
上記 5. でやっている「本アプリの `.exe` へのパス」の求め方を意外に感じる方もいらっしゃるかもしれません.
「え、そういうのは
[`GetModuleFileName`](https://learn.microsoft.com/ja-jp/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew)
でやるもんじゃないの?」と.
でも, これで十分なんです. [実際に確かめた方もいらっしゃいます](https://hirokio.jp/visualcpp/getmodulefilename/)し.
</sub>


## `GetLinkPath`

与えられた GUID におけるアプリのリンクのパスを返します.

ここで引数として与えられる [GUID](https://ja.wikipedia.org/wiki/GUID) は,
Windows&reg; の「標準フォルダー」を識別する
[`KNOWNFOLDERID`](https://learn.microsoft.com/ja-jp/windows/win32/shell/knownfolderid)
というもので, GUID を引数として渡すと該当するフォルダーの物理パスが返ってきます.

GUID が `FOLDERID_Programs` ( `Start Menu` の `Programs` に相当 ) の場合は,
さらにサブフォルダーを付加指定できます.

処理手順としては:

1. [`SHGetKnownFolderPath`](https://learn.microsoft.com/ja-jp/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath) で GUID からフォルダーパスを得る.
1. `FOLDERID_Programs` に対してサブフォルダーの指定があれば, 上記で得たパスにそれを追加.
1. アプリ名に `".lnk"` と拡張子を加えたファイル名を生成.
1. パス＋ファイル名を返す.

となっています.
つまり, 

* GUID で指定されたフォルダーにアプリへのリンクを作るとしたら, そのリンクはこういうフルパスになる

という文字列が返ります.


## `CreateLink`

本アプリの `.exe` へのリンクを作成します.

処理手順は下記の通り:

1. [`GetVersionInfo`](CChkMailsApp.md#getversioninfo)
で指定 `.exe` のバージョン情報上の「説明 ( File description )」を得る.
1. これから
[COM](https://learn.microsoft.com/ja-jp/windows/win32/com/component-object-model--com--portal)
を使う予定なので, 礼儀として ( thread 中のどこかで済ませていれば省略可だが, 一応念のため )
[`CoInitialize`](https://learn.microsoft.com/ja-jp/windows/win32/api/objbase/nf-objbase-coinitialize) しておく.
1. [`IShellLink`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nn-shobjidl_core-ishelllinkw)
インターフェイスを 1つ
[`CoCreateInstance`](https://learn.microsoft.com/ja-jp/windows/win32/api/combaseapi/nf-combaseapi-cocreateinstance).
1. 指定 `.exe` に対して
[`SHGetFileInfo`](https://learn.microsoft.com/ja-jp/windows/win32/api/shellapi/nf-shellapi-shgetfileinfow)
して `.exe` のアイコンの情報を得る.
1. `IShellLink` インターフェイスを介して `.exe` のアイコンの情報を
[`SetIconLocation`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-seticonlocation).
1. `IShellLink` インターフェイスを介して `.exe` の「説明 ( File description )」を
[`SetDescription`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setdescription).
1. `IShellLink` インターフェイスを介して `.exe` のフルパスを
[`SetPath`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setpath).
1. `IShellLink` インターフェイスを介して `.exe` の置かれたパスを「作業ディレクトリ ( working directory )」 として
[`SetWorkingDirectory`](https://learn.microsoft.com/ja-jp/windows/win32/api/shobjidl_core/nf-shobjidl_core-ishelllinkw-setworkingdirectory).
1. `IShellLink` インターフェイスを介して [`IPersistFile`](https://learn.microsoft.com/ja-jp/windows/win32/api/objidl/nn-objidl-ipersistfile) インターフェイスを
[`QueryInterface`](https://learn.microsoft.com/ja-jp/cpp/atl/queryinterface).
1. `IPersistFile` インターフェイスを介して, リンクのフルパスを
[`Save`](https://learn.microsoft.com/ja-jp/windows/win32/api/objidl/nf-objidl-ipersistfile-save).
1. `IPersistFile` インターフェイスを
[`Release`](https://learn.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-release).
1. `IShellLink` インターフェイスを
[`Release`](https://learn.microsoft.com/ja-jp/windows/win32/api/unknwn/nf-unknwn-iunknown-release).
1. [COM](https://learn.microsoft.com/ja-jp/windows/win32/com/component-object-model--com--portal)
を使い終わったので, ルールとして
[`CoUninitialize`](https://learn.microsoft.com/ja-jp/windows/win32/api/combaseapi/nf-combaseapi-couninitialize).

[COM](https://learn.microsoft.com/ja-jp/windows/win32/com/the-component-object-model)
って難解ですよねー.
さぞかし頭の良い方が考えたんでしょうねー.
<br>
<sub>
最後の発言が賞賛なのか皮肉なのかに関しては, 回答を差し控えさせていただきます.
</sub>

## `IsExisting`

与えられたファイル名のファイルが実在するか否かを返します.

単に [`CFile::GetStatus`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/cfile-class?#getstatus)
が成功したか否かでファイルの有無を答えるだけです.
