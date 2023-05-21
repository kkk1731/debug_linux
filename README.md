# debug_linux

# myLKM
myLKM を使うことで、システムコール内で実行する関数を動的に変更することができます。
つまりLinuxを再起動することなくシステムコールの中身を変える的なことです。

Loadable Kernel Module(LKM)を使うことでそれを実現しています。具体的には関数ポインタを書き換えているのですが、セキュリティ的にだめなことやってそうなのであくまでデバッグ専用に使っています。

## How to use myLKM
事前準備として、なにかしらのヘッダファイル（サンプルでは include/linux/mm_types.h) に "my_dlink_func" という関数ポインタを定義しておく

1. myLKM/main.c の dlink_alt() 関数の中身を好きなように書き換える
2. myLKM ディレクトリで "make" コマンドを実行してコンパイル
3. myLKM の親ディレクトリで myLKM_dynamic.sh を実行
  1.モジュールをインストールする場合は"1"を入力、その後呼び出す関数を選択(サンプルでは関数1しか作ってないので"1"を入力)
  2.モジュールを元に戻す場合は"2"を選択

するとシステムコール "dlink_test"で呼び出される関数の内容が変わっているはず。
