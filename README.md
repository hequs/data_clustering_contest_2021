# Data Clustering Contest 2021

Telegram Data Clustering Contest 2021 solution by [Dark Lizard](https://contest.com/user/Dark-Lizard) 🦎
- [Round 1](https://contest.com/docs/dc2021-r1) (February, 2021) - 1st  place - 🥇
- [Round 2](https://contest.com/docs/dc2021-r2) (May, 2021) - 2nd place - 🥈

Also see our prior Data Clustering Contest [submissions](https://github.com/IlyaGusev/tgcontest)!

## Install
Prerequisites: CMake
```
$ sudo apt-get install cmake libicu-dev
```

DVC install:

```
$ sudo wget https://dvc.org/deb/dvc.list -O /etc/apt/sources.list.d/dvc.list
$ sudo apt-get update
$ sudo apt-get install dvc
```

For MacOS
```
$ brew install cmake icu4c
```

Prepare all
```
git clone https://github.com/hequs/data_clustering_contest_2021.git
cd data_clustering_contest_2021
git submodule update --init --recursive

dvc pull
```

Build
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
(mac) CMAKE_PREFIX_PATH=/usr/local/opt/icu4c cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
(mac) CMAKE_PREFIX_PATH=/usr/local/opt/icu4c cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TOOLS=True -DBUILD_TESTS=True .. && cmake --build .
```

Run
```
cd ..
mkdir output
./build/libtgcat-tester/tgcat-tester language data/dc0206-input.txt output/dc0206-input.txt
./build/libtgcat-tester/tgcat-tester language data/dc0206-input.txt output/dc0206-input.txt resources/langdetect.ftz resources/topicdetect_zero_shot_baseline.ftz
```

## Other contestants
* Round 2
  * IV place
    * Sharp Sloth: [github](https://github.com/thehemen/dc-contest-2021-r2)
* Round 1
  * III place
    * Sharp Sloth: [github](https://github.com/thehemen/dc-contest-2021)
  * IV place
    * Tanned Gull: [github](https://github.com/artkulak/telegram_categorization), [article](https://artlabs.tech/2021/10/06/artlabs-team-got-4th-place-in-telegram-data-clustering-contest/)
