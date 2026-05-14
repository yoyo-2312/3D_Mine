# Mine3D

Mine3D 是一个使用 C++ 编写的 Windows 桌面 3D 扫雷游戏。  
项目使用 EasyX 负责图形绘制，使用 `winmm` 播放音效。

## 游戏截图

![Mine3D 游戏截图](c502def85f1417dca90d287a5bfc831c_720.jpg)

## 游戏简介

这是一个基于三维空间的扫雷项目，雷区规模为 `10 x 10 x 10`。  
玩家需要在不同层之间切换，结合数字提示判断雷的位置，并尽量在更短时间内完成排雷。

## 核心特点

- 三维雷区设计，不再局限于传统平面扫雷
- 四个难度等级：10、30、50、100 颗雷
- 支持通过底部层数栏切换当前深度
- 带有音效、暂停、帮助、重新开始和成绩界面
- `GameRecord.txt` 会在需要时自动创建

## 操作说明

- 左键：打开一个格子
- 双击已打开的数字格：当周围旗子数与数字相等时，自动打开周围格子
- 右键：在首步之后插旗或取消插旗
- 点击底部层数栏：切换当前显示层
- 点击右上角图标：帮助、重新开始、暂停、声音、返回主界面

## 项目结构

- `Main Func.cpp`：程序入口
- `Game_Mine.cpp` / `Game_Mine.h`：游戏状态初始化和主循环
- `algorithm functions.cpp` / `algorithm_function.h`：游戏逻辑与输入处理
- `draw functions.cpp` / `draw_function.h`：界面绘制与资源加载
- `Sweep_IMG/`：运行时需要的图片资源
- `Sweep_MP3/`：运行时需要的音频资源
- `build.bat`：Windows 本地构建脚本

## 环境要求

- Windows
- Visual Studio 2022，并安装 `Desktop development with C++`
- 在同一个 Visual Studio 实例中安装 EasyX

## 快速开始

先在仓库根目录执行编译：

```bat
build.bat
```

编译完成后运行：

```powershell
.\Mine3D.exe
```

## 编译

在仓库根目录运行：

```bat
build.bat
```

这个脚本会自动完成以下工作：

- 定位最新的 Visual Studio C++ 工具链
- 初始化 MSVC 编译环境
- 链接当前项目需要的 EasyX 库目录
- 在 `Mine3D.exe` 仍在运行时提前给出提示

如果系统里没有 EasyX，`build.bat` 会尝试下载官方安装程序并启动它。  
你仍然需要在安装界面中为 Visual Studio 2022 手动点击一次 `Install`，安装完成后再重新运行 `build.bat`。

## 运行

请在仓库根目录启动程序，这样相对路径资源才能正确加载：

```powershell
.\Mine3D.exe
```

## 仓库说明

- 不要提交生成出来的二进制文件和调试文件，`.gitignore` 已经排除了这些内容
- `GameRecord.txt` 是运行时生成的本地数据文件，已经被忽略
- 如果你想分享可执行程序，建议把 `Mine3D.exe` 作为 GitHub Release 资源上传，而不是直接放进仓库
