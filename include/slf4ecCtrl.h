/**
 * @file
 *
 * API for controlling SLF4EC
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

#ifndef LOG_CTRL_H_
#define LOG_CTRL_H_

#include "slf4ecTypes.h"

#ifndef COMPILED_LOG_LEVEL
#define COMPILED_LOG_LEVEL LEVEL_MAX
#else
#if COMPILED_LOG_LEVEL > LEVEL_MAX
#error "COMPILED_LOG_LEVEL cannot be higher than 'LEVEL_MAX'"
#endif
#endif

extern const char* const logLevelNames[]; /**< Array containing the name for each log levels */
extern const GetLogTimestamp logTimeApi;  /**< Access to the configured timeApi */

/**
 * Initialize logging by specifying which logging categories are available and loggers for output.
 *
 * @remark This function is not thread safe.
 *
 * @param [in] nbCategories Number of categories in the @p categories parameter
 * @param [in] categories Configured categories
 * @param [in] nbLoggers Number of loggers in the @p loggers parameter
 * @param [in] loggers Configured loggers
 * @retval ::LOG_OK Logging initialized successfully.
 * @retval ::LOG_INVALID_PARAMETER when @p categories or @p loggers are not valid.
 * @retval ::LOG_ALREADY_INITIALIZED ::initLogger was already called previously.
 */
LogResult initLogger(const uint8_t nbCategories, LogCategory* const* categories, const uint8_t nbLoggers, Logger* const* loggers);

/**
 * Set the LogLevel for all categories.
 *
 * @remark This function is not thread safe.
 *
 * @param [in] level LogLevel to set.
 * @retval ::LOG_OK @p level applied successfully to all categories.
 * @retval ::LOG_INVALID_PARAMETER when @p level doesn't exist.
 */
LogResult setLevels(const uint8_t level);

/**
 * Retrieve the list of configured categories.
 *
 * @param [out] categories Pointer to an array of configured categories
 * @return Number of configured categories
 */
uint8_t getCategories(LogCategory* const** categories);

/**
 * Retrieve the list of configured loggers.
 *
 * @param [out] loggers Pointer to an array of configured loggers
 * @return Number of configured loggers
 */
uint8_t getLoggers(Logger* const** loggers);

#endif /* LOG_CTRL_H_ */
