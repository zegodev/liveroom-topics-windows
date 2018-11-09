

## 源码编译
```
git clone https://github.com/zegodev/liveroom-topics-windows.git
cd src
```
使用vs2013或者以上版本打开工程文件`ZegoLiveRoomTopics.sln`。


右键项目，`生成事件`-->`生预成事件`-->`命令行`，删除掉生成appid的头文件。
`$(SolutionDir)AppSupport/gen_appconfig_header.bat`

修改`AppSupport`目录下`AppIDHelper.h.in`文件为`AppIDHelper.h`。
并更新使用自己的appid和app sign，建议把appid和sign存放于自己的服务器上，运行时通过网络请求动态获取，防止泄露。

```

工程目录结构

├── AppSupport // 通用支持工具目录
│   ├── ZGHelper.h/cpp
│   └── ZGManager.h/cpp
│
├── bin // 输出二进制目录
│   ├── Win32
│   │     ├── Debug
│   │     └── Release
│   └── x64
│         ├── Debug
│         └── Release
│
├── build_dir // 编译中间文件目录
│
├── libs // 库目录
│   └── zegosdk
│         ├── include
│         │      └── ZegoLiveRoom
│         │           └── LiveRoom.h ...
│         └── libs
│                ├── Win32
│                │    └── ZegoLiveRoom.lib/dll
│                │
│                └── x64
│                     └── ZegoLiveRoom.lib/dll
│
├── Res // 资源目录，比如媒体播放器需要的本地 mp3/mp4 文件。
│   └── *.mp3/*.mp4 ...
│ 
├── Topics // 各个专题的示例代码
│   │── MediaPlayer 媒体播放器,播放媒体文件并推流示例
│   │             ├── ZGMediaPlayerDemo.h/cpp // 与UI无关的逻辑代码
│   │             └── MediaPlayerUI           // UI相关代码目录
│   │                  ├── MediaPlayerDialog.h/cpp
│   │                  ├── resource.h         // UI 控件等宏定义文件
│   │                  └── MediaPlayerRc.rc   // UI 资源文件
|   |
│   └── MediaSideInfo 媒体次要信息示例
│
├── ZegoLiveRoomTopics // 项目工程文件目录
│   ├── ZegoLiveRoomTopics.vcxproj
│   └── ZegoLiveRoomTopicsDlg.h/cpp
│
└── ZegoLiveRoomTopics.sln // 项目解决方案文件

