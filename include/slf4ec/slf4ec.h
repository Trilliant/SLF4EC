/**
 * @file
 *
 * Full interface for SLF4EC
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

#ifndef SLF4EC_H_
#define SLF4EC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "slf4ec/slf4ecTypes.h"

#if defined(UNIT_TESTING) && !defined(USE_LOCATION_INFO)
#define USE_LOCATION_INFO
#endif

#ifdef __func__
#define FUNCTION __func__
#else
#define FUNCTION __FUNCTION__
#endif

/**********************************************************************************************************************************************
 * NOTE: A maximum of 64 arguments can be passed to the logger to format the string. This is due to the complex macro logic below to support
 * empty __VA_ARGS__ with IAR. In GCC the '##' operator would swallow the trailing comma so wouldn't require this complex logic.
 *
 * Problem described here:
 * http://w3facility.org/question/swallowing-comma-in-variadic-macros-on-compilers-that-do-not-recognise/
 * http://en.wikipedia.org/wiki/Variadic_macro#Trailing_comma
 *
 * Solution based on information found here:
 * https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 * http://www.cnblogs.com/shangdawei/archive/2013/06/09/3129233.html
 * http://smackerelofopinion.blogspot.ca/2011/10/determining-number-of-arguments-in-c.html
 * http://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
 *
 * In SLF4EC, a function suffixed with 0 does have 1 or more arguments passed through __VA_ARGS__.
 *            a function suffixed with 1 does not have extra arguments. __VA_ARGS__ is empty.
 * ****************************************************************************************************************************************** */

// Utility function to count by right shifting values using __VA_ARGS__. Counts to a maximum of 64.
// https://gustedt.wordpress.com/2010/06/03/default-arguments-for-c99/
#define ARG64(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25,      \
              _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
              _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, ...) _64

// Check if there is more than one parameter
// https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
#define HAS_EXTRA_PARAMS(...) ARG64(__VA_ARGS__, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
                                    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0)

#define TRIGGER_PARENTHESIS_(...) ,

#define IS_EXTRA(...) \
IS_EXTRA_(                                                                                                                                                    \
    HAS_EXTRA_PARAMS(__VA_ARGS__),                                  /**< test if there is just one argument, eventually an empty one */                       \
    HAS_EXTRA_PARAMS(TRIGGER_PARENTHESIS_ __VA_ARGS__),             /**< test if _TRIGGER_PARENTHESIS_ together with the argument adds a comma */             \
    HAS_EXTRA_PARAMS(__VA_ARGS__(/* empty */)),                     /**< test if the argument together with a parenthesis adds a comma */                     \
    HAS_EXTRA_PARAMS(TRIGGER_PARENTHESIS_ __VA_ARGS__(/* empty */)) /**< test if placing it between _TRIGGER_PARENTHESIS_ and the parenthesis adds a comma */ \
    )

#define PASTE5(_0, _1, _2, _3, _4) _0##_1##_2##_3##_4
#define IS_EXTRA_(_0, _1, _2, _3) HAS_EXTRA_PARAMS(PASTE5(IS_EXTRA_CASE_, _0, _1, _2, _3))
#define IS_EXTRA_CASE_1111 ,

#define TOKEN_PASTE(x, y) TOKEN_PASTE_(x, y)
#define TOKEN_PASTE_(x, y) x##y

/**********************************************************************************************************************************************
 * End of the complex macro logic for empty varargs
 * ****************************************************************************************************************************************** */

#ifndef USE_LOCATION_INFO
/**
 * Private function called by macros to log without location information.
 */
LogResult nfLog0(const LogCategory* category, const uint8_t level, const char* msg);
LogResult nfLog1(const LogCategory* category, const uint8_t level, const char* formatStr, ...);
LogResult nfLogv(const LogCategory* category, const uint8_t level, const char* formatStr, va_list vaList);
#else
/**
 * Private function called by macros to log with location information.
 */
LogResult yfLog0(const char* file, const uint32_t line, const char* function, const LogCategory* category, const uint8_t level, const char* msg);
LogResult yfLog1(const char* file, const uint32_t line, const char* function, const LogCategory* category, const uint8_t level, const char* formatStr, ...);
LogResult yfLogv(const char* file, const uint32_t line, const char* function, const LogCategory* category, const uint8_t level, const char* formatStr, va_list vaList);
#endif

/**
 * Private function called by macros when logging is not compiled in.
 */
LogResult noLog();

/*
 ************************************************************
 * Handles log levels available at compile time
 ************************************************************
 */
#define _logOff0(logCategory, ...) \
    noLog()
#define _logOff1(logCategory, ...) \
    noLog()
#define _vlogOff(logCategory, ...) \
    noLog()

#if COMPILED_LOG_LEVEL >= LEVEL_FATAL
#define _logFatal0(logCategory, ...) \
    _log0(logCategory, LEVEL_FATAL, __VA_ARGS__)
