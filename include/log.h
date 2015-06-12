/*
 * @file
 *
 * Simplified logging API
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

#ifndef SIMPLE_LOG_H_
#define SIMPLE_LOG_H_

#include "slf4ec.h"

/**
 * @file
 *
 * @code
 * #define USE_LOCATION_INFO
 * @endcode
 * Determines if log location is included (filename, line number and function name).
 * @remark It increases binary size significantly.
 *
 * @code
 * #define COMPILED_LOG_LEVEL (LEVEL_MAX)
 * @endcode
 * Logging calls below defined level are not compiled in the final binary.
 * @remark Helps to reduce binary size significantly.
 */

/**
 * Will not log anything, essentially a NOP. To be used when documenting a previously existing log. See ::LEVEL_OFF
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logOff(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logOff, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs fatal information. See ::LEVEL_FATAL
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logFatal(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logFatal, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs error information. See ::LEVEL_ERROR
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logError(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logError, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs warning information. See ::LEVEL_WARN
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logWarn(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logWarn, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs information. See ::LEVEL_INFO
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logInfo(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logInfo, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs debug information. See ::LEVEL_DEBUG
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logDebug(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logDebug, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logs trace information. See ::LEVEL_TRACE
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logTrace(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logTrace, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

/**
 * Logging to be used temporarily during development. See ::LEVEL_TEST
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] formatStr C string that contains the text to be logged.
 * It can optionally contain embedded format specifiers that are replaced by the values specified in subsequent additional arguments and formatted as requested.
 * @param [in] ... Depending on the @p formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * Additional arguments are ignored by the function.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logTest(logCategory, formatStr, ...) \
    TOKEN_PASTE(_logTest, IS_EMPTY(__VA_ARGS__)(logCategory, formatStr, __VA_ARGS__))

#endif /* SIMPLELOG_H_ */
