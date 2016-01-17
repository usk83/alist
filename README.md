alist - anchor list
=======================================================

2015年度秋学期  
ネットワークプログラミング（システムプログラミング基礎） 最終課題

IPアドレス(v4, v6), url, port, pathの入力を受け付けて、入力された先のページに含まれるリンク一覧

1. オプション処理
2. URLの入力受付（オプションで指定されていない時）
3. URLをパースしてホスト部、ポート番号、ディレクトリなどにわける
4. 上記にしたがってソケットを作成
5. 接続しHTTPリクエストを送る
6. html（レスポンス）をパース
7. title、リンク一覧の表示

## 使い方

```
usege: ./alist [-hi] [-l level] [-u url]
  -h: help
  -i: show detailed info
  -l: recursion depth (Future work)
  -u: set the target URL
```

## ファイル構成

- alist.c, alist.h (メインプログラム)
- macro.h (汎用マクロの定義)
- myhtmlparser.c, myhtmlparser.h (リクエストのパース関連)
- mynetworks.c, mynetworks.h (ネットワーク関連)
- myopt.c, myopt.h (オプション関連)
- Makefile
