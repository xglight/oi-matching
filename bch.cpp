#include <stdio.h>
#include <direct.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#include <windows.h>
typedef double db;
using namespace std;
class ini_file {
public:
    void write_ini_val(char *file, char *info, char *name, int val) {
        char val_str[105];
        sprintf(val_str, "%d", val);
        WritePrivateProfileString(info, name, val_str, file);
    }
    void write_ini_str(char *file, char *info, char *name, char *val) {
        WritePrivateProfileString(info, name, val, file);
    }
    int find_ini_val(char *file, char *info, char *name) {
        return GetPrivateProfileInt(info, name, 0, file);
    }
    char *find_ini_str(char *file, char *info, char *name) {
        char *str = new char[105];
        GetPrivateProfileString(info, name, "", str, 105, file);
        return str;
    }
};
string exe_path, work_path;
char *get_exepath() {
    static char szFilePath[MAX_PATH + 1] = {0};
    GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
    (strrchr(szFilePath, '\\'))[0] = 0;
    return szFilePath;
}
char *get_workpath() {
    char buff[250];
    _getcwd(buff, 250);
    char *work_path(buff);
    return work_path;
}
bool is_file(char *name) {
    ifstream f(name);
    return f.good();
}
char *ini_path = new char[105];
string in_path, out_path;
bool generate_mode, generate_location;
int times, time_limit, exe_time_limit;
ini_file ini;
void create_ini(char *ini_path) {
    char info[105], name[105], str[105];
    strcpy(info, "file-path"); // file-path
    strcpy(name, "path"), strcpy(str, get_exepath());
    ini.write_ini_str(ini_path, info, name, str);
    strcpy(info, "settings"); // settings
    strcpy(name, "generate_location"), strcpy(str, "-o");
    ini.write_ini_str(ini_path, info, name, str);
    strcpy(name, "match-times");
    ini.write_ini_val(ini_path, info, name, 1);
    strcpy(name, "generate_mode"), strcpy(str, "-t");
    ini.write_ini_str(ini_path, info, name, str);
    strcpy(name, "time_limit");
    ini.write_ini_val(ini_path, info, name, 1);
    strcpy(name, "exe_time_limit");
    ini.write_ini_val(ini_path, info, name, 10);
}
void set_ini() {
    sprintf(ini_path, "%s\\config.ini", get_exepath());
    if (!is_file(ini_path)) create_ini(ini_path);
    char info[105], name[105], t[105];
    strcpy(info, "settings"); // settings
    strcpy(name, "generate_location");
    strcpy(t, ini.find_ini_str(ini_path, info, name));
    if (strcmp(t, "-o") == 0)
        generate_location = 0;
    else
        generate_location = 1;
    strcpy(name, "match-times");
    times = ini.find_ini_val(ini_path, info, name);
    strcpy(name, "generate_mode");
    strcpy(t, ini.find_ini_str(ini_path, info, name));
    if (strcmp(t, "-s"))
        generate_mode = 0;
    else
        generate_mode = 1;
    strcpy(name, "time_limit");
    time_limit = ini.find_ini_val(ini_path, info, name);
    strcpy(name, "exe_time_limit");
    exe_time_limit = ini.find_ini_val(ini_path, info, name);
}
int char_int(string c) {
    int r = 0;
    for (int i = 0; i < c.size(); i++)
        r = r * 10 + (c[i] - '0');
    return r;
}
string int_char(int x) {
    string s;
    while (x > 0) {
        s = char((x % 10) + '0') + s;
        x /= 10;
    }
    s = '_' + s;
    return s;
}
void prepare() {
    exe_path = get_exepath(), work_path = get_workpath();
    set_ini();
    if (generate_location == 0) {
        string folder = exe_path + "\\data";
        if (_access(folder.c_str(), 0) == -1) _mkdir(folder.c_str());
        out_path = in_path = folder;
    } else {
        string folder = work_path + "\\data";
        if (_access(folder.c_str(), 0) == -1) _mkdir(folder.c_str());
        out_path = in_path = folder;
    }
    struct _timeb T;
    _ftime(&T), srand(T.millitm);
}
string ins;
void help() {
    ifstream in;
    char t[105];
    sprintf(t, "%s\\help.txt", exe_path.c_str());
    in.open(t);
    while (in.getline(t, 1000))
        cout << t << endl;
    in.close();
}
int ac, wa, tle, mle;
db ti;
clock_t start, ed;
void diff(string s1, string s2) {
    ifstream in1(s1);
    ifstream in2(s2);
    string out, ans;
    int cnt = 0;
    while (in2 >> ans) {
        cnt++;
        if (in1.good())
            in1 >> out;
        else {
            if (ans != "") {
                printf("\033[1;31mWA\033[0m\ntoo short\n"), wa++;
                return;
            }
            while (in2 >> ans)
                if (ans != "") {
                    printf("\033[1;31mWA\033[0m\ntoo short\n"), wa++;
                    return;
                }
        }
        if (out != ans) {
            printf("\033[1;31mWA\033[0m\n"), wa++;
            printf("行数: %d\n", cnt);
            cout << "输出: " << out << endl;
            cout << "答案: " << ans << endl;
            return;
        }
        out = ans = "";
    }
    while (in1 >> out)
        if (out != "") {
            printf("\033[1;31mWA\033[0m\ntoo much\n"), wa++;
            cout << out << endl;
            return;
        }
    printf("\033[1;32mAC\033[0m\n");
    ac++;
    return;
}
void total(int x) {
    printf("共计:\n");
    if (ac != 0) printf("\033[1;32mAC:\033[0m %d\n", ac);
    if (wa != 0) printf("\033[1;31mWA:\033[0m %d\n", wa);
    if (tle != 0) printf("\033[1;33mTLE:\033[0m %d\n", tle);
    if (mle != 0) printf("\033[1;34mMLE:\033[0m %d\n", mle);
    printf("平均时间: %lf\n", ti / x);
    printf("总时间: %lf\n", ti);
    printf("----------------------------\n");
}
double te() {
    return (db)(ed - start) / CLOCKS_PER_SEC;
}
void version() {
    printf("bch 1.4.0");
}
int main(int argc, char **argv) {
    prepare();
    ifstream in;
    ifstream out;
    ins = argv[1];
    if (ins == "help" || ins == "-h" || ins == "--help" || ins == "?")
        help();
    else if (ins == "-v" || ins == "--version")
        version();
    else if (ins == "-m") {
        char *file1 = argv[2], *file2 = argv[3], *file3 = NULL, *file4 = NULL;
        char *folder = NULL;
        int t = 4, mode = 0;

        while (argv[t] != NULL) {
            if (strcmp(argv[t], "-r") == 0)
                mode = 1;
            else if (strcmp(argv[t], "-rf") == 0)
                mode = 2;
            else if (strcmp(argv[t], "-fo") == 0)
                mode = 3;
            else if (strcmp(argv[t], "-fi") == 0)
                mode = 4;
            else if (strcmp(argv[t], "-o") == 0) {
                generate_location = 0;
                string folder = exe_path + "\\data";
                if (_access(folder.c_str(), 0) == -1) _mkdir(folder.c_str());
                out_path = in_path = folder;
            } else if (strcmp(argv[t], "-l") == 0) {
                generate_location = 1;
                string folder = work_path + "\\data";
                if (_access(folder.c_str(), 0) == -1) _mkdir(folder.c_str());
                out_path = in_path = folder;
            } else if (strcmp(argv[t], "-n") == 0)
                times = char_int(argv[++t]);
            else if (strcmp(argv[t], "-t") == 0)
                generate_mode = 0;
            else if (strcmp(argv[t], "-s") == 0)
                generate_mode = 1;
            else {
                switch (mode) {
                case 1:
                    file3 = argv[t];
                    break;
                case 2:
                    if (file3 == NULL)
                        file3 = argv[t];
                    else
                        file4 = argv[t];
                    break;
                case 3:
                    folder = argv[t];
                case 4:
                    file3 = argv[t];
                }
            }
            t++;
        }

        int cnt = 0;
        string inname;
        switch (mode) {
        case 1:
            printf("将要以%s生成的输入文件%s和%s进行对拍\n", file3, file1, file2);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("是否进行编号: %s\n", (generate_mode) ? "true" : "false");
            printf("对拍次数: %d\n", times);
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");
            printf("\n----对拍结果----\n");
            for (int i = 1; i <= times; i++) {
                char *t = new char[105], j[105];
                strcpy(j, generate_mode ? int_char(i).c_str() : "");
                sprintf(t, "%s.exe > %s\\%s%s.in", file3, in_path.c_str(), file3, j), system(t);
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
                start = clock();
                sprintf(t, "%s.exe < %s\\%s%s.in > %s\\%s%s.out", file1, in_path.c_str(), file3, j, out_path.c_str(), file1, j), system(t);
                ed = clock();
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file2), system(t);
                sprintf(t, "%s.exe < %s\\%s%s.in > %s\\%s%s.out", file2, in_path.c_str(), file3, j, out_path.c_str(), file2, j), system(t);
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                string out = in_path + "\\" + file1 + j + ".out", ans = out_path + "\\" + file2 + j + ".out";
                printf("测试点 %d:\n", i);
                if (te() > time_limit)
                    printf("\033[1;33mTLE\033[0m\n"), tle++;
                else
                    diff(out, ans);
                printf("时间: %lf\n", te()), ti += te();
                printf("---\n");
            }
            total(times);
            break;
        case 2:
            printf("将要以%s作为%s的输入生成输入文件对%s和%s进行对拍\n", file4, file3, file1, file2);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("是否进行编号: %s\n", (generate_mode) ? "true" : "false");
            printf("对拍次数: %d\n", times);
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");
            printf("\n----对拍结果----\n");
            for (int i = 1; i <= times; i++) {
                char *t = new char[105], j[105];
                strcpy(j, (generate_mode == 1) ? int_char(i).c_str() : "");
                sprintf(t, "%s.exe < %s.in > %s\\%s%s.in", file3, file4, in_path.c_str(), file3, j), system(t);
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
                sprintf(t, "%s.exe < %s\\%s%s.in > %s\\%s%s.out", file1, in_path.c_str(), file3, j, out_path.c_str(), file1, j);
                start = clock();
                system(t);
                ed = clock();
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file2), system(t);
                sprintf(t, "%s.exe < %s\\%s%s.in > %s\\%s%s.out", file2, in_path.c_str(), file3, j, out_path.c_str(), file2, j), system(t);
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                string out = in_path + "\\" + file1 + j + ".out", ans = out_path + "\\" + file2 + j + ".out";
                printf("测试点 %d:\n", i);
                if (te() > time_limit)
                    printf("\033[1;33mTLE\033[0m\n"), tle++;
                else
                    diff(out, ans);
                printf("时间: %lf\n", te()), ti += te();
                printf("---\n");
            }
            total(times);
            break;
        case 3:
            printf("将要以%s下的所有输入文件对%s和%s进行对拍\n", folder, file1, file2);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("是否进行编号: %s\n", (generate_mode) ? "true" : "false");
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");
            printf("\n----对拍结果----\n");
            char t[1005];
            sprintf(t, "dir /b %s > %s\\in_name.txt", folder, folder);
            system(t);
            sprintf(t, "%s\\%s\\in_name.txt", work_path.c_str(), folder);
            in.open(t);
            cnt = 0;
            while (in >> inname) {
                if (inname.find(".in") == -1) continue;
                cout << inname << endl;
                char j[105];
                strcpy(j, int_char(++cnt).c_str());
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
                sprintf(t, "%s.exe < %s\\%s > %s\\%s%s.out", file1, folder, inname.c_str(), out_path.c_str(), file1, j);
                start = clock();
                system(t);
                ed = clock();
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file2), system(t);
                sprintf(t, "%s.exe < %s\\%s > %s\\%s%s.out", file2, folder, inname.c_str(), out_path.c_str(), file2, j), system(t);
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                string out = in_path + "\\" + file1 + j + ".out", ans = out_path + "\\" + file2 + j + ".out";
                printf("测试点 %d:\n", cnt);
                if (te() > time_limit)
                    printf("\033[1;33mTLE\033[0m\n"), tle++;
                else
                    diff(out, ans);
                printf("时间: %lf\n", te()), ti += te();
                printf("---\n");
            }
            total(cnt);
            break;
        case 4:
            printf("将要以%s作为输入文件对%s和%s进行对拍\n", file3, file1, file2);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");
            sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
            sprintf(t, "%s.exe < %s.in > %s\\%s.out", file1, file3, out_path.c_str(), file1);
            start = clock();
            system(t);
            ed = clock();
            system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
            sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file2), system(t);
            sprintf(t, "%s.exe < %s.in > %s\\%s.out", file2, file3, out_path.c_str(), file2), system(t);
            system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
            printf("\n----对拍结果----\n");
            printf("测试点 1:\n");
            string out = in_path + "\\" + file1 + ".out", ans = out_path + "\\" + file2 + ".out";
            if (te() > time_limit)
                printf("\033[1;33mTLE\033[0m\n"), tle++;
            else
                diff(out, ans);
            printf("时间: %lf\n", te()), ti += te();
            printf("---\n");
            total(1);
            break;
        }
    } else if (ins == "-mf") {
        char *file1 = argv[2], *file_in = NULL, *file_out = NULL;
        char *folder = NULL;
        int t = 3;
        while (argv[t] != NULL) {
            if (strcmp(argv[t], "-o") == 0)
                generate_location = 0;
            else if (strcmp(argv[t], "-l") == 0)
                generate_location = 1;
            else if (strcmp(argv[t], "-t") == 0)
                generate_mode = 0;
            else if (strcmp(argv[t], "-s") == 0)
                generate_mode = 1;
            else {
                if (file_in == NULL)
                    file_in = argv[t];
                else
                    file_out = argv[t];
            }
            t++;
        }

        if (file_out == NULL) {
            string inname;
            folder = file_in;

            if (generate_location == 0) {
                string fol = exe_path + "\\data";
                if (_access(fol.c_str(), 0) == -1) _mkdir(fol.c_str());
                out_path = work_path + "\\" + folder;
                in_path = work_path + "\\" + folder;
            } else {
                string fol = work_path + "\\data";
                if (_access(fol.c_str(), 0) == -1) _mkdir(fol.c_str());
                out_path = fol;
                in_path = work_path + "\\" + folder;
            }

            printf("将要以%s下的所有输入和答案文件对%s进行对拍\n", folder, file1);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("是否进行编号: %s\n", (generate_mode) ? "true" : "false");
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");
            printf("\n----对拍结果----\n");
            char t[1005];
            sprintf(t, "dir /b %s > %s\\in_name.txt", folder, folder);
            system(t);
            sprintf(t, "%s\\%s\\in_name.txt", work_path.c_str(), folder);
            in.open(t);
            int cnt = 0;
            while (in >> inname) {
                if (inname.find(".in") == -1) continue;
                cnt++;
                char j[105];
                if (generate_mode == 1)
                    strcpy(j, inname.substr(inname.find("_"), inname.find(".in") - inname.find("_")).c_str());
                else
                    strcpy(j, "");
                sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
                sprintf(t, "%s.exe < %s\\%s > %s\\%s%s.out", file1, folder, inname.c_str(), out_path.c_str(), file1, j);
                start = clock();
                system(t);
                ed = clock();
                system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
                string out = out_path + "\\" + file1 + j + ".out", ans = in_path + "\\" + file1 + j + ".ans";
                printf("测试点 %d:\n", cnt);
                if (te() > time_limit)
                    printf("\033[1;33mTLE\033[0m\n"), tle++;
                else
                    diff(out, ans);
                printf("时间: %lf\n", te()), ti += te();
                printf("---\n");
            }
            total(cnt);
        } else {
            if (generate_location == 0) {
                string fol = exe_path + "\\data";
                if (_access(fol.c_str(), 0) == -1) _mkdir(fol.c_str());
                out_path = fol;
            } else {
                string fol = work_path + "\\data";
                if (_access(fol.c_str(), 0) == -1) _mkdir(fol.c_str());
                out_path = fol;
            }

            printf("将要以%s和%s分别作为输入和答案文件对%s进行对拍\n", file_in, file_out, file1);
            printf("\n----对拍信息----\n");
            printf("生成位置: %s\n", (generate_location) ? "local" : "online");
            printf("时间限制: %d\n", time_limit);
            printf("程序运行时间限制: %d\n", exe_time_limit);
            printf("----------------------------\n");

            char t[1005];
            sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
            sprintf(t, "%s.exe < %s > %s\\%s.out", file1, file_in, out_path.c_str(), file1);
            start = clock();
            system(t);
            ed = clock();
            system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
            string out = out_path + "\\" + file1 + ".out", ans = file_out;
            printf("\n----对拍结果----\n");
            printf("测试点 1:\n");
            if (te() > time_limit)
                printf("\033[1;33mTLE\033[0m\n"), tle++;
            else
                diff(out, ans);
            printf("时间: %lf\n", te()), ti += te();
            printf("---\n");
            total(1);
        }
    } else if (ins == "-f") {
        int t = 3;
        string out1 = work_path + "\\", out2 = work_path + "\\";

        char *file1 = argv[2];
        if (strcmp(argv[t], "-c") == 0)
            out1 = out1 + argv[++t], t++;
        else
            out1 = out1 + file1 + ".out";
        char *file2 = argv[t++], *file3 = NULL;
        if (strcmp(argv[t], "-c") == 0)
            out2 = out2 + argv[++t], t++;
        else
            out2 = out2 + file2 + ".out";
        cout << out1 << endl
             << out2 << endl;
        while (argv[t] != NULL) {
            if (strcmp(argv[t], "-n") == 0)
                times = char_int(argv[++t]);
            else
                file3 = argv[t];
            t++;
        }
        cout << out1 << endl
             << out2 << endl;
        if (file3 == NULL)
            printf("将要对%s和%s进行对拍\n", file1, file2);
        else
            printf("将要以%s生成的输入文件%s和%s进行对拍\n", file3, file1, file2);
        printf("\n----对拍信息----\n");
        printf("对拍次数: %d\n", times);
        printf("时间限制: %d\n", time_limit);
        printf("程序运行时间限制: %d\n", exe_time_limit);
        printf("----------------------------\n");
        printf("\n----对拍结果----\n");
        for (int i = 1; i <= times; i++) {
            char *t = new char[105];
            sprintf(t, "%s.exe", file3), system(t);
            sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file1), system(t);
            start = clock();
            sprintf(t, "%s.exe", file1), system(t);
            ed = clock();
            system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
            sprintf(t, "start \"timekill\" timekill.exe %d %s.exe", exe_time_limit, file2), system(t);
            sprintf(t, "%s.exe", file2), system(t);
            system("taskkill>nul 2>nul /f /t /fi \"WINDOWTITLE eq timekill\"");
            printf("测试点 %d:\n", i);
            if (te() > time_limit)
                printf("\033[1;33mTLE\033[0m\n"), tle++;
            else
                diff(out1, out2);
            printf("时间: %lf\n", te()), ti += te();
            printf("---\n");
        }
        total(times);
    } else if (ins == "-s") {
        char info[105], name[105], str[105];
        strcpy(info, "settings"); // settings

        char *t = argv[2];
        if (strcmp(t, "til") == 0) {
            int v = char_int(argv[3]);
            strcpy(name, "time_limit");
            ini.write_ini_val(ini_path, info, name, v);
        } else if (strcmp(t, "etil") == 0) {
            int v = char_int(argv[3]);
            strcpy(name, "exe_time_limit");
            ini.write_ini_val(ini_path, info, name, v);
        } else if (strcmp(t, "mt") == 0) {
            int v = char_int(argv[3]);
            strcpy(name, "match-times");
            ini.write_ini_val(ini_path, info, name, v);
        } else if (strcmp(t, "gl") == 0) {
            if (strcmp(argv[3], "-o") != 0 && strcmp(argv[3], "-l") != 0) {
                printf("值无效！");
                return 0;
            }
            strcpy(name, "generate_location"), strcpy(str, argv[3]);
            ini.write_ini_str(ini_path, info, name, str);
        } else if (strcmp(t, "gm") == 0) {
            if (strcmp(argv[3], "-s") != 0 && strcmp(argv[3], "-t") != 0) {
                printf("值无效！");
                return 0;
            }
            strcpy(name, "generate_mode"), strcpy(str, argv[3]);
            ini.write_ini_str(ini_path, info, name, str);
        } else
            printf("未发现该设置！");
    } else if (ins == "-p") {
        printf("生成位置: %s\n", (generate_location) ? "local" : "online");
        printf("是否进行编号: %s\n", (generate_mode) ? "true" : "false");
        printf("对拍次数: %d\n", times);
        printf("时间限制: %d\n", time_limit);
        printf("程序运行时间限制: %d\n", exe_time_limit);
    } else
        help();
}
