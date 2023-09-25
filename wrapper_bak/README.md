# NetEase IM CrossPlatform(PC) C++ SDK  封装层接入指引

## 运行环境

在 Windows 下 SDK 使用 VS2017 及其工具链（v141_xp、/MD）进行编译，在您的应用部署前需要将 VS2017 运行时库安装到目标系统中或将依赖文件存放到执行程序可以搜索到的路径下。最新的 Visual Studio 2017 运行时库可以从微软官方网站进行下载：[https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0](https://support.microsoft.com/en-us/topic/the-latest-supported-visual-c-downloads-2647da03-1eea-4433-9aff-95f26a218cc0)

在 macOS 下 SDK 使用 Xcode 及其工具链进行编译，无特殊支持要求，最低支持系统到 macOS 10.13。

## SDK 目录结构及主要文件介绍

为降低接入成本, NetEase IM SDK （以下简称 NIM SDK）从 8.4.0 版本起对发布目录进行了调整, C++ 封装层使用 CMake 进行管理, 根据不同的情况您在接入 C++ 封装层时可能需要适当的修改, 新的发布目录拆分为如下结构：

```
├─bin
├─include
├─lib
└─wrapper
```

- `bin`/`lib` 目录存放 NIM C SDK 的动态库文件, 在您发布时请打包使用到的模块, `.lib`后缀为 Windows 下的动态库符号链接文件
  - (lib)nim(.dll|.so|.dylib): 即时通讯
  - (lib)nim_chatroom(.dll|.so|.dylib): 聊天室
  - (lib)nim_qchat(.dll|.so|.dylib): 圈组
  - (lib)h_available(.dll|.so|.dylib): 高可用模块, 被以上三个模块所依赖
  - (lib)nim_tools_http(.dll|.so|.dylib): http 工具库, 若不使用`wrapper/nim_tools_cpp_wrapper/nim_tools_http_cpp.h`中的接口可去除.
  - nim_audio.dll: 音频解码库, 仅支持 Windows 环境, 若不使用`wrapper/nim_tools_cpp_wrapper/nim_audio_cpp.h`中的接口可去除.
  - nrtc.dll: 音视频通话库, 仅支持 Windows 环境, 若不使用`wrapper/nim_cpp_wrapper/api/nim_cpp_vchat.h`中的接口可去除.
  - node-nim.node: NIM SDK Node 封装层, 用于 Electron 以及 Node.js 环境下的接入, 其他情况可去除.
  - 其他如 msvcp120.dll、msvcr120.dll 等(系统)依赖
- `include` 目录存放了 NIM C SDK 的导出头文件
- `wrapper`目录存放了 C++ 封装层的源代码（含头文件）

## 以 C 接口接入

如果您希望以纯 C 接口接入 NIM SDK, 可以直接添加 include 为头文件搜索路径, ,动态加载或静态链接所需的动态库即可

## 以 C++ 接口接入

### CMake 工程接入

如果您原工程是基于 CMake 进行管理, 可以使用 `ADD_SUBDIRECTORY` 命令将 wrapper 目录添加为子目录, 使用 `INCLUDE_DIRECTORIES` 方法将 include 和 wrapper 文件夹添加为头文件搜索路径, 最后根据您的执行目标使用 `TARGET_LINK_LIBRARIES` 按需添加链接库即可, 库名称分别为：

- `nim_cpp_wrapper` NIM C++ 封装层库
- `nim_chatroom_cpp_wrapper` Chatroom C++ 封装层库
- `nim_wrapper_util` NIM 及 Chatroom C++ 封装层库通用的工具库
- `nim_tools_cpp_wrapper` NIM HTTP 组件 C++ 封装层库
- `nim_qchat_cpp_wrapper` NIM QChat 圈组 C++ 封装层库

### 传统方式接入（VS、Qt、Xcode）

使用传统方式接入 C++ 封装层, 您需要先生成 C++ 封装层的二进制文件.根据您工程的运行时库和平台工具集来生成您需要的封装层二进制文件, 以下列举了不同情况下生成 C++ 封装层所需的配置, 您只需要选择其中一种符合您工程配置的选项来生成工程文件即可.

**使用 CMake GUI 工具生成**

首先使用 CMake GUI 工具打开打开 C++ 封装层文件夹并配置生成缓存目录

<img src="https://nim-nosdn.netease.im/MTAxMTAwMg==/bmltYV81MjE0MzI3Mjc1XzE2MTg2MzQ5OTQwOTFfZTUxOTFhMzctM2YzNy00MWU3LWJjZWMtYmFjNDg0ZGY2MjQy?createTime=1618635005">

在点击 Configure 按钮后会提示您来配置工程选项, 参考下图配置您的工程：

<img src="https://nim-nosdn.netease.im/MTAxMTAwMg==/bmltYV81MjE0MzI3Mjc1XzE2MTg2MzQ5OTQwOTFfNGNmMTQwNTAtZDg2Ny00MzY2LTllNTgtMDgxZmM2MDFhMGIz?createTime=1618635005">

点击 Finish 按钮后工程初始化完成, 您可以在界面配置生成工程文件的选项, 参考下图配置：

<img src="https://nim-nosdn.netease.im/MTAxMTAwMg==/bmltYV81MjE0MzI3Mjc1XzE2MTg2MzQ5OTQwOTFfOTM3NzljM2MtNjhkZC00MTBkLWE3ODYtOWY4ZmYzODk0YTgw?createTime=1618635005">

您可以通过配置 `BUILD_SHARED_LIBS` 选择生成动态库还是静态库, 默认情况下将生成静态库, `CMAKE_INSTALL_PREFIX` 选项决定了 C++ 头文件和库文件的导出目录, 指定为 SDK 的根目录即可.

点击 Generate 按钮来生成工程文件, 生成后点击 Open Project 按钮即可打开工程, 在打开的工程中右键点击 INSTALL 生成项目.

<img src="https://nim-nosdn.netease.im/MTAxMTAwMg==/bmltYV81MjE0MzI3Mjc1XzE2MTg2MzQ5OTQwOTFfNGI5NzJhODktNGI2Ny00YjgwLTk1ZTgtNWMxNTYwYzc1NTc2?createTime=1618635005">

生成完成后, C++ 封装层的头文件及库文件会自动拷贝到 SDK 根目录下的 include 及 lib 目录下.

```
Install configuration: "Debug"
Installing: E:/nim-sdk/lib/nim_wrapper_util_d.lib
Installing: E:/nim-sdk/lib/nim_cpp_wrapper_d.lib
Installing: E:/nim-sdk/lib/nim_chatroom_cpp_wrapper_d.lib
Installing: E:/nim-sdk/lib/nim_tools_cpp_wrapper_d.lib
Installing: E:/nim-sdk/include/nim_chatroom_cpp_wrapper
Installing: E:/nim-sdk/include/nim_chatroom_cpp_wrapper/api
Installing: E:/nim-sdk/include/nim_chatroom_cpp_wrapper/api/nim_chatroom_cpp.h
.......
Installing: E:/nim-sdk/include/nim_cpp_wrapper
Installing: E:/nim-sdk/include/nim_cpp_wrapper/api
.......
```

当工程编译完成后, 只需要在您自己的工程中, 将 `include` 目录添加为头文件搜索路径, 将 `lib` 目录添加到您工程的库文件搜索路径, 并链接指定库文件即可.以 Visual Studio 举例, 分别对应：

- 头文件搜索路径, 工程右键属性->C/C++->常规, 右侧的“附加包含目录”
- 库文件搜索路径, 工程右键属性->链接器->常规, 右侧的“附加库目录”
- 链接库文件, 工程右键属性->链接器->输入, 右侧“附加依赖项”

> 注意：对于 8.4.0 以前的版本升级到 8.4.0 的情况, 您需要将原来头文件包含路径从原来的 `nim_sdk` 修改为 `include` 目录.

**使用命令行工具生成**

通常情况下我们需要在 CI 集成阶段自动化编译步骤, 您可以通过命令行来生成 C++ 封装层代码：

```bash
# 在 SDK 目录下新建一个 build 文件夹缓存编译 C++ 封装层时产生的临时文件
md build && cd build
# 生成工程文件, 您可以添加不同参数来决定生成的工程文件属性
# 添加 -DCMAKE_CXX_FLAGS_DEBUG=/MTd -DCMAKE_CXX_FLAGS_RELEASE=/MT 生成 MTd/MT 工程
# 添加 -DBUILD_SHARED_LIBS=ON 来生成动态库工程
# 添加 -G"Visual Studio 15 2017 Win64" 来生成 VS2017 64 位工程
# 添加 -G"Visual Studio 14 2015" 来生成 VS2015 的工程
# 添加 -T"v141_xp" 来生成支持 Windows XP 的工程
cmake ../wrapper
# 编译工程文件并导出头文件及库文件
# --build 参数描述了要编译当前目录下的工程
# --config 参数指定了你要编译 Release 还是 Debug 版本的库文件
# --target 表示您要编译的指定目标, 这里使用 install, 在 macOS 下请注意 install 单词大小写
cmake --build . --config Debug --target install
```

### 团队协作与 CI 集成

我们不建议将 CMake 产生的工程文件直接引用到项目中使用, 因为 CMake 使用将绝对路径写入到工程的配置文件（`.vcxproj`）中, 您在自己开发机上生成的工程文件无法在其他电脑顺利编译.

所以建议您在 CI 集成或者与其他同事协作时, 通过自定义脚本在项目编译前始终执行 CMake 来生成并编译工程得到需要的二进制文件产物.

当然您同样可以将生成好的 `.dll` 和 `.lib` 上传到您的代码版本控制系统, 这样虽然方便但在每次更新 SDK 版本时都上传二进制文件到代码版本控制系统中会导致代码仓库体积不断增大.

如果您的工程项目已经是基于 CMake 管理, 则不存在这样的问题, 只需将 wrapper 工程作为您主项目的依赖即可, 其他无需任何多余配置.
