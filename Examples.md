# 実例

以下, 実例を少し挙げておきます.

メールの実例は, [Base64](https://ja.wikipedia.org/wiki/Base64) でデコード済みのイメージで挙げています.
全部挙げると長くなるのでメールヘッダーのみ ( 本文はカット ), さらに不要な場所はカットし,
受信者に関する情報は伏せて ( 書き換えて ) ありますのでご了承ください.
<br>
<sup>
一方, 送信者に関する情報はあえてそのまま ( 無修正で ) 出しています.
なんか卑怯な気もしますが, そこをボカすと意味が通じにくくなる例が多いので…….
</sup>


## 架空の送信元を騙っているスパムメールの例

```
Discard-Reason: Time Zone Unreliable, Authentication Failure, Evasive Link Path, Link to Unreliable Domain
Embedded-Link: sagawa-hdwcqxj.com∕neclsr∕gvyihzwqm∕bhaob@bryidls.𝕝𝕠𝕟𝕖𝕨𝕒𝕪.cn
Return-Path: <Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net>
Received: from azeb.net([114.232.44.117]) by ifmta666-pr.im.network.ne.jp
    with ESMTP
    id <20241208214142235.IBOR.72595.azeb.net@imta666.provider.ne.jp>
    for <tama.the.dog@provider.ne.jp>; Mon, 9 Dec 2024 07:15:52 +0900
Message-ID: <350e7134c3d6441a3f7b885631b942a0@azeb.net>
Sender: <Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net>
From: 佐川急便株式会社 <Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net>
To: <tama.the.dog@provider.ne.jp>
Subject: 【重要】お荷物の配達について重要なお知らせ
Date: Mon, 09 Dec 2024 06:15:51 +0800
MIME-Version: 1.0
Content-Type: text/html;
    charset="utf-8"
Content-Transfer-Encoding: base64
List-Unsubscribe: <mailto:Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net?subject=unsubscribe>
Authentication-Results: mx.provider.ne.jp; spf=none
    smtp.mailfrom=Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net; sender-id=none
    header.Sender=Amazon-fwgesqjthhrpuwlbcsnonzgaj@azeb.net
```

`spf=none` で `sender-id=none` なので,
[Authentication](../README.md#authentication) が pass しなかった件数が `2` となり,
デフォルト値の `2 以上` に該当するので, 本アプリではデフォルトでハネるようなメールです.

この手のメールは,
ただ単純に `From:` をでっち上げたケースですね.
完全なでっち上げなので, このメールヘッダーから犯人を突き止める手掛かりはあまり得られません.
犯人にとってはローリスクですが, ローリターンなやり方とも言えます.
受信側のプロバイダーの「迷惑メール防止サービス」的な仕掛けでハネられるので.
<br>
<sup>
( しかし, `Amazon` うんぬんな `From:` から送っておいて `佐川急便株式会社` って,
あまりやる気のない犯人のようです. )
</sup>

この手の「単純捏造」はメールヘッダーに `DKIM-Signature:` がないのが特徴で,
慣れてくると視覚的にも見分けやすい部類です.
ただし, 普通に営業している普通の会社でも,
[`DKIM-Signature:` を採用していない会社](#dkim-signature-がない正当なメールの例)
がほんの少し ( 当方調べで 2社 ) あるので,
その有無を絶対的な判断基準と考えてはいけません.
あと, [`DKIM-Signature:` があるのにスパムメール](#実在する送信元から送られたスパムメールの例)
ということもあるので, この際 `DKIM-Signature:` にこだわるのはやめておきましょうか.

上記の例で, 本来のメールヘッダーの上に乗っかっている<br>
`Discard-Reason:` と<br>
`Embedded-Link:` は,<br>
本アプリがログ ( `.txt` の方 ) としてセーブする際に乗せているものです.
[Authentication](../README.md#authentication) が pass しなかったことは,
`Authentication Failure` で表されていますが,
他にも [Time Zone](../README.md#time-zone) や [Domain](../README.md#domain) や
[Coding](#coding) で引っかかっていることも表されています.

引っかかったメールのログには,
メールの本文中に埋め込まれていたリンクを `Embedded-Link:` として添えていますが,
このリンクの URL

`sagawa-hdwcqxj.com∕neclsr∕gvyihzwqm∕bhaob@bryidls.𝕝𝕠𝕟𝕖𝕨𝕒𝕪.cn`

が「回避的コード」で書かれています. パターン検索から逃れるためのコーディングなのでしょうか.
あと, `@` の前の
`sagawa-hdwcqxj.com∕neclsr∕gvyihzwqm∕bhaob`
に挟まっている `∕` は一般的なスラッシュ ( `/` ) ではありません.
Unicode の `U+2215` である 「Division slash」で, 数式表記用のコードです.
`sagawaなんとか.com` に見せかけたかったんですかね?

そもそも URL に `@` があるリンクにロクなものはありません.
`@` の前にそれっぽい企業の URL っぽい文字列を並べておいて,
文字列を左からちょろっと見て判断してしまう人々を騙そうとしているように見えます.
URL における `@` の右側の文字列って, ユーザー名指定の文字列なのでアクセスするサイトには無関係です.
この例の場合は, `sagawa-hdwcqxj.com∕neclsr∕gvyihzwqm∕bhaob@` をすっ飛ばして
`bryidls.𝕝𝕠𝕟𝕖𝕨𝕒𝕪.cn` にアクセスすることになります.
<br>
<sub>
( `@` を摘発条件の一つに含めようかとも思いましたが, この手のメールは他の条件でも摘発可能なケースばかりなので, 様子見中. )
</sub>

あと, 「𝕝𝕠𝕟𝕖𝕨𝕒𝕪」って, 一般的なメーラー上でクリックすると「loneway」とみなしてくれるんでしょうか?

ちなみに,
[アブない URL かどうか判定してくれる Trend Micro のサイト](https://global.sitesafety.trendmicro.com/)
にこの URL を突っ込んでみたところ,
`Dangerous`
と出ました.
`loneway.cn` と入れても判定は変わらなかったので,
やはり「loneway」かどうかの文字列検索から逃れるために「𝕝𝕠𝕟𝕖𝕨𝕒𝕪」などと言っているのだと思われます.


## 実在する送信元から送られたスパムメールの例

```
Discard-Reason: Domain Unreliable, Time Zone Unreliable, Link to Unreliable Domain
Embedded-Link: www.jreast.co.jp
Embedded-Link: play.google.com
Embedded-Link: itunes.apple.com
Embedded-Link: viewsnet.jp
Embedded-Link: viewsnet-mercard.juxinpay.cc
Return-Path: <viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc>
Received: from htbltd (166.108.193.150) by service.xys25.cc id haof8c0001gp
 for <tama.the.dog@provider.ne.jp>; Sun, 8 Dec 2024 16:13:22 -0500
 (envelope-from <viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc>)
DKIM-Signature: v=1; a=rsa-sha1; c=relaxed/relaxed; s=mykey; d=service.xys25.cc;
 h=Message-ID:From:To:Subject:Date:MIME-Version:Content-Type;
 bh=vj93wHFVRuTq6Sh1FsoBBv/VGTo=;
 b=hg3Y/rnltpu4Sng2JRrZ3t2NAPSNF3s+gm4xNWwhF+11wm7c0BERZwaFAGPP/ilXgEI+8ZV0ner1
   AbT0fi+LzNtw4wTDn/5SCB7j8QOPO8fyuTFW1rtsUSe2u1Ani3owV5l8+2o3XCHrFM64EUHj7/9b
   lHdNIsXSorBK31eS7K0=
Message-ID: <2024120905141421356@service.xys25.cc>
From: 株式会社ビューカード <viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc>
To: <tama.the.dog@provider.ne.jp>
Subject: ご利用明細更新のお知らせ
Date: Mon, 9 Dec 2024 05:13:10 +0800
MIME-Version: 1.0
Content-Type: multipart/alternative;
    boundary="----=_NextPart_000_0727_01839C3C.19B21170"
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc;
    sender-id=pass
    header.From=viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc
```

`spf=pass` で `sender-id=pass` なので,
[Authentication](../README.md#authentication) では引っかかりません.
どこにも引っかからずにユーザーまで届いてしまうスパムメールの代表格です.

なぜ, 両者が pass してしまうのかというと,
送信者である　`viewcard.infl-viewsnet.dogkin-property.tenuory-fensial@service.xys25.cc` が,
ホントに実在するからですね.
ホントに登録したドメインに設けた実在のメールアドレスから,
正規のやり方でメールを送っている例です.

ただ, ホントに送信者 ( のドメイン ) を登録してしまっているので,
当局が本気になれば犯人が割り出せてしまうタイプのやり方とも言えます.
犯人にとってはちょっとリスクが高いですね.

で, 埋め込まれていたリンク先の一つである

`viewsnet-mercard.juxinpay.cc`

を [例の Trend Micro のサイト](https://global.sitesafety.trendmicro.com/) で見てもらうと,
案の定 `Dangerous` 判定が下りました.
<br>
<sub>
[同サイト](https://global.sitesafety.trendmicro.com/) は常にデータを収集してデータベースの更新に努めていますが,
まだ誰も問い合わせたことのない新しい URL を問い合わせると, `Untested` と出ることもあります.
( やりましたね! 一番乗りです. )
あるいは, サイト側の状況によっては `Safe` と出ることすらあります.
なので過信は禁物ですが, `Dangerous` との判定が出た場合は, ホントに危険です.
</sub>

本アプリでは,

* [Domain](../README.md#domain) で `.cc` をハネる.
* [Time Zone](../README.md#time-zone) で `+8000` をハネる.

のいずれかで煩わされることはなくなります.
「区別」だけでは不十分で「差別」を必要とする理由です.
<br>
<sup>
[`.cc`](https://ja.wikipedia.org/wiki/.cc) からは要らないメールが多いですね.
過去の経緯からそんなことになってしまったようです.
</sup>


## `DKIM-Signature:` がない正当なメールの例

```
Return-Path: <55000000001383424455@yodobashi.com>
Received: from yctcue01.yodobashi.co.jp([219.127.199.87])
    by ifmta666-pr.im.network.ne.jp with ESMTP
    id <20241105065016016.MSQJ.42919.yctcue01.yodobashi.co.jp@imta666.provider.ne.jp>
    for <pochi.the.cat@provider.ne.jp>;
    Tue, 5 Nov 2024 15:50:16 +0900
Subject: ヨドバシ・ドット・コム：ご注文ありがとうございます
From: ヨドバシ・ドット・コム<thanks_gochuumon@yodobashi.com>
To: pochi.the.cat@provider.ne.jp
MIME-Version: 1.0
Date: Tue, 5 Nov 2024 15:50:15 +0900
Reply-to: ヨドバシ・ドット・コム<thanks_gochuumon@yodobashi.com>
Content-Type: multipart/alternative; boundary="1730.789415.925177"
Message-ID: <1730.789415.938071@yctcue01.yodobashi.co.jp>
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=55000000001383424455@yodobashi.com; sender-id=pass
    header.From=thanks_gochuumon@yodobashi.com
```

習慣的に `DKIM-Signature:` を目視確認していて, 「ん？」と思った事例です.
でも何の問題もない例でもあります.

いや, まあ, 単に [DKIM](https://ja.wikipedia.org/wiki/DKIM) を採用されていないだけなんですが,
ここからのメールには「ご入金」とか「ご注文」とか機微な言葉が含まれることが多いので,
もうちょっと安心感を高めていただけると, こちらとしても助かります.


## `DKIM-Signature:` があるスパムメールの例

```
Discard-Reason: Sender Faking a Domain, Time Zone Unreliable, Sender Using Aberrant Message-ID
Embedded-Link: support.creditosyfinanzas.com
Embedded-Link: www.yodobashi.com
Return-Path: <yodobashi.com@golfetcpearland.com>
Received: from golfetcpearland.com([165.154.231.111])
    by ifmta666-pr.im.network.ne.jp with ESMTP
    id <20241231182511314.VFDD.51930.golfetcpearland.com@imta666.provider.ne.jp>
    for <tama.the.dog@provider.ne.jp>; Wed, 1 Jan 2025 03:25:11 +0900
DKIM-Signature: v=1; a=rsa-sha256; c=relaxed/relaxed; s=mail; d=golfetcpearland.com;
 h=Message-ID:From:To:Subject:Date:MIME-Version:Content-Type;
 i=yodobashi.com@golfetcpearland.com;
 bh=YFERM52Cm6uxlu0DYYu+p9x2k+Td8J0FEEpdCp/Vu/o=;
 b=TRLWc6LsPUtGV1gH5bUnCWbb/JSDV4keDnXaVTemvjSPjmj+gJEk/ifF93ku9evUx7o3gkfx42dv
   TbwF/PNEghoIUgfUuw54ZYR/VmprL35qFDwXR6bsAIBxbqtmT2MBVAyE5U+ccyWSmpi3r1z2Ab9X
   w03e3D6cFDfgEOrMBoA=
Message-ID: <3E757CE93F3651EC89697303DED471A4@cEy.exndwgiji>
From: "yodabashi" <yodobashi.com@golfetcpearland.com>
To: <tama.the.dog@provider.ne.jp>
Subject: ヨドバシ・ドット・コム：「お客様情報」変更依頼受付のご連絡
Date: Wed, 1 Jan 2025 02:24:55 +0800
MIME-Version: 1.0
Content-Type: multipart/alternative;
    boundary="----=_NextPart_010_02FE_011B23C2.1C5F7320"
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=yodobashi.com@golfetcpearland.com; sender-id=pass
    header.From=yodobashi.com@golfetcpearland.com
```

こちらは, `DKIM-Signature:` があるのに, スパムメールだった例です.

[Sender](../README.md#sender) の「ドメインを装ったメールアドレス」 ( 通称「子供騙し」) によって摘発された例ですが,
「“ヨドバシ・ドット・コム”を名乗っているのだから, ひょっとして“`yodobashi.com@`”というアドレスで正しいのか?」
と一瞬誤検出を疑いました.
まあ, “`yodobashi.com@`”というユーザー名で, “`golfetcpearland.com`”などというドメインから送ってくるはずはないので,
一瞬の焦りで済みましたが.
<br>
<sub>
でも「ドット・コム」を名乗る eコマース系の事業者って多いですよね.
そのうち 1社でも送信者名に「`.com@`」と名乗っていたら,
当アプリの「ドメインを装ったメールアドレス」 ( 通称「子供騙し」) は存在意義を失います.
</sub>

で, こちらは, ご丁寧に ( 本家が採用していない ) `DKIM-Signature:` まで付けて,
`Authentication-Results:` も全て pass させているというなかなか凝った事例です.
「子供騙し」どころではありません.
さらに,
`Received:` にある `165.154.231.111` を
[IP アドレスがどこの国のものか検索してくれるサイト](https://www.iplocation.net/ip-lookup)
で訊ねると,
全会一致で「Japan」という答えが出るというシロモノです.
でも, ISP として提示されている「SCloud Pte Ltd」はシンガポールの企業ですね.
なかなか用意周到に迂回を重ねたメールのようにも思えます.


## `Authentication-Results:` が全ては pass していない正当なメールの例

```
Return-Path: <001fbh-00fna@bu034.am.arara.com>
Received: from bu035.am.arara.com([42.125.251.200])
    by ifmta666-pr.im.network.ne.jp with ESMTP
    id <20241104020037322.LCEY.41470.bu035.am.arara.com@imta1005.provider.ne.jp>
    for <pochi.the.cat@provider.ne.jp>;
    Mon, 4 Nov 2024 11:00:37 +0900
Received: from unknown (HELO) (172.18.130.206)
  by bu035.am.arara.com with SMTP; Mon, 04 Nov 2024 11:00:36 +0900
DKIM-Signature: v=1; a=rsa-sha256; c=relaxed/simple; d=sokamocka.com;
    s=20240228activecore; t=1730685636;
    bh=40M5u/DEOkAl7rNKf2L6iVyGmhb94clLgFlNT99dKVk=;
    h=From:To:Subject:Message-Id:Date:MIME-Version:Content-Type:
    Content-Transfer-Encoding;
    b=HYZvqSjzQegO320ThUEPmD5f1W4z8eOo7Zb1gV1sjvfF6Aa0NADZq5mlzcxX3FR96
    M9wGNKzZ6toJ+TO7g5FOEnaVpCy9pzGxOgHd8OB7/xt35/5xuLj9zA+Ee7EYVGEGSd
    v+ohxkiNVR2AUWMkbahinoAJFwpZs4AQObPQ6Tds=
From: 草花木果 <info@sokamocka.com>
To: pochi.the.cat@provider.ne.jp
Subject: ＼ランク割なら最大19％OFF／アクネ洗顔石けん・美白美容液など♪まとめ買いがおトク
Message-Id: <001fbh-00fna@bu034.am.arara.com>
Date: Mon, 04 Nov 2024 11:00:36 +0900
MIME-Version: 1.0
Content-Type: multipart/alternative; boundary="mimemk00"
Content-Transfer-Encoding: 8bit
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=001fbh-00fna@bu034.am.arara.com; sender-id=permerror
    header.From=info@sokamocka.com
```

`spf=pass` ですが `sender-id=permerror` なので,
[Authentication](../README.md#authentication) が pass しなかった件数が `1` となり,
デフォルト値の `2 以上` にギリ該当しないので, 本アプリではデフォルトでもハネないで済んでいるメールです.

そしてこれは,
本アプリの [Authentication](../README.md#authentication) の判定条件がややこしくなった一因のメールでもあります.

本アプリの試作版で `Authentication-Results:` で引っかかってハネられたとの話を聞いて,
メールヘッダーを目視確認してみると,
`From:` が `sokamocka.com` なのに実際の送信者が `arara.com` なのでスパムを予感させました.
で, 念のため [arara.com で検索](https://www.google.com/search?q=arara.com)してみると,
ホントに「メール配信代行業者」のようで, ようやく一安心した次第です.
愉快犯が愉快なメールをバラまいて愉快がっているのかと誤解してごめんなさい.

そもそも `sender-id=permerror` って[民主党の基準](http://archive.dpj.or.jp/special/dkim/index.html)によると,
「登録情報に誤りがある」そうですよ.
直しておいていただけませんかね?
<br>
<sup>
`Authentication-Results:` をどう読むかについて,
各サイトの解説記事はなかなかマイルドな表現をしているのですが,
なぜか[民主党](http://archive.dpj.or.jp/special/dkim/index.html)が,
割と解りやすい表現で言い切ってくれているので,
当方ではこれを「民主党基準」と呼んで, よく参照しています.
</sup>


## `Authentication-Results:` が全て pass している代行業者メールの例

```
Return-Path: <bounce-2024120717-0010355706f7@srs8057.cuenote.jp>
Received: from ml.etc-plaza.jp ([172.24.6.176]) by mailsrvp1p.ml.etc-plaza.jp
 (dourojigyousya) with ESMTP id <0SO400GVK8E0CS13@mailsrvp1p.ml.etc-plaza.jp>
 for tama.the.dog@provider.ne.jp; Sat, 07 Dec 2024 17:46:51 +0900 (JST)
DKIM-Signature: v=1; a=rsa-sha256; c=relaxed/simple; d=ml.smile-etc.jp;
    s=csrs-dkim-202406171500; t=1733561211; x=1734166011;
    bh=w1oSijv07THAvsmqtsMlkYqZrQIfIaXPLLfy6L4vuYg=;
    h=Date:From:Subject:To;
    b=FJVYDN8V2TrW2AUcwY+HvP4XqXb9Gnq/ww0ucqj8Hfu6cFkgJ1UP8X5jUHMV
    cJSvLTRQo0TP5OHywuYhWVoUQYUUYTcZ6QtLsofLwNFL77+ve9DALIfulRrTY
    PRMYqPOb03ni2GKRMGtMIA5p+ouvw4Y5SZiwKO8Rq4kPHb+mKoRw24h6nlB6D
    tYHbdHi1L1r4WtzpD7g0zt60/scDMbps7y75ontmTDOWQ2PYR7xIeBpleGifi
    XaGihhwO1noT5YWmK+7CRz2ip/clswbysx1YU/PgU6NVUWlLlZiXahRYnnwVq
    tqm4Uh6pvy0H6EOGpqrs4qflJp+aup7dCV+Lnw==
MIME-version: 1.0
Content-type: text/plain; charset=utf-8
Date-warning: Date header was inserted by mailsrvp1p.ml.etc-plaza.jp
Date: Sat, 07 Dec 2024 17:46:51 +0900 (JST)
From: ＥＴＣマイレージサービス事務局<admin@ml.smile-etc.jp>
Message-id: <447414204.314.1733561211770.JavaMail.jhetc@mailapl1>
Subject: 【重要】神戸市道路公社におけるポイント付与終了について
Content-transfer-encoding: base64
To: recipients not specified: ;
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=bounce-2024120717-0010355706f7@srs8057.cuenote.jp;
    sender-id=pass header.From=admin@ml.smile-etc.jp
```

`sender-id` が pass していないメールを見かけて,
「`sender-id` とは `From:` に基づくものだ。`sender-id` が pass しないのは,
 `From:` と送信者が異なる“配信代行メール”の宿命なのか？」と,
いくつか同様の「配信代行事例」を調べてみましたが,
「代行していても `sender-id` が pass している事例」があったので,
誤った勘繰りであってことがはっきりしました.

上記はその証拠です.

クライアントは「`admin@ml.smile-etc.jp`」ですが,
実際の送信者は代行業者の `cuenote.jp` です.
つまり, 配信代行でも `sender-id` を pass させることは, 「やればできる」たぐいの仕事であろう, と.


## `Date:` がどこの国だか判らない正当なメールの例

```
Return-Path: <202412060151061f9c3eef447347cdb925ff7a9220p0fe-C5CGKT47C4OV7@bounces.amazon.co.jp>
Received: from e242-10.smtp-out.us-west-1.amazonses.com([23.251.242.10])
    by ifmta666-pr.im.network.ne.jp with ESMTP
    id <20241206015107391.RGLW.126181.e242-10.smtp-out.us-west-1.amazonses.com@imta666.provider.ne.jp>
    for <tama.the.dog@provider.ne.jp>; Fri, 6 Dec 2024 10:51:07 +0900
Date: Fri, 6 Dec 2024 01:51:06 +0000
From: "Amazon.co.jp" <store-news@amazon.co.jp>
To: tama.the.dog@provider.ne.jp
Message-ID: <0101019399a9dda1-e9422386-72c7-49ab-9f26-37b8b554130c-000000@us-west-2.amazonses.com>
Subject: Amazonを装った不審なメール等にご注意ください
MIME-Version: 1.0
Content-Type: multipart/alternative; 
    boundary="----=_Part_8104580_445183779.1733449866650"
Authentication-Results: mx.provider.ne.jp; spf=pass
    smtp.mailfrom=202412060151061f9c3eef447347cdb925ff7a9220p0fe-C5CGKT47C4OV7@bounces.amazon.co.jp;
    sender-id=pass header.From=store-news@amazon.co.jp
```

メールの送信元は `.co.jp` なのですが,
`Date:` が `+0000` となっています.
別に英国から送ってきている訳ではなく, UTC のつもりなんだと思いますが <sub>( `(UTC)` とは書かれていませんが )</sub>.

ということで`Amazon.co.jp` と取引のある方は要注意です.

1. [Filter](../README.md#filter) の [Time Zone](../README.md#time-zone) の `+0000` は&#x2611;しない.
1. `+0000` を&#x2611;する引き換えに, `Amazon.co.jp` 関係のメールアドレスを全て
[Whitelist](../README.md#whitelist) に登録する.

のいずれかが必要になります.

上記 1. の場合は問題ありません. ただ, スパムメールが `+0000` からやってきた場合は,
[Time Zone](../README.md#time-zone) 以外の対応が必要になります.

上記 2. を試みる際には,
何が「`Amazon.co.jp` 関係の正規のメールアドレス」なのかは, 過去の受信実績からご判断ください.
以前は「正規のメールアドレス一覧」みたいなものを `Amazon.co.jp` 自身が公表していたような気がするのですが,
いま見ると, そういう情報が見つかりません.<sub>( 2025年1月現在 )</sub>

その代わりなのか
「[Amazon.co.jpが送信したEメールは、メッセージセンターでご確認いただけます。](https://www.amazon.co.jp/gp/help/customer/display.html?nodeId=G4YFYCCNUSENA23B)」
ということになっています.
しかし,
一日に何十通も届く「`Amazon`を騙ったメール」にいちいちそんな対応をしていたら時間の無駄遣いですし,
しんどくてかないません.

しょうがないので, これまでの取引の実績に基づきましょうか.
メーラーの受信来歴から `amazon.co.jp` 方面からのメールを開いて,
そのメールヘッダーを表示させてから `Ctrl`+`A` して `Ctrl`+`C` して,
[Whitelist](../README.md#whitelist) に `Ctrl`+`V` する,
といった流れで登録することになります.
<br>
<sub>
`Amazon.co.jp` がもうちょっと国別のローカライズを心掛けてくれたら, スパム対策ももうちょっと的を絞れるんですが,
`Amazon` のようないんたあなしょなるな超巨大企業には, 「国」なんて些細な単位を気に掛けるヒマはないんですかね.
「スパムが騙る人気企業ナンバーワン」( 2024, 当アプリ調べ ) に君臨する企業様としては,
もうちょっと下々の者にも気を使っていただきたいものです.
</sub>


## 古いメールの例

```
Return-Path: <CC2-X0000-273-3690581-AB4D7B52-0@mail02.victor-service.co.jp>
Authentication-Results: mgw666.provider.ne.jp; dkim=neutral (message not signed) header.i=none
Received-SPF: SoftFail (mgw666.provider.ne.jp: domain of
  user-reg@jvckenwood.com is inclined to not designate
  61.209.242.174 as permitted sender) identity=pra;
  client-ip=61.209.242.174; receiver=mgw666.provider.ne.jp;
  envelope-from="CC2-X0000-273-3690581-AB4D7B52-0@mail02.victor-service.co.jp";
Received-SPF: None (mgw666.provider.ne.jp: no sender
  authenticity information available from domain of
  CC2-X0000-273-3690581-AB4D7B52-0@mail02.victor-service.co.jp)
  identity=mailfrom; client-ip=61.209.242.174;
  receiver=mgw666.provider.ne.jp;
  envelope-from="CC2-X0000-273-3690581-AB4D7B52-0@mail02.victor-service.co.jp";
Received: from unknown (HELO mail02.victor-service.co.jp) (127.0.0.1)
  by 0 with SMTP; 17 Apr 2017 15:39:55 +0900
Date: Mon, 17 Apr 2017 15:39:55 +0900 (JST)
Message-Id: <20170417.153955263.001.002.40424041@mail02.victor-service.co.jp>
To: tama.the.dog@provider.ne.jp
Subject: 【Club JVC】 製品オーナーズメルマガ
From: JVCケンウッド　Club JVC事務局 <user-reg@jvckenwood.com>
Mime-Version: 1.0
Content-Type: text/plain; charset=ISO-2022-JP
Content-Transfer-Encoding: 7bit
```
こちらは古い ( 2017 年 ) メールの例です.
現在のメールでは,
[RFC 5451](https://datatracker.ietf.org/doc/html/rfc5451)
に基づいて `Authentication-Results:` に全ての認証結果がまとめられていますが,
こちらのメールは,
廃止された [RFC 4408](https://datatracker.ietf.org/doc/html/rfc4408) に基づいて,
[SPF](https://ja.wikipedia.org/wiki/Sender_Policy_Framework) 単独で
独自の認証結果 `Received-SPF:`
を上げていた頃のものです.

本アプリは, 今更こういうメールが来るとは考えていません.
認証結果は一網打尽に `Authentication-Results:` にあるものだと考えています.
<br>
<sup>
ちなみに, 上記の例を受け取ったとすると,
「 [DKIM](https://ja.wikipedia.org/wiki/DKIM) は pass してないなあ。」
ぐらいにしか考えません.
</sup>

しかも上記の例では `Received-SPF:` もうまくいっていません. ( この頃はそんなもんだったんです. )
なので, いまどきの基準で言えば「ハネなければならないメール」ということになりますが,
今使われているフォーマットではないので特に介入しません.

メールヘッダーも日々進歩しているようです.
もしお手持ちの古いメールを眺めることがあったら,
その点にお気を付けください.
<br>
<sup>
まさか, いまだにこんなメールを送ってくる
「[RFC 4408](https://www.ietf.org/rfc/rfc4408.txt)保存会」みたいなプロバイダーは居ませんよね?



## [Filter](../README.md#filter) の設定例

参考までに当方の設定例です.<br>
この設定でここしばらく「要らないメール」は来ていませんし, 「要るメール」は棄てられていません.

| 項目 | 設定 |
| --- | --- |
| [Authentication](../README.md#authentication) | デフォルト ( `2` ) |
| [Coding](../README.md#coding) | デフォルト ( 全て&#x2611; ) |
| [Domain](../README.md#domain) | カスタマイズ (<br> `.asia`, `.cc`, `.cn`, `.co`, `.cyou`, `.gd`, `.goog`, `.icu`, `.pl`, `.sbs`, `.se`, `.top`, `.world`, `.xyz` に&#x2611; ) |
| [Name](../README.md#name) | 必要もなかったけど~~面白がって~~評価のためにカスタマイズ ( [別ファイル参照](docs/ChkMails.Names.txt) ) |
| [Sender](../README.md#sender) | デフォルト ( 全て&#x2611; ) |
| [Time Zone](../README.md#time-zone) | カスタマイズ ( +0800 に&#x2611; ) |
| [Whitelist](../README.md#whitelist) | デフォルト ( 未使用 ) |

> [!IMPORTANT]
あくまで当方の交流範囲に基づいての設定なので, 万人が丸パクリできるものではありません.
あなたご自身の交友関係や取引関係に鑑みて設定すべきであることを申し添えておきます.
「お前が書いた通りに設定しといたら大事なメールが棄てられちゃったじゃんか！」とか言われても,
当方としては「知らんがな。」としか返しようがありません.

<div style="text-align: right;">
In-house Tool / 家中 徹
</div>
