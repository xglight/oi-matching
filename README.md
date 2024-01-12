# 对拍器

> 版本1.4.0

## 功能

### 一、利用输入文件对拍

`bch -m file1 file2 -r [-o/-l] [-n number] [-t/-s] file3`
以 file3 生成的输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -rf [-o/-l] [-n number] [-t/-s] file3 file4`
以 file4 作为 file3 的输入生成输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -fo [-o/-l] folder`
以 folder 下的所有输入文件对 file1 和 file2 进行对拍

`bch -m file1 file2 -fi [-o/-l] file3`
以 file3 作为输入文件对 file1 和 file2 进行对拍

### 二、利用输入文件对拍（文件流）

**默认两个文件的输出流为 "文件名.out"**

`bch -f file1 [-c out1] file2 [-c out2] [file3] [-n number]`
（以 file3 生成的输入文件）对 file1 和 file2 进行对拍

### 三、利用输入和答案文件对拍

`bch -mf file1 [-o/-l] file_in file_out`
以 file_in 和 file_out 作为输入和答案文件对 file1 进行对拍

`bch -mf file1 [-o/-l] [-t/-s] folder`
以 folder 下的所有输入和答案文件对 file1 进行对拍

### 四、全局设置

**储存在 config.ini 文件中**

`bch -s -til [number]` 设置时间限制（超过 TLE）（单位 s）

`bch -s -etil [number]` 设置程序运行时间限制（超过结束） （单位 s）

`bch -s -mt [number]` 设置随机生成次数

`bch -s -gl [-o/-l]` 设置输出位置

`bch -s -gm [-s/-t]` 设置是否编号

### 五、参数查看

`bch -p`

### 六、参数解释

`file1` 对拍的程序

`file2` 结果绝对正确的程序

`-o` 输出及结果储存在 `bch.exe/data` 目录下  

`-l` 输出及结果储存在 `工作文件夹/data` 目录下

`-n number` 随机生成次数

`-t` 输出文件不编号（只有一个）

`-s` 输出文件编号（有多个）

`-c` 指定输出位置（相对路径，以工作文件夹为起始）

#### 七、注意事项

1. 本程序时间精度为 100ms



若在使用过程中发现问题，欢迎改进。
