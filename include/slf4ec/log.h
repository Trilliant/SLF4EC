/**
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

#ifdef __cplusplus
extern "C" {
#endif

#include "slf4ec/slf4ec.h"

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
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logOff(logCategory, ...) \
    TOKEN_PASTE(_logOff, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logOff() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogOff(logCategory, fmt, valist) \
    _vlogOff(logCategory, fmt, valist)

/**
 * Logs fatal information. See ::LEVEL_FATAL
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logFatal(logCategory, ...) \
    TOKEN_PASTE(_logFatal, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logFatal() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogFatal(logCategory, fmt, valist) \
    _vlogFatal(logCategory, fmt, valist)

/**
 * Logs error information. See ::LEVEL_ERROR
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logError(logCategory, ...) \
    TOKEN_PASTE(_logError, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logError() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogError(logCategory, fmt, valist) \
    _vlogError(logCategory, fmt, valist)

/**
 * Logs warning information. See ::LEVEL_WARN
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logWarn(logCategory, ...) \
    TOKEN_PASTE(_logWarn, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logWarn() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogWarn(logCategory, fmt, valist) \
    _vlogWarn(logCategory, fmt, valist)

/**
 * Logs information. See ::LEVEL_INFO
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logInfo(logCategory, ...) \
    TOKEN_PASTE(_logInfo, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logInfo() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogInfo(logCategory, fmt, valist) \
    _vlogInfo(logCategory, fmt, valist)

/**
 * Logs debug information. See ::LEVEL_DEBUG
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logDebug(logCategory, ...) \
    TOKEN_PASTE(_logDebug, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logDebug() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogDebug(logCategory, fmt, valist) \
    _vlogDebug(logCategory, fmt, valist)

/**
 * Logs trace information. See ::LEVEL_TRACE
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logTrace(logCategory, ...) \
    TOKEN_PASTE(_logTrace, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logTrace() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogTrace(logCategory, fmt, valist) \
    _vlogTrace(logCategory, fmt, valist)

/**
 * Logging to be used temporarily during development. See ::LEVEL_TEST
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] ... Minimally contains the @b formatStr, itself optionally containing format specifiers which will be replaced by the values specified in the additional arguments.
 * Depending on the @b formatStr, the function may expect a sequence of additional arguments,
 * each containing a value to be used to replace a format specifier in the format string (or a pointer to a storage location, for n).
 * There should be at least as many of these arguments as the number of values specified in the format specifiers.
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define logTest(logCategory, ...) \
    TOKEN_PASTE(_logTest, IS_EXTRA(__VA_ARGS__)(logCategory, __VA_ARGS__))

/**
 * A version of logTest() that is passed a set of arguments as a va_list.
 *
 * @param [in] logCategory ::LogCategory to log against
 * @param [in] fmt  The message string, including any format specifiers which will be replaced by the values specified in the argument list that follows.
 * @param [in] valist  The argument list
 * @retval ::LOG_OK Logged successfully.
 * @retval ::LOG_NOT_INITIALIZED ::initLogger must be called prior to logging.
 */
#define vlogTest(logCategory, fmt, valist) \
    _vlogTest(logCategory, fmt, valist)

/**
 * Checks if logging is active for the specified @p logCategory and @p logLevel.
 *
 * @param[in] logCategory ::LogCategory to check
 * @param[in] logLevel LogLevel to check
 *
 * @retval true if the @p logLevel is active for the given @p logCategory.
 * @retval false if the @p logLevel is NOT active for the given @p category.
 */
#define logIsActive(logCategory, logLevel) \
    (logCategory.currentLogLevel >= logLevel)

#ifdef __cplusplus
}
#endif

#endif /* SIMPLELOG_H_ */
