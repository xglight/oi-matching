# 对拍器

> 版本1.1.0

## 功能

### 一、利用输入文件对拍

`bch -m file1 file2 -r [-o/-l] [-n number] [-t/-s] file3`
以 file3 生成的输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -rf [-o/-l] [-n number] [-t/-s] file3 file4`
以 file4 作为 file3 的输入生成输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -fo [-o/-l] folder`
以 folder 下的所有输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -fi [-o/-l] file3`
将要以 file3 作为输入文件对 file1 和 file2 进行对拍

### 二、利用输入和答案文件对拍

`bch -mf file1 [-o/-l] file_in file_out`
以 file_in 和 file_out 作为输入和答案文件对 file1 进行对拍

`bch -mf file1 [-o/-l] [-t/-s] folder`
以 folder 下的所有输入和答案文件对 file1 进行对拍

### 三、参数解释

`-o` 输出及结果储存在 `bch.exe` 目录下  

`-l` 输出及结果储存在 `工作文件夹/data` 目录下

`-n number` 随机次数

`-t` 输出文件不编号（只有一个）

`-s` 输出文件编号（有多个）