````markdown
# philo (Philosophers)

> **I never thought philosophy would be so deadly**  
> 42 Tokyo – 学習プロジェクト

---

## プロジェクト概要

`philo` は、ダイニング・フィロソファー問題 (Dining Philosophers Problem) を C 言語と POSIX スレッド（`pthread`）、ミューテックスを用いて実装した 42 課題プロジェクトです。  
複数の「哲学者 (philosophers)」が円卓を囲み、左右のフォークを取ってスパゲッティを食べ、その後に思考・睡眠を繰り返す様子をシミュレートします。  
与えられた制約時間内に食事を開始できなければ“死亡”と判定され、ログに死亡メッセージを出力します。

---

## 機能と仕様

1. **スレッドによる哲学者の同時実行**  
   - 各哲学者は独立したスレッド (`pthread`) として動作します。  
   - スレッドごとに「思考 (thinking) → 食事 (eating) → 睡眠 (sleeping)」をループで繰り返します。

2. **ミューテックスによるフォークの排他制御**  
   - フォークは哲学者の数と同じ数だけ用意され、各フォークを `pthread_mutex_t` で保護します。  
   - 哲学者は食事前に左右のフォークをロックし、食事後にアンロックします。

3. **タイミング管理**  
   - 各パラメータ（`time_to_die`, `time_to_eat`, `time_to_sleep`）はミリ秒単位で指定します。  
   - 哲学者は `time_to_die` ミリ秒以内に次の食事を開始しないと “死亡” と判定されます。  
   - `time_to_eat` は食事に要する時間、`time_to_sleep` は睡眠に要する時間を制御します。

4. **死亡監視 (`monitor_death`)**  
   - メイン処理とは別に、監視スレッドを立ち上げて全哲学者の生存状況を定期的に確認します。  
   - いずれかの哲学者が `time_to_die` を超過すると即座に死亡メッセージを表示し、シミュレーションを終了します。

5. **制限付き食事回数 (オプション)**  
   - 5つ目の引数 `[number_of_times_each_philosopher_must_eat]` を指定すると、全哲学者が指定回数食事を終えた時点でプログラムを正常終了します。  
   - この引数がなければ、いずれかの哲学者が “死亡” するまでループを続行します。

---

## 実行環境・前提条件

- OS: Linux / macOS などの POSIX 準拠環境  
- コンパイラ: `cc` (Clang / GCC)  
- 必要ライブラリ: `pthread` ライブラリ  
- C 言語プロジェクトのコーディング規約「Norm」に準拠  
- Makefile によるビルド管理

---

## ビルド手順

```bash
# リポジトリのルート (philo/ があるディレクトリ) に移動してから
make
````

* 成功すると、実行ファイル `philo` が生成されます。

---

## 実行例

```bash
# 引数例：5 人の哲学者、time_to_die=800ms、time_to_eat=200ms、time_to_sleep=200ms、must_eat_count=3
./philo 5 800 200 200 3
```

* 出力例 (ミリ秒: 哲学者番号 アクション)

  ```
  0  1 has taken a fork
  0  1 has taken a fork
  0  1 is eating
  0  3 has taken a fork
  0  3 has taken a fork
  0  3 is eating
  ...
  800 2 died
  ```

  * 各行の形式：`<timestamp_in_ms> <哲学者ID> <メッセージ>`

    * `has taken a fork`
    * `is eating`
    * `is sleeping`
    * `is thinking`
    * `died`

* 哲学者が死亡すると、そのメッセージのみが表示され、プログラムは終了します。

---

## 使用例

1. **基本実行（哲学者が死亡するまで）**

   ```bash
   # 3 人の哲学者、time_to_die=400ms、time_to_eat=200ms、time_to_sleep=100ms
   ./philo 3 400 200 100
   ```

2. **指定回数の食事後に終了**

   ```bash
   # 5 人の哲学者、time_to_die=800ms、time_to_eat=200ms、time_to_sleep=200ms、must_eat_count=3
   ./philo 5 800 200 200 3
   ```

3. **Helgrind でデータ競合チェック**

   ```bash
   make h
   ```

4. **ストレステスト（大人数での動作確認）**

   ```bash
   make test
   ```

---

## ビルド／実行手順まとめ

```bash
# リポジトリをクローンまたは配置
git clone <your-repo-url>
cd philo

# ビルド
make

# 実行例1: 哲学者が死亡するまで
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>

# 実行例2: 一定回数食事後に終了
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> <must_eat_count>

# Helgrind を使ったデバッグ (データ競合チェック)
make h

# 大規模テスト（199 人など）
make test

# クリーンアップ
make fclean
```

---

## 開発環境

* **OS**: Ubuntu 20.04 / macOS 10.15 など
* **コンパイラ**:

  * `cc` (Clang もしくは GCC)
  * 推奨: Clang 13 以上 / GCC 9 以上
* **ツール**:

  * `make`
  * `valgrind`（Helgrind を使用する場合）

```
```