#define _logFatal1(logCategory, ...) \
    _log1(logCategory, LEVEL_FATAL, __VA_ARGS__)
#define _vlogFatal(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_FATAL, fmt, valist)
#else
#define _logFatal0(logCategory, ...) \
    noLog()
#define _logFatal1(logCategory, ...) \
    noLog()
#define _vlogFatal(logCategory, fmt, valist) \
    nolog()
#endif

#if COMPILED_LOG_LEVEL >= LEVEL_ERROR
#define _logError0(logCategory, ...) \
    _log0(logCategory, LEVEL_ERROR, __VA_ARGS__)
#define _logError1(logCategory, ...) \
    _log1(logCategory, LEVEL_ERROR, __VA_ARGS__)
#define _vlogError(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_ERROR, fmt, valist)
#else
#define _logError0(logCategory, ...) \
    noLog()
#define _logError1(logCategory, ...) \
    noLog()
#define _vlogError(logCategory, fmt, valist) \
    nolog()
#endif

#if COMPILED_LOG_LEVEL >= LEVEL_WARN
#define _logWarn0(logCategory, ...) \
    _log0(logCategory, LEVEL_WARN, __VA_ARGS__)
#define _logWarn1(logCategory, ...) \
    _log1(logCategory, LEVEL_WARN, __VA_ARGS__)
#define _vlogWarn(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_WARN, fmt, valist)
#else
#define _logWarn0(logCategory, ...) \
    noLog()
#define _logWarn1(logCategory, ...) \
    noLog()
#define _vlogWarn(logCategory, fmt, valist) \
    nolog()
#endif

#if COMPILED_LOG_LEVEL >= LEVEL_INFO
#define _logInfo0(logCategory, ...) \
    _log0(logCategory, LEVEL_INFO, __VA_ARGS__)
#define _logInfo1(logCategory, ...) \
    _log1(logCategory, LEVEL_INFO, __VA_ARGS__)
#define _vlogInfo(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_INFO, fmt, valist)
#else
#define _logInfo0(logCategory, ...) \
    noLog()
#define _logInfo1(logCategory, ...) \
    noLog()
#define _vlogInfo(logCategory, fmt, valist) \
    nolog()
#endif

#if COMPILED_LOG_LEVEL >= LEVEL_DEBUG
#define _logDebug0(logCategory, ...) \
    _log0(logCategory, LEVEL_DEBUG, __VA_ARGS__)
#define _logDebug1(logCategory, ...) \
    _log1(logCategory, LEVEL_DEBUG, __VA_ARGS__)
#define _vlogDebug(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_DEBUG, fmt, valist)
#else
#define _logDebug0(logCategory, ...) \
    noLog()
#define _logDebug1(logCategory, ...) \
    noLog()
#define _vlogDebug(logCategory, fmt, valist) \
    nolog()
#endif

#if COMPILED_LOG_LEVEL >= LEVEL_TRACE
#define _logTrace0(logCategory, ...) \
    _log0(logCategory, LEVEL_TRACE, __VA_ARGS__)
#define _logTrace1(logCategory, ...) \
    _log1(logCategory, LEVEL_TRACE, __VA_ARGS__)
#define _vlogTrace(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_TRACE, fmt, valist)
#else
#define _logTrace0(logCategory, ...) \
    noLog()
#define _logTrace1(logCategory, ...) \
    noLog()
#define _vlogTrace(logCategory, fmt, valist) \
    nolog()
#endif

#define _logTest0(logCategory, ...) \
    _log0(logCategory, LEVEL_TEST, __VA_ARGS__)
#define _logTest1(logCategory, ...) \
    _log1(logCategory, LEVEL_TEST, __VA_ARGS__)
#define _vlogTest(logCategory, fmt, valist) \
    _logv(logCategory, LEVEL_TEST, fmt, valist)

/*
 ************************************************************
 * Handles logging of location info at compile time
 ************************************************************
 */

#ifdef USE_LOCATION_INFO
#define _log0(logCategory, level, ...) \
    yfLog0(__FILE__, __LINE__, FUNCTION, &logCategory, level, __VA_ARGS__)
#define _log1(logCategory, level, ...) \
    yfLog1(__FILE__, __LINE__, FUNCTION, &logCategory, level, __VA_ARGS__)
#define _logv(logCategory, level, fmt, vaList) \
    yfLogv(__FILE__, __LINE__, FUNCTION, &logCategory, level, fmt, vaList)
#else
#define _log0(logCategory, level, ...) \
    nfLog0(&logCategory, level, __VA_ARGS__)
#define _log1(logCategory, level, ...) \
    nfLog1(&logCategory, level, __VA_ARGS__)
#define _logv(logCategory, level, fmt, vaList) \
    nfLogv(&logCategory, level, fmt, vaList)
#endif

#ifdef __cplusplus
}
#endif

#endif /* SLF4EC_H_ */
