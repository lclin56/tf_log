# TF Log

TF Log 是一个简单的 C/C++ 日志库，可以为你的项目添加日志功能。

## 特点

- 多个日志级别：INFO、DEBUG、WARN、ERROR
- 不同的日志模式：输出到控制台、回调函数、写入文件
- 支持线程安全选项
- 简单地集成到 C 和 C++ 项目中，只需包含头文件
- 支持日志卷大小拆分

## 目录

- [安装](#安装)
- [使用方法](#使用方法)
- [配置](#配置)
- [贡献](#贡献)
- [许可证](#许可证)

## 安装

克隆仓库并在你的项目中包含 `tf_log.h` 头文件。

```c
#include "tf_log.h"
```

## 使用方法

1. 使用默认设置初始化日志记录器：

```c
LOG_INIT();
```

2. 记录日志消息：

```c
LOG_INFO("这是一条信息消息。");
LOG_DEBUG("调试信息：%d", 变量);
LOG_WARN("警告：可能有问题。");
LOG_ERROR("错误：出现了严重问题。");
```

3. 完成后取消初始化日志记录器：

```c
LOG_UNINIT();
```

## 配置

你可以使用以下宏来配置日志记录器的行为：

- `LOG_SET_LEVEL(level)`：设置日志级别。
- `LOG_SET_MODE(mode)`：设置日志模式（输出、回调、文件）。
- `LOG_SET_PATH(path)`：设置日志文件路径。
- `LOG_SET_FILENAME(filename)`：设置日志文件名。
- `LOG_SET_MAX_SIZE(max_size)`：设置最大日志文件大小。
- `LOG_SET_CALL_BACK(call_back)`：设置回调函数。

## 贡献

欢迎贡献！请按照以下步骤进行：

1. Fork 仓库。
2. 为你的特性/修复创建一个新分支。
3. 进行修改并进行充分测试。
4. 创建一个拉取请求。

## 许可证

本项目基于 MIT 许可证发布 - 详见 [LICENSE](LICENSE) 文件。