#ifndef TF_LOG_H
#define TF_LOG_H

// Include necessary headers based on the language
#ifndef __cplusplus
#include <time.h>
#include <string.h>
#include <stdio.h>
#else
#include <ctime>
#include <cstring>
#include <cstdio>
#endif

// Enumeration for log levels
typedef enum
{
    TF_LOG_LEVEL_CLOSE = 0,
    TF_LOG_LEVEL_INFO,
    TF_LOG_LEVEL_DEBUG,
    TF_LOG_LEVEL_WARN,
    TF_LOG_LEVEL_ERROR,
} TF_LOG_LEVEL;

#define TF_LOG_MAX_FILE_SIZE (5 * 1024 * 1024)   // Maximum log file size: 5 MB
#define TF_LOG_DEFAULT_PATH "./"                 // Default log path
#define TF_LOG_DEFAULT_FILENAME "tf_message.log" // Default log filename

// Enumeration for log modes
typedef enum
{
    TF_LOG_MODE_PRINT,    // Print logs to console
    TF_LOG_MODE_CALLBACK, // Use a callback function for logging
    TF_LOG_MODE_FILE      // Write logs to a file
} TF_LOG_MODE;

typedef int (*LOG_CALL_BACK)(int level, const char *log_str);

// External variables
extern LOG_CALL_BACK tf_log_callback;
extern TF_LOG_MODE tf_log_mode;
extern FILE *tf_log_file;
extern int tf_log_level;
extern char tf_log_path[1024];
extern char tf_log_filename[256];
extern size_t tf_log_max_file_size;

#ifdef _REENTRANT
#include <pthread.h>
extern pthread_mutex_t log_file_mutex; // Mutex for thread safety

