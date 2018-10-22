

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