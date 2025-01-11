# `CTimeZone`

## 構成

ソースコード上は,
[`TimeZones.h`](../ChkMails/ChkMails/TimeZones.h) と
[`TimeZones.cpp`](../ChkMails/ChkMails/TimeZones.cpp)
で実装されています.

この class は以下のメンバーで構成されています.

| 型 | 変数名 | 内容 | 情報源 |
| --- | --- | --- | --- |
| `CString` | `m_strKey`	| regstry key			| [`Time Zones\` 配下の key 名](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks) |
| `CString` | `m_strOffset`	| `UTC+0900` などの時間差表現	| registry の `Display` |
| `CString` | `m_strName`	| 標準時名			| registry の `Std` |
| `CString` | `m_strCity`	| `大阪、札幌、東京` などの代表都市名	| registry の `Display` |
| `CString` | `m_strAbbr`	| `JST` などの略称			| 独自に付与 |
| `LONG`    | `m_lBias`	| UTC からの時間差[分]			| registry の `TZI` |
| `LONG`    | `m_lBiasSTD`	| UTC からの時間差[分] (標準時)		| registry の `TZI` |
| `LONG`    | `m_lBiasDST`	| UTC からの時間差[分] (夏時間)		| registry の `TZI` |
| `SYSTEMTIME` | `m_tSTD`	| 標準時の開始日時		| registry の `TZI` |
| `SYSTEMTIME` | `m_tDST`	| 夏時間の開始日時		| registry の `TZI` |
| `void`    | [`CTimeZone`](#ctimezone-1) | contructor | [C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495) 避け |
| `bool`    | [`IsDST`](#isdst) | 与えられた日時が夏時間か判定 | 独自実装 |

* 基本的にデータを格納するための class なんですが,
例外として, [`IsDST( CTime tNow )`](#isdst) という「与えた日時が夏時間か否か」を返す関数が付いています.
同じく関数としての [constractor](#ctimezone-1) には大した意味はありません.


## 概要

1つの[タイムゾーン](https://ja.wikipedia.org/wiki/等時帯)を表現する情報を封入した class です.

このタイムゾーン情報は, Windows の registry<p>

[`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones\`](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks)

の下に収められているデータベースから取得したものです.

この class の生成は, 後述の [`CTimeZones`](#ctimezones) で行われています.


## `CTimeZone`

この class の constructor です.

メンバー変数を初期化しています.
そうしないと
[C26495](https://learn.microsoft.com/en-us/cpp/code-quality/c26495)
が指摘されてウザいから, という理由で付けているにすぎません.

この段階では意味のない初期値でメンバー変数を初期化するだけで,
意味のある値でメンバー変数が埋められるのは,
実際の[タイムゾーン](https://ja.wikipedia.org/wiki/等時帯)情報が読み込まれる
[`CTimeZones::ReadTimeZones`](#readtimezones) での話となります.


## `IsDST`

与えられた日時が[夏時間](https://ja.wikipedia.org/wiki/夏時間)か否かを返します.

[`CTime`](https://learn.microsoft.com/ja-jp/cpp/atl-mfc-shared/reference/ctime-class)
形式で与えられた日時が,
( `m_tDST` に格納されている ) 夏時間の開始日時から
( `m_tSTD` に格納されている ) 標準時の開始日時までの間に入っているか否かを計算します.

この両方の「開始日時」ですが,
両者の `SYSTEMTIME` の中に下記のように格納されています.<sup>*</sup>

| メンバー | 意味 |
| --- | --- |
| `wYear` | 無意味なので常に `0` |
| `wMonth` | 夏時間／標準時間が始まるのは何月か |
| `wDay` | 夏時間／標準時間が始まるのは第何曜日か<br>( `5`の場合は最終曜日 ) |
| `wDayOfWeek` | 夏時間／標準時間が始まるの曜日 |
| `wHour` | 夏時間／標準時間が始まるのは何時か |
| `wMinute` | 夏時間／標準時間が始まるのは何分か |


この情報に基づいて今年の開始日時を計算し, 与えられた日時が両者の間に入っているかを求めます.
<br>
<br>
<sub>
*
もちろん `SYSTEMTIME` の使い方としては邪道です.
</sub><p>


# `CTimeZones`

## 構成

ソースコード上は,
[`ZonePage.h`](../ChkMails/ChkMails/ZonePage.h) と
[`ZonePage.cpp`](../ChkMails/ChkMails/ZonePage.cpp)
で実装されています.

この class は以下のメンバー関数で構成されています.

#### Constructor

[`CTimeZones`](#czonepages-1)

#### Public

[`operator[]`](#operator)
[`GetCount`](#getcount)
[`GetBias`](#getbias)

#### 固有関数

[`ReadTimeZones`](#readtimezones)
[`MakeAbbr`](#makeabbr)


## 概要

全ての [`CTimeZone`](#ctimezone) を束ねて取り扱うための class です.

プログラミング上のネーミングのテーマでは,
「同じ名前の単数形と複数形を混ぜて使うな。」と戒められることもありますが,
この場合はしょうがありません.
 [`CTimeZone`](#ctimezone) を束ねたのが `CTimeZones` なのです.

Constructor で, Windows&reg; が内蔵しているデータベースに入っている全てのタイムゾーン情報を読み出しています.
なので, メンバー変数として `CTimeZones` を所有しているだけで, いつでも各地のタイムゾーン情報を参照することができます.

現在地の時刻に基づいて各地の時刻を求める応用例については,
[CZonePage](CZonePage.md) の [`DrawLocalTime()`](CZonePage.md#drawlocaltime) などをご参照ください.

## `CTimeZones`

この class の constructor です.

[`ReadTimeZones`](#readtimezones) を呼び出して,
[`CTimeZone`](#ctimezone) 配列を構成します.


## `operator[]`

[`CTimeZone`](#ctimezone) 配列を参照するための `[]` の operator です.


## `GetCount`

[`CTimeZone`](#ctimezone) 配列の要素数を返します.


## `GetBias`

[タイムゾーン](https://ja.wikipedia.org/wiki/等時帯)の略称からバイアス値<sup>*</sup>を得ます.

普通, メールヘッダーの送信日時は

`Date: Wed, 11 Dec 2024 00:17:26 +0000 (UTC)`

のようにバイアス値<sup>*</sup>とタイムゾーン略称を併記するものですが, たまに

`Date: Mon, 23 Dec 2024 09:55:02 GMT`

というタイムゾーン略称だけで済ませている横着な場合もあります.
<br>
<sup>
(「“GMT”だよ、解るだろ? 当然。」という感じがエラそうでなんかヤですね.
だいたい“GMT”じゃなくて“UTC”と書いてもらえませんかね? )
</sup>

こういう場合に, その略称から数値としてのバイアス値<sup>* </sup>を得るための関数です.
結局,
[`CTimeZone`](#ctimezone) 配列を総ナメして該当する [`CTimeZone`](#ctimezone) を探り当て,
そのバイアス値<sup>*</sup>を参考に計算した値を返しています.

<sub>*
UTC との時間差を表す「Microsoftタイムゾーン業界用語」. [ISO](https://ja.wikipedia.org/wiki/ISO_8601) や
[RFC](https://datatracker.ietf.org/doc/html/rfc3339) 業界用語では「offset」.</sub>
<br>
<sup>
　例えば中国の場合は `+0800` で統一されています. ( あんなに東西に長いのに. )
</sup>


## `ReadTimeZones`

[`CTimeZone`](#ctimezone) 配列の中身を詰めます.

[`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones\`](https://learn.microsoft.com/en-us/windows/win32/api/timezoneapi/ns-timezoneapi-time_zone_information#remarks)
に入っている全てのタイムゾーン情報を読み出しています.

ソースコードを検索しても,
`m_tSTD` や `m_tDST` を読み取っている気配が感じられないのですが,
それは `m_lBias` から始まる構造をまるまるバイナリーとして読み取っているからです.
そういう手抜きな読み取り方ができるよう,
[`CTimeZone`](#ctimezone) の `m_lBias` 以下の情報構造は,
registry に入っているまんまの構造をマネしています.

## `MakeAbbr`

与えられた [`CTimeZone`](#ctimezone) に[タイムゾーン](https://ja.wikipedia.org/wiki/等時帯)の略称を入れます.

Windows&reg; の中にも特にタイムゾーン略称のデータベースなんて実装されていないようなので,
仕方なく手作りした表を参照して入れています.

アルファベットでせいぜい 3,4 文字の表記なので当然ぶつかるわけですが
( 特に「CST」とか 「China Standard Time」なのか「中部標準時」なのか…… ),
そういう場合には
「バイアス値表記をはしょって略称だけで言ってきそうな国と地域」
を実績と独断に基づいて厳選しています.