// Logging macro for thread-safe environments
#define LOG_MESSAGE(level, level_str, format, ...)                                          \
    do                                                                                      \
    {                                                                                       \
        if (tf_log_level <= level)                                                          \
        {                                                                                   \
            char time_buf[26];                                                              \
            time_t t = time(NULL);                                                          \
            struct tm result;                                                               \
            strftime(time_buf, sizeof(time_buf), "%F %T", localtime_r(&t, &result));        \
            char full_log_str[1024];                                                        \
            snprintf(full_log_str, sizeof(full_log_str), "[%s %s %s:%d %s] " format "\n",   \
                     level_str, time_buf, __FILE__, __LINE__, __func__, ##__VA_ARGS__);     \
            switch (tf_log_mode)                                                            \
            {                                                                               \
            case TF_LOG_MODE_PRINT:                                                         \
                fprintf(level == TF_LOG_LEVEL_ERROR ? stderr : stdout, "%s", full_log_str); \
                break;                                                                      \
            case TF_LOG_MODE_CALLBACK:                                                      \
                if (tf_log_callback != NULL)                                                \
                {                                                                           \
                    tf_log_callback(level, full_log_str);                                   \
                }                                                                           \
                break;                                                                      \
            case TF_LOG_MODE_FILE:                                                          \
                if (tf_log_file != NULL)                                                    \
                {                                                                           \
                    if (ftell(tf_log_file) > tf_log_max_file_size)                          \
                    {                                                                       \
                        pthread_mutex_lock(&log_file_mutex);                                \
                        if (ftell(tf_log_file) > tf_log_max_file_size)                      \
                        {                                                                   \
                            fclose(tf_log_file);                                            \
                            char file_path[1024];                                           \
                            char new_filename[1024];                                        \
                            snprintf(file_path, sizeof(file_path), "%s/%s",                 \
                                     tf_log_path, tf_log_filename);                         \
                            strftime(time_buf, sizeof(time_buf), "%Y%m%d%H%M%S",            \
                                     localtime_r(&t, &result));                             \
                            snprintf(new_filename, sizeof(new_filename),                    \
                                     "%s/%s_%s", tf_log_path, tf_log_filename, time_buf);   \
                            rename(file_path, new_filename);                                \
                            tf_log_file = fopen(file_path, "a");                            \
                        }                                                                   \
                        pthread_mutex_unlock(&log_file_mutex);                              \
                    }                                                                       \
                    fprintf(tf_log_file, "%s", full_log_str);                               \
                }                                                                           \
                break;                                                                      \
            }                                                                               \
        }                                                                                   \
    } while (0)

// Initialization macro for thread-safe environments
#define LOG_INIT()                                       \
    LOG_CALL_BACK tf_log_callback = NULL;                \
    TF_LOG_MODE tf_log_mode = TF_LOG_MODE_PRINT;         \
    FILE *tf_log_file = NULL;                            \
    int tf_log_level = TF_LOG_LEVEL_INFO;                \
    char tf_log_path[1024] = TF_LOG_DEFAULT_PATH;        \
    char tf_log_filename[256] = TF_LOG_DEFAULT_FILENAME; \
    size_t tf_log_max_file_size = TF_LOG_MAX_FILE_SIZE;  \
    pthread_mutex_t log_file_mutex = PTHREAD_MUTEX_INITIALIZER;

#else
// Logging macro for non-thread-safe environments
#define LOG_MESSAGE(level, level_str, format, ...)                                          \
    do                                                                                      \
    {                                                                                       \
        if (tf_log_level <= level)                                                          \
        {                                                                                   \
            char time_buf[26];                                                              \
            time_t t = time(NULL);                                                          \
            struct tm result;                                                               \
            strftime(time_buf, sizeof(time_buf), "%F %T", localtime_r(&t, &result));        \
            char full_log_str[1024];                                                        \
            snprintf(full_log_str, sizeof(full_log_str), "[%s %s %s:%d %s] " format "\n",   \
                     level_str, time_buf, __FILE__, __LINE__, __func__, ##__VA_ARGS__);     \
            switch (tf_log_mode)                                                            \
            {                                                                               \
            case TF_LOG_MODE_PRINT:                                                         \
                fprintf(level == TF_LOG_LEVEL_ERROR ? stderr : stdout, "%s", full_log_str); \
                break;                                                                      \
            case TF_LOG_MODE_CALLBACK:                                                      \
                if (tf_log_callback != NULL)                                                \
                {                                                                           \
                    tf_log_callback(level, full_log_str);                                   \
                }                                                                           \
                break;                                                                      \
            case TF_LOG_MODE_FILE:                                                          \
                if (tf_log_file != NULL)                                                    \
                {                                                                           \
                    if (ftell(tf_log_file) > tf_log_max_file_size)                          \
                    {                                                                       \
                        fclose(tf_log_file);                                                \
                        char file_path[1024];                                               \
                        char new_filename[1024];                                            \
                        snprintf(file_path, sizeof(file_path), "%s/%s",                     \
                                 tf_log_path, tf_log_filename);                             \
                        strftime(time_buf, sizeof(time_buf), "%Y%m%d%H%M%S",                \
                                 localtime_r(&t, &result));                                 \
                        snprintf(new_filename, sizeof(new_filename),                        \
                                 "%s/%s_%s", tf_log_path, tf_log_filename, time_buf);       \
                        rename(file_path, new_filename);                                    \
                        tf_log_file = fopen(file_path, "a");                                \
                    }                                                                       \
                    if (tf_log_file)                                                        \
                    {                                                                       \
                        fprintf(tf_log_file, "%s", full_log_str);                           \
                        fflush(tf_log_file);                                                \
                    }                                                                       \
                }                                                                           \
                break;                                                                      \
            }                                                                               \
        }                                                                                   \
    } while (0)

// Initialization macro for non-thread-safe environments
#define LOG_INIT()                                       \
    LOG_CALL_BACK tf_log_callback = NULL;                \
    TF_LOG_MODE tf_log_mode = TF_LOG_MODE_PRINT;         \
    FILE *tf_log_file = NULL;                            \
    int tf_log_level = TF_LOG_LEVEL_INFO;                \
    char tf_log_path[1024] = TF_LOG_DEFAULT_PATH;        \
    char tf_log_filename[256] = TF_LOG_DEFAULT_FILENAME; \
    size_t tf_log_max_file_size = TF_LOG_MAX_FILE_SIZE;
#endif

// Macro to uninitialize the logging settings
#define LOG_UNINIT()                                                                    \
    do                                                                                  \
    {                                                                                   \
        if (tf_log_file != NULL)                                                        \
        {                                                                               \
            fclose(tf_log_file);                                                        \
            tf_log_file = NULL;                                                         \
        }                                                                               \
        tf_log_callback = NULL;                                                         \
        tf_log_mode = TF_LOG_MODE_PRINT;                                                \
        tf_log_level = TF_LOG_LEVEL_INFO;                                               \
        strncpy(tf_log_path, TF_LOG_DEFAULT_PATH, sizeof(tf_log_path) - 1);             \
        strncpy(tf_log_filename, TF_LOG_DEFAULT_FILENAME, sizeof(tf_log_filename) - 1); \
        tf_log_max_file_size = TF_LOG_MAX_FILE_SIZE;                                    \
    } while (0)

// Macro to set logging parameters
#define LOG_SET(level, mode, path, filename, max_size)                                   \
    do                                                                                   \
    {                                                                                    \
        tf_log_level = level;                                                            \
        tf_log_mode = mode;                                                              \
        strncpy(tf_log_path, path, sizeof(tf_log_path) - 1);                             \
        strncpy(tf_log_filename, filename, sizeof(tf_log_filename) - 1);                 \
        tf_log_max_file_size = max_size;                                                 \
        if (tf_log_mode == TF_LOG_MODE_FILE)                                             \
        {                                                                                \
            char filepath[1280];                                                         \
            snprintf(filepath, sizeof(filepath), "%s/%s", tf_log_path, tf_log_filename); \
            if (tf_log_file)                                                             \
                fclose(tf_log_file);                                                     \
            tf_log_file = fopen(filepath, "a");                                          \
        }                                                                                \
    } while (0)

// Macros to set specific logging parameters
#define LOG_SET_LEVEL(level) (tf_log_level = (level))
#define LOG_SET_MODE(mode) LOG_SET(tf_log_level, mode, tf_log_path, tf_log_filename, tf_log_max_file_size)
#define LOG_SET_PATH(path) LOG_SET(tf_log_level, tf_log_mode, path, tf_log_filename, tf_log_max_file_size)
#define LOG_SET_FILENAME(filename) LOG_SET(tf_log_level, tf_log_mode, tf_log_path, filename, tf_log_max_file_size)
#define LOG_SET_FILE(path, filename, max_size) LOG_SET(tf_log_level, LOG_MODE_FILE, path, filename, max_size)
#define LOG_SET_MAX_SIZE(max_size) (tf_log_max_file_size = (max_size))
#define LOG_SET_CALL_BACK(call_back) (tf_log_callback = (call_back))

// Logging macros for different log levels
#define LOG_INFO(format, ...) LOG_MESSAGE(TF_LOG_LEVEL_INFO, "INFO", format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_MESSAGE(TF_LOG_LEVEL_DEBUG, "DEBUG", format, ##__VA_ARGS__)
#define LOG_WARN(format, ...) LOG_MESSAGE(TF_LOG_LEVEL_WARN, "WARN", format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_MESSAGE(TF_LOG_LEVEL_ERROR, "ERROR", format, ##__VA_ARGS__)

#endif // TF_LOG_H
