/**
 * @file   	exutils.h
 * @brief  	Utility functions for Shunya AI examples
 */

/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#define LOG_VERSION "0.1.0"

/**
 * @brief Structer for log events
 */
typedef struct {
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    void *udata;
    int line;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

/**
 * @brief Log levels
 *
 */
enum {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

/* Define functions for various log levels */
#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief Returns the name of the given log level as a string.
 *
 * @param level
 * @return const char*
 */
const char *log_level_string(int level);
/**
 * @brief Set the lock to the log file.
 *
 * If the log will be written to from multiple threads a lock function can be set.
 * The function is passed the boolean true if the lock should be acquired or false if the
 * lock should be released and the given udata value.
 *
 * @param fn
 * @param udata
 */
void log_set_lock(log_LockFn fn, void *udata);
/**
 * @brief Set log level.
 *
 * The current logging level can be set by using the log_set_level() function.
 * All logs below the given level will not be written to stderr.
 * By default the level is LOG_TRACE, such that nothing is ignored.
 *
 * @param level log level
 */
void log_set_level(int level);
/**
 * @brief Enable Quite mode
 *
 * Quiet-mode can be enabled by passing true to the log_set_quiet() function.
 * While this mode is enabled the library will not output anything to stderr,
 * but will continue to write to files and callbacks if any are set.
 *
 * @param enable enable quite mode
 */
void log_set_quiet(bool enable);

/**
 * @brief
 *
 * One or more callback functions which are called with the log data can be provided
 * to the library by using the log_add_callback() function.
 * A callback function is passed a log_Event structure containing the
 * line number, filename, fmt string, va printf va_list, level and the given udata.
 *
 * @param fn function name
 * @param udata data to be written
 * @param level log level
 * @return int
 */
int log_add_callback(log_LogFn fn, void *udata, int level);

/**
 * @brief Adds a file pointer to the logging function
 *
 * One or more file pointers where the log will be written can be provided
 * to the library by using the log_add_fp() function.
 * Any messages below the given level are ignored.
 * If the library failed to add a file pointer a value less-than-zero is returned.
 *
 * @param fp
 * @param level
 * @return int
 */
int log_add_fp(FILE *fp, int level);

/**
 * @brief Write to the log.
 *
 * @param level Log level
 * @param file filename
 * @param line linenumber
 * @param fmt message
 * @param ... variable argument
 */
void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif  /* __LOG_H__ */ /* End of Include Guard */
