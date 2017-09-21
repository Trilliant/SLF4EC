/**
 * @file
 *
 * Defines types used in SLF4EC
 *
 * @author Jérémie Faucher-Goulet
 * 
 * @copyright Trilliant Inc. © 2015 - http://www.trilliantinc.com
 *
 * @License
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Trilliant
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef LOG_TYPES_H_
#define LOG_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UNIT_TESTING
#include <stdlib.h>
#endif
#include <stddef.h>
#include <stdarg.h>
#include <inttypes.h>

#if defined(UNIT_TESTING) && !defined(COMPILED_LOG_LEVEL)
#define COMPILED_LOG_LEVEL LEVEL_MAX
#endif

#define MAX_FILE_LENGTH 15 /**< Maximum length for displaying a path in a logger */
#define MAX_FCT_LENGHT 10  /**< Maximum length for displaying a function name in a logger */

/**
 * Disables any logging.
 */
#define LEVEL_OFF (0)

/**
 * Fatal log level. Used in case of system crash where the system cannot recover or is left in an unstable state.
 * A reboot is usually expected.
 */
#define LEVEL_FATAL (1)

/**
 * Error log level. Used in cases where an operation could not be completed successfully.
 * Data might have been lost but the system can recover and continue working.
 */
#define LEVEL_ERROR (2)

/**
 * Warning log level. Used when something unexpected happened but didn't have any adverse effect on the operation in progress.
 * No data or functionality loss.
 */
#define LEVEL_WARN (3)

/**
 * Info log level. Used to provide some high level indication of the application's ongoing processes.
 * Usually used as default level.
 */
#define LEVEL_INFO (4)

/**
 * Debug log level. Used when trying to isolate an issue or something very specific.
 */
#define LEVEL_DEBUG (5)

/**
 * Trace log level. Enables every log entries to get as much verbose as possible.
 */
#define LEVEL_TRACE (6)

/**
 * Test log level. Used only during personal local development.
 * Should never be used in released software.
 */
#define LEVEL_TEST (7)

#define LEVEL_MIN LEVEL_OFF  /**< Minimum Log Level (::LEVEL_OFF) */
#define LEVEL_MAX LEVEL_TEST /**< Maximum Log Level (::LEVEL_TEST) */

/**
 * Message format
 */
typedef enum
{
    FORMAT_FULL = 0,    /**< Outputs all details */
    FORMAT_MSG_ONLY = 1 /**< Outputs only the message */
} LogFormat;

/**
 * Return values from the logging API
 */
typedef enum
{
    LOG_OK = 0,              /**< No error */
    LOG_NOT_INITIALIZED,     /**< Not initialized */
    LOG_ALREADY_INITIALIZED, /**< Already initialized */
    LOG_INVALID_PARAMETER    /**< Invalid parameter */
} LogResult;

/**
 * Logging Category
 */
typedef struct
{
    const char* const name;  /**< Name for this category. */
    uint8_t currentLogLevel; /**< Current logging level for this category. */
} LogCategory;

/**
 * Packages data for the loggers
 */
typedef struct
{
    /**
     * Path to the source code file where this event occurred.
     * NULL if @p USE_LOCATION_INFO is not defined.
     */
    const char* const file;

    /**
     * Line number within the source code file where this event occurred.
     * NULL if @p USE_LOCATION_INFO is not defined.
     */
    const uint32_t* const line;

    /**
     * Name of the function within which the even occurred.
     * NULL if @p USE_LOCATION_INFO is not defined.
     */
    const char* const function;

    const uint64_t* const timestamp;   /**< timestamp of this event. */
    const LogCategory* const category; /**< LogCategory of this event. */
    const uint8_t* const level;        /**< LogLevel for this event. */
    const char* const formatStr;       /**< Format String for this event. */
    va_list* const vaList;             /**< Argument list for this event. */
} LogRecord;

/**
 * Called method inside a logger to output the information.
 * A logger must implement this.
 *
 * @param [in] logRecord Information on the event. This parameter is guaranteed to always hold a valid value so checks for NULL are unnecessary.
 * @param [in] logFormat Format to be used with this event
 */
typedef void (*const PublishLog)(const LogRecord* const logRecord, const LogFormat logFormat);

/**
 * Method called to initialize a logger.
 *
 * @param [in] config Configuration needed by the logger to initialize.
 */
typedef void (*const InitLog)(const void* const config);

/**
 * Method called by SLF4EC to obtain a timestamp of the event before publishing it to loggers.
 *
 * @return EPOCH timestamp
 */
typedef uint64_t (*const GetLogTimestamp)(void);

/**
 * Defines a logger.
 */
typedef struct
{
    const char* const loggerName; /**< Name of this logger. */
    const InitLog initFct;        /**< Function to initialize the logger. */
    const void* const initArgs;   /**< Arguments needed to initialize this logger. */
    const LogFormat format;       /**< Format to be used with this logger. */
    uint8_t currentLogLevel;      /**< Current LogLevel for this logger. Anything below will not be logged. */
    const PublishLog publishFct;  /**< Function to be called to output the event. */
} Logger;

#ifdef __cplusplus
}
#endif

#endif /* LOG_TYPES_H_ */
