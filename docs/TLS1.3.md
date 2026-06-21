# TLS1.3 について

TLS1.3 って TLS1.2 に比べて,
なんというか, プロトコルとしての行儀が悪いんです.
土台となる話が決着する前に, その先の話を進めようとしたりするんです.
まるで事務方が事前準備を終えるのを待たずに, 前倒しで会談に乗り込んでくる大統領みたいにせっかちです.
<sub>(※単なるたとえ話です。実在する大統領とは一切関係ありません。)</sub>

そう, [せっかち](https://www.google.com/search?q=TLS+1.3+は+TLS+1.2+よりせっかち)なんです.
「速ければ正義」
とでも思ってるんですかね?
なので TLS1.2 に慣れた人ほど「ええーっ！ここでソレ？」とか段取りの違いに戸惑います.

それに加えて, Windows&reg; に入っている TLS を使うための実装 `secur32.dll` の仕様がヒドいんです.
いくら Microsoft&reg; の「これは仕様です。」に慣れた人でも「ええーっ！ソレが仕様？」とか常識の違いに戸惑います.

そもそも実現可能なミッションなのか? と
[Bing 在住の AI である Copilot さん](https://copilot.microsoft.com/)に
```
Windows の secur32.dll を使って TLS 1.3 対応に成功した人って居る？
```
と, 身も蓋もないことを訊いてみたところ,
```
結論だけ先に言うと、
secur32.dll（＝SSPI 経由の Schannel）を使って TLS 1.3 を実装できたという成功報告は、
現時点で確認できる範囲では “ほぼ存在しない” です。
```

と, 言い切りやがりました. <sub>( 2026年6月現在 )</sub>

[Microsoft Q&A](https://learn.microsoft.com/en-us/answers/) ( 旧“Microsoft コミュニティ”) での状況も

```
・SCH_CREDENTIALS を使って TLS 1.3 を有効化
・AcquireCredentialsHandle は成功
・しかし 接続確立直前で切断される
```

とのことで,
当アプリでも[以前軽く試して「ダメだこりゃ。」と判断した箇所](TLS1.2.md)でみなさん挫折しているようです.

そもそもなんでそんなことになるのか
[Google](https://www.google.com/) 在住の AI である Gemini さんの見解も聞いてみたところ,

```
Microsoft が TLS 1.3 を Schannel に実装した際、
「API（関数）を新設せず、既存の古い 1.2 向けの関数（InitializeSecurityContext や DecryptMessage）に、
無理やり TLS 1.3 の非同期プロトコルをねじ込んだ」 からです。
```
とかなり辛辣です. <sub>( 2026年6月現在 )</sub>

というわけで, TLS1.2 を TLS1.3 に更新する際には:

* 全く目新しい登場人物が現れるわけではない.
* しかし, 見慣れた登場人物が見慣れない役を演じる.

という心づもりで臨むのがよさそうです.

そんなことを念頭に置きつつ,
以下, クライアントとしてサーバーと TLS1.3 でやり取りするための手順です.

> [!NOTE]
> なお, ここで「`secur32.dll` のアレ」扱いしているのは,
このサービスを利用するためにプログラムから呼び出している API
[`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
や
[`DecryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel)
の説明ページのいちばん下に添えられている「必要条件」のところにある DLL から来ています.<br>
> <sup>( 厳密にはこの DLL の中に TLS の機能が入っているわけではなさそうですが, こちらから呼び出す API はこの DLL に入っているので. )</sup>
<br>
> この呼び方で話が通じなかったら「[Schannel](https://learn.microsoft.com/ja-jp/windows/win32/com/schannel)
のアレ」とか「[SSPI](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/sspi)
のアレ」とか, 表現方法を変えてみた方がいいかもしれません.<br>
> <sup>( Gemini さんや Copilot さんには「secur32 のアレ」で話は通じましたけどね. 
あと, [「secur32」ですよ.「secure32」じゃないです](https://devblogs.microsoft.com/oldnewthing/20110721-00/?p=10093)からね. )</sup>


## 接続

クライアントとしてサーバーに接続を試みるところまでは, 普通の TCP/IP 接続と変わりません.
TLS 独自の対応が登場するのは,
こちら側の接続にサーバーが応じてくれたとき
( [`select()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-select) や
[`WSAAsyncSelect()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-wsaasyncselect)
で待ち受けたネットワークイベントの `FD_CONNECT` が来たとき, あるいは
[`CAsyncSocket::OnConnect()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onconnect)
が呼び出されたとき )
からです.

このとき,
[`AcquireCredentialsHandle()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
を呼んで TLS としての接続を開始しますが,
その引数 `PVOID pAuthData` に<br>
TLS1.2 では
[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
を与えていたのに対し,<br>
TLS1.3 では
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
を与えるんだそうです.<br>
なんて解りにくいんでしょう. ( なんて憶えにくいんでしょう. なんて間違えやすいんでしょう. )

接続時の流れをまとめると下記の通りです. ( TLS1.3 対応で増えたところには🚩を立てておきました. )

1. Windows&reg;11なら [`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
ではなく
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
を選ぶ. 🚩
1. 選んだ方を [`AcquireCredentialsHandle()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
に渡して TLS 通信を準備.
1. [`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
を呼んで, TLS 通信を開始.
1. `SEC_I_CONTINUE_NEEDED` が返ってきたら,
出力バッファに Client Hello のイメージが出来上がっているのでそれを送信.
1. 送信が済んだら, [`FreeContextBuffer()`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer)
を呼んで, バッファーを解放.

違いは
[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
か
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
かだけなのですが,
[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
のページには「代わりに
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
を使え」的なことが書いてあるので,
それにすなおに応じた実装者は, 知らず知らずのうちに TLS1.3 に導かれることになります.

ところが, そのすなおな実装を実際に動かしてみると, 途中で進行が止まってしまいます.
<sub>すなおな人をダマすなんて, ひどい!</sub>

どこで進行が止まるのかというと,
サーバーからの応答があったところで立ち往生するんです.


## 応答

こちら側の接続にサーバーが応じて何か送信してくれたとき
( [`select()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-select) や
[`WSAAsyncSelect()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-wsaasyncselect)
で待ち受けたネットワークイベントの `FD_READ` が来たとき, あるいは
[`CAsyncSocket::OnReceive()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onreceive)
が呼び出されたとき )
ですが,
ここにも TLS1.3 独自の対応が必要です.

処理の流れは, まず出だしで,

1. ソケットを介してサーバーからの暗号を受信.
1. 受信メッセージを
[`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel).
1. `SEC_E_OK` か `SEC_I_CONTINUE_NEEDED` が返ってきていたら, とりあえず何もしない. 🚩
( 後で出力バッファの中身を送信するため ) 👈
1. `SEC_E_INCOMPLETE_MESSAGE`が返ってきたら, 次の受信を待つためこの場は終わりにする.
1. その他が返ってきていたら, 現状を破棄するために「失敗」と判断して, 後の手順はスキップ.
1. 入力バッファの `SECBUFFER_EXTRA` じゃなかった分を暗号バッファから除く.
1. 戻り値が `SEC_E_OK` だったら,
[`QueryContextAttributes()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/querycontextattributes--schannel) でブロックサイズ等を取得.
<br>あと, アプリ層にも「つながった」と通知.

とした上で,

「失敗」していなければ:

* 出力バッファに何か出来上がっていたら, それを送信して [`FreeContextBuffer()`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer).🚩

「失敗」だったら:

* TLS 関連のハンドルを後片付け.

という流れにしてあります.

TLS1.2 では👈のところで 「`SEC_I_CONTINUE_NEEDED` だったら」という条件で,
出力バッファに形成された送信メッセージを返信していました.
戻り値 `SEC_I_CONTINUE_NEEDED` は「返信してね。」という意味に 1対1 で対応していたのです.

TLS1.3 では,
`SEC_E_OK` でも出力バッファに送信メッセージが形成されるようになりました.
この場合 `SEC_I_CONTINUE_NEEDED` は返ってきません.
「返信してね。」を待っているつもりでいると待ちぼうけになります.

でも TLS1.2 のサーバーを相手にしている場合は,
従来通り `SEC_I_CONTINUE_NEEDED` が返ってくるんです.
こういうの, 「互換性」っていうんですかね?
そのおかげで `SEC_I_CONTINUE_NEEDED` 1点買いの旧処理系は,
新たに TLS1.3 のサーバーに対峙して初めて, 待ちぼうけという現象に気づくことになります.
でも TLS1.2 のサーバー相手なら今まで通り動きます.
だから処理系は正しいはずだと思えるのです. 思えてしまうのです.

前に進もうとしているあなたの足を引っ張っているのは, 「互換性」という名の居心地の良い罠なのです.

そんな罠から脱するため, ここでは「動いていた処理系」をあえて見直し,

* `SEC_E_OK` か `SEC_I_CONTINUE_NEEDED` の 2点買いに変更. その他が出たら「失敗」.
* `SEC_E_OK` のときは, ブロックサイズ等を取得.

で一旦処理を区切り, その後に改めて

* 出力バッファにメッセージが形成されていたら, それを送信. ( `SEC_E_OK` か `SEC_I_CONTINUE_NEEDED` かに関わらず )

という 2段構えの流れに組み直しました.
<br>
それでようやく前に進むことができたのです.

……しかし,
最大の罠はこのさらに先,
データの受信時に潜んでいました…….


## 受信

ここでいう「受信」とは,
TLS の接続直後の儀式が終わって,
いよいよアプリケーションデータをやり取りしようというフェースに入った後の受信
( [`select()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-select) や
[`WSAAsyncSelect()`](https://learn.microsoft.com/ja-jp/windows/win32/api/winsock2/nf-winsock2-wsaasyncselect)
で待ち受けたネットワークイベントの `FD_READ` が来たとき, あるいは
[`CAsyncSocket::OnReceive()`](https://learn.microsoft.com/ja-jp/cpp/mfc/reference/casyncsocket-class#onreceive)
が呼び出されたとき. 応答と同じなのでフェーズ管理が必要 )
ですが,
ここにも TLS1.3 独自の対応が必要です.

まずは,

* 暗号バッファがいっぱいだったら, 何もせずに帰る.
* ソケットを介してサーバーからの暗号を受信.
* 何も受信できなかったら帰る.

という露払いの後,
受信した暗号を全て飲み込み終わるまで,
下記の段取りを続けます.

1. `SECBUFFER_DATA` として暗号バッファを
[`DecryptMessage()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel).
1. `SEC_E_OK` が返ってきた場合:
	1. 入力バッファに `SECBUFFER_DATA` として上がってきたデータがあったら,<br>復号済みバッファに追加.
	1. 入力バッファの `SECBUFFER_EXTRA` じゃなかった分を暗号バッファから除く.
	1. 「なんか受信した」とアプリ層に通知.
1. `SEC_I_CONTEXT_EXPIRED` が返ってきた場合:
	1. 暗号バッファをクリアして撤収. ( これ以上やってもムダなので )
1. `SEC_E_INCOMPLETE_MESSAGE` が返ってきた場合:
	1. 暗号バッファはそのままでこの場は終了. ( 次の受信でデータが満ちるのを期待 )
1. `SEC_I_RENEGOTIATE` が返ってきた場合:🚩
	1. 入力バッファに `SECBUFFER_EXTRA` として上がってきたデータがあったら,🚩
	1. それを新たな `SECBUFFER_TOKEN` として [`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel).🚩
	1. その結果, `SEC_E_OK` か `SEC_I_CONTINUE_NEEDED` が返ってきたら,<br>
入力バッファの `SECBUFFER_EXTRA` じゃなかった分を暗号バッファから除く.<br>
出力バッファの内容を送信して [`FreeContextBuffer()`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecontextbuffer). 🚩
	1. それ以外が返ってきたら,<br>「失敗」と判定.
1. それ以外が返ってきた場合:<br>
	「失敗」と判定.
1. 「失敗」と判定された場合:<br>TLS のハンドルを片付けて終了.

この `SEC_I_RENEGOTIATE` ( `0x00090321` ) が最大の敵です.<br>
TLS1.3 を攻略すべく戦っている兵士たちの間でも, 時折「戦場の怪談」のように噂されていました.

「いよいよというところで、 `SEC_I_RENEGOTIATE` というのが現れるらしい。」<br>
「リネゴシエート？バカな、TLS1.3 に再交渉はないはずだ。」<br>
「じゃあ、なんか違う意味か？」<br>
「その `0x00090321` って、値としては `SEC_E_BUFFER_TOO_SMALL` ( `0x80090321` ) の `Sev` ビット落ちだよな？」<br>
「なんか足りないってことか？」<br>
「わからん！だが、こいつが現れると、その先に一歩も進めなくなるんだ！」

前線は大混乱です.
TLS1.3 にマジメに取り組んでいる人ほど罠にハマります.
<br>
ここで `SEC_I_RENEGOTIATE` という戻り値で表現されているのは,

* *サーバーから「[ポストハンドシェイク制御メッセージ](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク制御メッセージ)」が来たから、*<br>
この EXTRA を TOKEN にして新しいコンテキストを起こしてね。

ということなんだそうです.

……判るかぁぁぁっ！

いや, 再ネゴシエーションのあった TLS1.2 では,
[そんな話](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/recognizing-a-request-to-renegotiate-a-connection)や,
[こんな話](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/renegotiating-an-schannel-connection)もあったけど,
TLS1.3 になってもそんな意味でその仕様を流用(?)しているとは…….<br>
<sup>
( そもそも TLS1.3 業界用語の「ポストハンドシェイク」とは,
[TLS1.2 の「リネゴシエーション」を置き換えるため](https://github.com/tsuyopon/memo/blob/master/TLS/extension/RenegotiationInfo.md)に導入された仕組みのはずです.
違いますか? そうですよね! それなのに【以下検閲により削除】)
</sup>

ある意味「互換性を保った」と言えなくもないやり口ですが,
「まさかコレなはずはないし……」と最初に除外したソレが実は正解だったことが判ったときの驚き,
虚しさ,
そして怒り…….
「こんな思いまでして、なんで secur32 でなきゃいけないんだ？」
と自分の居場所を見失いそうになります.
<br>
<sup>
( なぜかというと, 「何も追加インストールとかしなくても素の Windows&reg; で動くから」だろ? 見失うな! しっかりしろ! )
</sup>

## 結末

そんなわけで,

* マジメに取り組んだ人ほどダマされる.
* マジメに取り組まない人は見切りをつける. <sub>( そして OpenSSL とか他の実装に逃げる. )</sub>

と,
secur32 では誰も幸せになれませんでしたとさ.

secur32 で成功したという話が`“ほぼ存在しない”`のは, こういうわけだったのです.

とっぴんぱらりのぷう.

&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  
&nbsp;  

## おまけ

※ このままだと後味が悪いので, [動作例](TLS1.3.Example.md) もご参考にどうぞ.
動いたから「[動作例](TLS1.3.Example.md)」なんですよ.

※ 冒頭で紹介した `ほぼ存在しない` は, ついうっかり「成功したよ。」と報告しちゃったもんだから,
その後は
```
結論だけ先に言うと、Windows の secur32.dll（＝SSPI／Schannel）を使って TLS 1.3 に“成功した”という報告は、
Windows 11 と Windows Server 2022 以外では確認されていません。
```
と表現されるようになりましたとさ. <sub>( 2026年6月現在 )</sub>

めでたし, めでたし.