# TLS1.2 について

> [!CAUTION]
ChkMails 初版の出た 2025年初頭に [`CParaSocket`](CparaSocket.md) でアナウンスしていた古い見解です.
最新の見解は[こちら](TLS1.3.md).

結論から言えば, 本品は TLS1.2 でフィックスしており, TLS1.3 はまだサポートしておりません.

2025年 2月現在の
[Microsoft&reg; の公開情報](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/protocols-in-tls-ssl--schannel-ssp-)によると,
Windows&reg;10 (22H2) では TLS1.3 は「サポート対象外」で, TLS1.2 が「有効」だそうです.

[同公開情報](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/protocols-in-tls-ssl--schannel-ssp-)によると,
Windows&reg;11 では最初っから TLS1.3 が「有効」だそうなので,

* Windows&reg;11 機で TLS1.3 対応の[POP3S](https://ja.wikipedia.org/wiki/Post_Office_Protocol#暗号化)サーバー

という条件で実験してみました.

その結果, 通信に失敗しました.

この状況に鑑みて,

* Windows&reg; では TLS1.3 はまだ早い

と結論付け,
本品では「TLS1.2固定」に設定しております.
( Version 1.0.3.209 )


### 実験の詳細

TLS1.2 / 1.3 の切り分けは,
実装上では,
[`AcquireCredentialsHandle`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/acquirecredentialshandle--schannel)
の引数 `PVOID pAuthData` に
[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
を与えるか,
[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)
を与えるかで決まるようです.
<sub>
解りにくっ!
</sub>

[`SCHANNEL_CRED`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-schannel_cred)
のページには「代わりに[`SCH_CREDENTIALS`](https://learn.microsoft.com/ja-jp/windows/win32/api/schannel/ns-schannel-sch_credentials)を使え」
的なことが書いてあるので,
この辺の情報を信じて実装を進めると, 自然に TLS1.3 に対応することになります.

ところが, これを実際に動かしてみると,
[`OnConnectTLS2`](CParaSocket.md#onconnecttls2)
で進行が止まってしまいました.

[`OnConnectTLS2`](CParaSocket.md#onconnecttls2)
において, 最後に
[`InitializeSecurityContext`](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/initializesecuritycontext--schannel)
から返ってくる戻り値は `SEC_E_OK` で, 一見正常終了しているように見えます.
が, そのとき既に受信バッファーに溜まっているはずの POP3 メッセージを検出しません.
単に正常終了するだけです.

結果,
POP3 としては手順が進まず, こちらが何も送信しないまま時間切れでサーバーから切断されて終わります.

というわけで
例の[公開情報](https://learn.microsoft.com/ja-jp/windows/win32/secauthn/protocols-in-tls-ssl--schannel-ssp-)
はアテにならず, 現時点 ( 2025年2月現在 ) では Windows&reg;11 でも TLS1.3 のサポートは
( 少なくとも[POP3S](https://ja.wikipedia.org/wiki/Post_Office_Protocol#暗号化)においては )
まだなんだろう, との判断を下し, 本品では TLS1.2 に留めてあります.
<br>
<sub>
( 別に TLS1.2 でも困らないので, それほど TLS1.3 にはこだわっていません. 上記の経緯が気に入りませんが. )
</sub>

この実験時点での関連ライブラリーの版数は:

```
C:\Windows\System32\secur32.dll            10.0.26100.1    (WinBuild.160101.0800)
C:\Windows\System32\sspicli.dll            10.0.26100.2454 (WinBuild.160101.0800)
```

となっており,
この辺が改版されたら再確認してみる所存です.

<sup>( 本体バージョン 1.1.13.621 以降, 上記 .dll の版数というより,
「Windows&reg;10 に義理立てして TLS1.2 にとどまるの, そろそろ見直してもいいんじゃない？」という考えで,
[TLS1.3 対応](TLS1.3.md)に踏み切りました. )</sup>
