/**
 * @file
 *
 * Logging logic
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

#include <stdbool.h>
#include "slf4ec/slf4ec.h"
#include "slf4ec/slf4ecCtrl.h"

#define LOGGER_ALREADY_INITIALIZED "Logger already initialized!\n"
#define LOGGER_NOT_INITIALIZED "Logger is not initialized!\n"

const char* const logLevelNames[] = {"OFF", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE", "TEST"};

static uint8_t nbCategories;
static uint8_t nbLoggers;
static LogCategory* const* categories;
static Logger* const* loggers;
static bool isInitialized = false;

static bool isCategoryActive(const LogCategory* const category, const uint8_t* const level);
static LogResult _privateLog(const char* const file,
                        const uint32_t* const line,
                        const char* const function,
                        const LogCategory* const category,
                        const uint8_t* const level,
                        const char* const formatStr,
                        va_list vaList);

uint8_t getCategories(LogCategory* const** _categories)
{
    *_categories = categories;
    return nbCategories;
}

uint8_t getLoggers(Logger* const** _loggers)
{
    *_loggers = loggers;
    return nbLoggers;
}

LogResult initLogger(const uint8_t _nbCategories,
                     LogCategory* const* _categories,
                     const uint8_t _nbLoggers,
                     Logger* const* _loggers)
{
    LogResult returnCode = LOG_OK;
    bool allLoggersOk = true;

    if ((_nbCategories > 0 && _categories == NULL) || (_nbLoggers > 0 && _loggers == NULL))
    {
        returnCode = LOG_INVALID_PARAMETER;
    }
    else
    {
        if (!isInitialized)
        {
            nbCategories = _nbCategories;
            categories = _categories;
            nbLoggers = _nbLoggers;
            loggers = _loggers;

            uint_fast8_t i;
            for (i = 0; i < nbLoggers; i++)
            {
                if (loggers[i]->initFct == NULL || loggers[i]->publishFct == NULL)
                {
                    allLoggersOk = false;
                    returnCode = LOG_INVALID_PARAMETER;
                    break;
                }
                loggers[i]->initFct(loggers[i]->initArgs);
            }

            isInitialized = allLoggersOk;
        }
        else
        {
            returnCode = LOG_ALREADY_INITIALIZED;
        }
    }

    return returnCode;
}

LogResult setLevels(const uint8_t level)
{
    LogResult returnCode = LOG_OK;

#if LEVEL_MIN != 0
#error "Need to add condition 'level < LEVEL_MIN' to if below"
#endif

    if (level > COMPILED_LOG_LEVEL)
    {
        returnCode = LOG_INVALID_PARAMETER;
    }
    else
    {
        if (isInitialized)
        {
            uint_fast8_t i;
            for (i = 0; i < nbCategories; i++)
            {
                if (categories[i]) {
                    categories[i]->currentLogLevel = level;
                }
            }
        }
        else
        {
            returnCode = LOG_NOT_INITIALIZED;
        }
    }

    return returnCode;
}

static void publishToLoggers(const LogRecord* const record)
{
    int i;
    for (i = 0; i < nbLoggers; i++)
    {
        if (loggers[i]->currentLogLevel >= *record->level)
        {
            loggers[i]->publishFct(record, loggers[i]->format);
        }
    }
}

LogResult noLog()
{
    LogResult returnCode = LOG_OK;

    if (!isInitialized)
    {
        returnCode = LOG_NOT_INITIALIZED;
    }

    return returnCode;
}

#ifndef USE_LOCATION_INFO
LogResult nfLog0(const LogCategory* const category, const uint8_t level, const char* const msg)
{
    va_list vaList;
    return _privateLog(NULL, NULL, NULL, category, &level, msg, vaList);
}

LogResult nfLog1(const LogCategory* const category, const uint8_t level, const char* const formatStr, ...)
{
    LogResult returnCode;
    va_list vaList;
    va_start(vaList, formatStr);
    returnCode = _privateLog(NULL, NULL, NULL, category, &level, formatStr, vaList);
    va_end(vaList);
    return returnCode;
}

LogResult nfLogv(const LogCategory* category, const uint8_t level, const char* formatStr, va_list vaList)
{
    return _privateLog(NULL, NULL, NULL, category, &level, formatStr, vaList);
}
#else
LogResult yfLog0(const char* const file,
                 const uint32_t line,
                 const char* const function,
                 const LogCategory* const category,
                 const uint8_t level,
                 const char* const msg)
{
    va_list vaList;
    return _privateLog(file, &line, function, category, &level, msg, vaList);
}

LogResult yfLog1(const char* const file,
                 const uint32_t line,
                 const char* const function,
                 const LogCategory* const category,
                 const uint8_t level,
                 const char* const formatStr, ...)
{
    LogResult returnCode;
    va_list vaList;
    va_start(vaList, formatStr);
    returnCode = _privateLog(file, &Line, function, category, &level, formatStr, vaList);
    va_end(vaList);
    return returnCode;
}

LogResult yfLogv(const char* const file,
                 const uint32_t line,
                 const char* const function,
                 const LogCategory* const category,
                 const uint8_t level,
                 const char* const formatStr,
                 va_list vaList)
{
    return _privateLog(file, &Line, function, category, &level, formatStr, vaList);
}
#endif

static LogResult _privateLog(const char* const file,
                        const uint32_t* const line,
                        const char* const function,
                        const LogCategory* const category,
                        const uint8_t* const level,
                        const char* const formatStr,
                        va_list vaList)
{
    va_list ap;
    va_copy(ap, vaList);

    if (!isInitialized)
    {
        return LOG_NOT_INITIALIZED;
    }

    if (isCategoryActive(category, level))
    {
        const uint64_t timestamp = logTimeApi();

        LogRecord curRecord =
        {
            .file = file,
            .line = line,
            .function = function,
            .timestamp = &timestamp,
            .category = category,
            .level = level,
            .formatStr = formatStr,
            .vaList = &ap
        };

        publishToLoggers(&curRecord);
    }

    return LOG_OK;
}

static inline bool isCategoryActive(const LogCategory* const category, const uint8_t* const level)
{
    bool isActive = false;

    if (category->currentLogLevel >= *level)
    {
        isActive = true;
    }

    return isActive;
}
