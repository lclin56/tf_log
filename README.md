# TF Log

TF Log is a simple logging library in C/C++ for adding log capabilities to your projects.
[简体中文](doc/README_CN.md)

## Features

- Multiple log levels: INFO, DEBUG, WARN, ERROR
- Different logging modes: print, callback, file
- Thread-safe options available
- Easy to integrate into C and C++ projects, just include the header file
- Support for log volume splitting

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

## Installation

Clone the repository and include the `tf_log.h` header file in your project.

```c
#include "tf_log.h"
```

## Usage

1. Initialize the logger with default settings:

```c
LOG_INIT();
```

2. Start logging messages:

```c
LOG_INFO("This is an info message.");
LOG_DEBUG("Debugging information: %d", variable);
LOG_WARN("Warning: Something might be wrong.");
LOG_ERROR("Error: Something went terribly wrong.");
```

3. Uninitialize the logger when done:

```c
LOG_UNINIT();
```

## Configuration

You can configure the logger's behavior using the following macros:

- `LOG_SET_LEVEL(level)`: Set the logging level.
- `LOG_SET_MODE(mode)`: Set the logging mode (print, callback, file).
- `LOG_SET_PATH(path)`: Set the path for log files.
- `LOG_SET_FILENAME(filename)`: Set the log filename.
- `LOG_SET_MAX_SIZE(max_size)`: Set the maximum log file size.
- `LOG_SET_CALL_BACK(call_back)`: Set the callback function.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature/fix.
3. Make your changes and test thoroughly.
4. Create a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
```