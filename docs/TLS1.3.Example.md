# TLS1.3 動作例

動きますよ. ホントに動くんです.

ホントに動いた証拠として,
動いている最中のやり取りを [Wireshark](https://www.wireshark.org/) でキャプチャーしておいた記録と,<br>
どの記録がどの局面でキャプチャーされたものなのかの説明も添えてお送りいたします.
<br>
<sup>
そうでもしておかないと, どれがなにやらだれがどこやら, わかんなくなっちゃうんですよ…….
</sup>


## 接続

サーバーと接続できたら,
まずはこちらから
[Client Hello](https://www.google.com/search?q=TLS+1.3+%22Client+Hello%22+って%3f)
を送信することになります.
その担当関数は [`CParaSocket::OnConnectTLS1()`](CParaSocket.md#onconnecttls1) です.

TLS1.3 の場合,
[Client Hello](https://www.google.com/search?q=TLS+1.3+%22Client+Hello%22+って%3f)
は下記のような内容で送ります.<br>
このイメージは
[`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
が,
`SEC_I_CONTINUE_NEEDED` を返してきたときに出力バッファに仕込んできたイメージそのものです.<br>
<sub>※ 以下このページの Wrireshark ログのイメージに添えられている ASCII 以外の文字コード ( 👈とか ) は,
説明のためにこのページで加えられたものです. Wireshark がやっているわけではありません.</sub>

```
    TLSv1.3 Record Layer: Handshake Protocol: Client Hello
        Content Type: Handshake (22)
        Version: TLS 1.0 (0x0301) 👈
        Length: 441
        Handshake Protocol: Client Hello
            Handshake Type: Client Hello (1)
            Length: 437
            Version: TLS 1.2 (0x0303)
            Random: 942ebc477711af7a31c30303ccb4ed29cc249dc3dacae014e53eaca589af0335
            Session ID Length: 32
            Session ID: 7fd6335c41342b32531cb16b34e3185a2d9025891bffc12df3b6c9076c89e1fb
            Cipher Suites Length: 40
            Cipher Suites (20 suites)
            Compression Methods Length: 1
            Compression Methods (1 method)
            Extensions Length: 324
            Extension: server_name (len=18) name=pops.provider.ne.jp
            Extension: supported_versions (len=9) TLS 1.3, TLS 1.2, TLS 1.1, TLS 1.0 👈
            Extension: signature_algorithms (len=26)
            Extension: session_ticket (len=0)
            Extension: supported_groups (len=8)
            Extension: ec_point_formats (len=2)
            Extension: key_share (len=208) x25519, secp256r1, secp384r1
            Extension: post_handshake_auth (len=0)
            Extension: extended_master_secret (len=0)
            Extension: renegotiation_info (len=1)
            Extension: psk_key_exchange_modes (len=2)
```

TLS1.2 との大きな違いは,「バージョンに対する心の広さ」ですかね.<br>
`こちら TLS1.2。サーバー、応答せよ。` と一方的だった
[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
と違って,<br>
`私は TLS1.0 ですが、TLS1.3、TLS1.2、TLS1.1、TLS1.0 も承ります。` 👈
と,
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
はへりくだって相手にバージョンを選ぶ余地を与えています.

「いや、`私は TLS1.0 ですが、`はへりくだり過ぎだろう。
『TLS1.0 および TLS1.1 禁止条約』
( というか [RFC 8996](https://datatracker.ietf.org/doc/html/rfc8996) )
を知らんのか？」<br>
と思われる向きもあるかもしれませんが, ご懸念には及びません.
相手も同じ条約を批准しています.<br>
「[`AcquireCredentialsHandle()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
に食わせる
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
の `.pTlsParameters` で、
`.grbitDisabledProtocols` を指定して古いのはマスクしておこう。」<br>
なんてことは考えなくてもいいんです.

[Client Hello](https://www.google.com/search?q=TLS+1.3+%22Client+Hello%22+って%3f)
を送り終わったら,
その応答の対応は [`CParaSocket::OnConnectTLS2()`](CParaSocket.md#onconnecttls2) に委ねます.


## 応答

TLS1.3 対応のサーバーなら下記のような
[Server Hello](https://www.google.com/search?q=TLS+1.3+%22Server+Hello%22+って%3f)
を返してきます.
<br>
こちらが提示した選択肢の中から, `TLS 1.3` を選んできます.👈

こちらが `TLS 1.3` を含めて提示しているのに,
[あえて `TLS 1.2` を選んでくるサーバー](https://www.google.com/search?q=あえて+TLS+1.2+を選んでくるサーバー)もまだまだ多いそうです.<sub>( 2026年6月現在 )</sub>
<br>
こちらが `TLS 1.3` を含めずに提示していたら,
[システム管理者の見えざる手が TSL1.3 を封印](https://www.google.com/search?q=グループポリシーで+TLS+1.3+を無効化)している可能性を考えてください.
<br>
<sup>
( あと, [Windows&reg;10 でこれをやろうとしている人. あなたは失格です](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/protocols-in-tls-ssl--schannel-ssp-). 退場! )
</sup>

```
    TLSv1.3 Record Layer: Handshake Protocol: Server Hello
        Content Type: Handshake (22)
        Version: TLS 1.2 (0x0303)
        Length: 122
        Handshake Protocol: Server Hello
            Handshake Type: Server Hello (2)
            Length: 118
            Version: TLS 1.2 (0x0303)
            Random: 848227eadc7caf667472040db085b0ee0ecaa4945c45b5ae7851a61b4bcb6e45
            Session ID Length: 32
            Session ID: 7fd6335c41342b32531cb16b34e3185a2d9025891bffc12df3b6c9076c89e1fb
            Cipher Suite: TLS_AES_256_GCM_SHA384 (0x1302)
            Compression Method: null (0)
            Extensions Length: 46
            Extension: key_share (len=36) x25519
            Extension: supported_versions (len=2) TLS 1.3 👈

    TLSv1.3 Record Layer: Change Cipher Spec Protocol: Change Cipher Spec 🚨
        Content Type: Change Cipher Spec (20)
        Version: TLS 1.2 (0x0303)
        Length: 1
        Change Cipher Spec Message

    TLS segment data (1279 bytes) 🔒
```
`Server Hello` の次に `Change Cipher Spec`🚨がくっついていますが,
これは本来不要なダミーだそうです.
通信経路上に挟まっている装置 ( ミドルボックス ) 
が[誤作動しないよう入れている配慮](https://www.bit-hive.com/articles/20230204)で,
受け取った側は無視しているそうです.
だからここでも無視してくださって結構です.

最後に `TLS segment data`🔒というのがくっついていますが,
何のデータなのかのデコード結果が添えられていません.
単に「TLS の分割データ」と言っているだけです.
デカいデータなので最後まで受け取ってみないとなんとも……ということなのでしょう.

この次のパケットを見ても,
hex ダンプには何やら大きなデータが入っているのが表示されているのに,
Ethernet フレーム, IP パケット, TCP パケットとしてのデコードがされているだけで,
その上の層については何の解説もなされていませんでした.
つまり, 「まだ続きがあって決着していない」ということなのでしょう.

こういう分割転送の「まだ途中」のイメージを
[`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
に食わせると `SECBUFFER_EXTRA` を返してきますが,
そうやって返されたものは「暗号バッファ」`m_pbEncrypted` に溜めておいて,
最後まで溜まり切ってから復号することにしています.

そして 1回クライアント側からの TCP [ACK] が挟まった後,
その次のパケットでようやく全体を送り切ったようで,
次のようなデコードが現れました.

```
    TLSv1.3 Record Layer: Application Data Protocol: Post Office Protocol 🔒
        Opaque Type: Application Data (23)
        Version: TLS 1.2 (0x0303)
        Length: 3967
        Encrypted Application Data […]: ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
        [Application Data Protocol: Post Office Protocol]
```

`TLSv1.3 Record Layer` に載った `Application Data` で, `Post Office Protocol`🔒 だと言ってます.
<sub>( 2026年6月現在 )</sub>

Post Office Protocol? もう?
いくら何でも登場が早すぎませんか?
ここまででやり取りしたのは
[Client Hello, Server Hello, Change Cipher Spec](https://www.google.com/search?q=TLS+1.3+%E3%81%AE+Client+Hello%2C+Server+Hello%2C+Change+Cipher+Spec)
だけですよね?
もうちょっと TLS1.3 的に続きがありそうな気がしますが…….

TLS1.3 的に, と言えば, ここから先はもう暗号化されているそうです.
つまり Wireshark にもデコード不能な領域に入ったということですね.
`TLSv1.3 Record Layer` に載った `Application Data` であるとだけは言えるのですが,
そのアプリケーションデータが何であるのかは Wireshark には読めません.
読めないので,

* 「TCP のポート番号が POP3 ( 995 ) のデータ」だから「`Post Office Protocol`」でイイんじゃね？

ぐらいのつもりで表現しているのでしょう.<br>
そんなきっぱりした表現ではなく, 自信なさそうに「`Post Office Protocol (みたいな?)`」とでも言ってくれれば,
こちらとしても納得しやすいのですが…….

この暗号化された `Post Office Protocol (みたいな?)` の実際の中身は,
[Certificate, CertificateVerify, Finished](https://www.google.com/search?q=Certificate%2C+CertificateVerify%2C+Finished+って%3F)
などの TLS1.3 的やり取りのデータでしょう.
本物の POP3 のコマンドやレスポンスの文字列 ( `+OK` とか ) が飛び交うのはもう少し先です.

この `Post Office Protocol (みたいな?)` を取り込むと,
[`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
が,
送り返すべきイメージを出力バッファに形成します.<br>
その形成されたイメージを送信すると,
Wireshark では下記のようにデコードされます.

```
    TLSv1.3 Record Layer: Change Cipher Spec Protocol: Change Cipher Spec 🚨
        Content Type: Change Cipher Spec (20)
        Version: TLS 1.2 (0x0303)
        Length: 1
        Change Cipher Spec Message

    TLSv1.3 Record Layer: Application Data Protocol: Post Office Protocol 🔒
        Opaque Type: Application Data (23)
        Version: TLS 1.2 (0x0303)
        Length: 69
        Encrypted Application Data […]: ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
        [Application Data Protocol: Post Office Protocol]
```

[配慮](https://www.bit-hive.com/articles/20230204)に対する配慮返しとして添えられているダミーの
`Change Cipher Spec`🚨 はともかく,<br>
その次の `Post Office Protocol (みたいな?)`返し🔒 の中身が,
TLS1.3 の
[Finished](https://www.google.com/search?q=TLS+1.3+%22Finished%22+って%3f)
なのでしょう.

上記メッセージを送り返すまでが [`CParaSocket::OnConnectTLS2()`](CParaSocket.md#onconnecttls2) の担当で,
これでいわゆる「[ハンドシェイク](https://www.google.com/search?q=TLS+ハンドシェイク)」は完了です.

次からの受信担当は [`CParaSocket::OnReceiveTLS()`](CParaSocket.md#onreceivetls) に移ります.


## 受信

[`CParaSocket::OnReceiveTLS()`](CParaSocket.md#onreceivetls) が最初に受信するメッセージは,
Wireshark では下記のようにデコードされているものです.<br>
暗号化されたままなので, 前回同様 `Post Office Protocol` とデコードされていますが,<br>
1番目に載っている方🔒 は前述の `Post Office Protocol (みたいな?)` とでも言うべきデータで,
いわゆる「[ポストハンドシェイク](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク)」のトリガーとなります.<br>
2番目に付いている方🖂 だけが本物の Post Office Protocol なメッセージです.

```
    TLSv1.3 Record Layer: Application Data Protocol: Post Office Protocol 🔒
        Opaque Type: Application Data (23)
        Version: TLS 1.2 (0x0303)
        Length: 539
        Encrypted Application Data […]: ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
        [Application Data Protocol: Post Office Protocol]

    TLSv1.3 Record Layer: Application Data Protocol: Post Office Protocol 🖂
        Opaque Type: Application Data (23)
        Version: TLS 1.2 (0x0303)
        Length: 105
        Encrypted Application Data […]: ․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․․
        [Application Data Protocol: Post Office Protocol]
```

このイメージを `SECBUFFER_DATA` として
[`DecryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel)
にかけると,
`SEC_I_RENEGOTIATE` というのが返ってきます.

この `SEC_I_RENEGOTIATE` は,<br>
TLS1.2 時代では「[再ネゴシエーション](https://learn.microsoft.com/en-us/windows/win32/secauthn/renegotiating-an-schannel-connection)してね。」という意味でしたが,<br>
TLS1.3 時代では「[ポストハンドシェイク](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク)してね。」という意味に置き換え……置き換え?

そもそも「[再ネゴシエーション](https://learn.microsoft.com/en-us/windows/win32/secauthn/recognizing-a-request-to-renegotiate-a-connection)」という概念は,
TLS1.3 で「[ポストハンドシェイク](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク)」に置き換えられたはずです.<br>
置き換えられたんだから無くなったはずです.<br>
無くなったはずなのになんで同じ `SEC_I_RENEGOTIATE` で表現するんですか!?<br>
そんなあなたがたの表現方法が, あちこちで混乱を引き起こしているのが判らないんですか?! > Microsoft&reg;

失礼, 少々取り乱しました.
で, 結局「[ポストハンドシェイク](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク)してね。」に対して,
アプリは何をすればいいのかというと,

1. [`DecryptMessage`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/decryptmessage--schannel)
が返してきた `SECBUFFER_EXTRA` を `SECBUFFER_TOKEN` として入力バッファに仕込み直す.
1. その入力バッファと空の出力バッファで
[`InitializeSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
を呼ぶ.
1. 出力バッファに何かイメージが返ってきた場合は, それも送信しておく.

ということなんだそうで,
結局 TLS1.2 時代に「[再ネゴシエーション](https://learn.microsoft.com/en-us/windows/win32/secauthn/recognizing-a-request-to-renegotiate-a-connection)」の手順としてやっていたことと同じ段取りです.<br>
「互換性」と言えばいいんですかね?
<br>
ある意味, よくできたインターフェイスであるとも言えます.
インターフェイスの向こう側に居る人々に思いが通じていたとは言えないようですが…….

この「[ポストハンドシェイク](https://www.google.com/search?q=TLS+1.3+ポストハンドシェイク)」を済ませると,<br>
以降 [`CParaSocket::ReceiveTLS()`](CParaSocket.md#receivetls) から復号済みの POP3 メッセージが引き出せるようになります.
<br>
本当の意味での [POP3 のコマンドやレスポンスの文字列](CMainWnd.md#respondpop)です.

上記 Wireshark のデコード結果の 2つ目, 🖂の方が,
POP3 サーバーが接続後に最初に送ってくる

`+OK provider.ne.jp POP3 server ready.`

といったレスポンス文字列を含むメッセージが暗号化されたものです.

ここまでくれば, 後はもう, アプリ層としては普通に POP3 なやり取りをすればいいだけです.<br>
その陰で, ソケット層が全て自動的に暗号化・復号を行っています.
<br>
<sup>
( いや, アプリ層は最初から暗号化のことなんか気にしてなかったっけ.
いいよねー上層部は. 下々の苦労なんか知らなくて. )
</sup>

そんな風に軌道に乗ったやり取りが, 切断されるまで続きます.


## 切断

ここで動作させているのは POP3 のクライアントアプリケーションなのですが,
その通信の終わりは,

1. クライアントが `QUIT\r\n` と送信.
1. サーバーが `+OK ․․․․\r\n` と返信.
1. サーバー側から TCP を `[FIN]`.
1. クライアントが TCP で `[FIN, ACK]`.
1. サーバーがダメ押しの `[ACK]`.

とたどり,<br>
特に 2. と 3. の間が 1[µs] も空いていないという速攻ぶりで TCP 層を切られてしまうので,
[TLS 層を締めくくるセレモニー](https://www.google.com/search?q=TLS+1.3+Close+Notify+とは)なんか挟んでいる暇がありません.

せいぜいハンドルリークが出ないように, 切られた後,
[`DeleteSecurityContext()`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-deletesecuritycontext),
[`FreeCredentialsHandle()`](https://learn.microsoft.com/ja-jp/windows/win32/api/sspi/nf-sspi-freecredentialshandle)
しておくぐらいが, こちらでできるせいいっぱいです.

ところで, このサーバーのいきなり失礼に見える切り方は,
最近では珍しいことではないそうで:

* TLS1.2 世代では
「[Close Notify](https://www.google.com/search?q=TLS+1.3+Close+Notify+とは)
がない切断は、[切り捨て攻撃](https://www.google.com/search?q=TLS+切り捨て攻撃)の可能性があるためエラーとみなすべき。」という風潮があった.
* TLS1.3 世代では
「アプリケーション層のやり取りからデータが最後まで正しく届いたことが明白なら、<br>
[TLS 層の終了手順](https://www.google.com/search?q=TLS+1.3+Close+Notify+とは)を省略して
TCP 層で切ってもセキュリティー上の問題が入り込む余地はない。」
と考えるようになった.

ということで, 終わり方にも「速ければ正義」の風潮が色濃く表れています.<br>
なんというか, ゆとりのない世の中になりました…….

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

[Google](https://www.google.com/) 在住の AI である Gemini さんに
「[メールサーバーでは TLS 1.3 対応と非対応どちらが多い?](https://www.google.com/search?q=メールサーバーでは+TLS+1.3+対応と非対応どちらが多い%3F)」と訊いてみたら,
```
メールサーバーの暗号化通信（STARTTLSなど）では、ウェブサーバーと比較するとTLS 1.2が主流（最も多い）であり、
TLS 1.3の対応率はまだ少数派（約30%前後）にとどまっています。
これは、メールサーバーが古いシステムや互換性重視のレガシー環境に依存しているためです。
```
とのこと. <sub>( 2026年6月現在 )</sub>

そんな中, gmail は TLS1.3 対応しているという話を聞いたので,
[gmail を POP する環境](https://www.google.com/search?q=gmail+%E3%82%92+POP+する環境)をこしらえて動作確認しました.<br>
が, [gmail にはスパムが来ない](https://www.google.com/search?q=gmail+にはスパムが来ない)し
gmail 以外はまだ TLS1.3 が通じないし, で, 導き出された結論は

「時期尚早」<sub>( 2026年6月現在 )</sub>

ちゃんちゃん.