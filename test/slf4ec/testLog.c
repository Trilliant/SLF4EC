/**
 * @file
 *
 * Test slf4ec.c
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
#include "testLog.h"

#define INVALID_TIME (-1LLU);

static uint64_t getTimestamp(void)
{
    return INVALID_TIME;
}
const GetLogTimestamp logTimeApi = &getTimestamp;

static const va_list dummyVaList;
static const uint8_t expectedArg = 33;
static uint8_t arg = 0;

static LogCategory* curCategory;
static LogFormat curFormat;
static char* curFile;
static int curLine;
static int* curLinePtr;
static char* curFct;
static uint8_t curLevel;
static char* curFormatStr;
static bool publishCalled = false;

/* Make accessible functions that are hidden when USE_LOCATION_INFO is enabled */
extern LogResult nfLog0(const LogCategory* const category, const uint8_t level, const char* const msg);
extern LogResult nfLog1(const LogCategory* const category, const uint8_t level, const char* const formatStr, ...);
extern LogResult nfLogv(const LogCategory* category, const uint8_t level, const char* formatStr, va_list vaList);

void dummyInit(const void* const config)
{
    arg = *((uint8_t*) config);
}

void dummyPublisher(const LogRecord* const logRecord, const LogFormat format)
{
    publishCalled = true;

    curCategory = (LogCategory*) logRecord->category;
    curFormat = format;
    curFile = (char*) logRecord->file;
    curLinePtr = (int*) logRecord->line;
    if (curLinePtr)
    {
        curLine = *curLinePtr;
    }
    curFct = (char*) logRecord->function;
    curLevel = *logRecord->level;
    curFormatStr = (char*) logRecord->formatStr;
}

LogCategory dummyCategory = {"DummyCategory", LEVEL_INFO};
Logger dummyLogger = {"DummyLogger", &dummyInit, &expectedArg, FORMAT_FULL, LEVEL_DEBUG, &dummyPublisher};
Logger noInitLogger = {"NoInitLogger", NULL, &expectedArg, FORMAT_FULL, LEVEL_DEBUG, &dummyPublisher};
Logger noPublishLogger = {"NoPublishLogger", &dummyInit, &expectedArg, FORMAT_FULL, LEVEL_DEBUG, NULL};

static LogCategory* const dummyCategories[] = {
    &dummyCategory};
static Logger* const dummyLoggers[] = {
    &dummyLogger};
static Logger* const badLoggers1[] = {&noInitLogger};
static Logger* const badLoggers2[] = {&noPublishLogger};

void initializeBadParams(void** state)
{
    (void) state;

    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(1, NULL, 0, NULL));
    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(0, NULL, 1, NULL));
    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(1, dummyCategories, 1, NULL));
    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(1, NULL, 1, dummyLoggers));
    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(1, dummyCategories, 1, badLoggers1));
    assert_int_equal(LOG_INVALID_PARAMETER, initLogger(1, dummyCategories, 1, badLoggers2));
}

void setLevelsNotInitialized(void** state)
{
    (void) state;

    assert_int_equal(LOG_NOT_INITIALIZED, setLevels(LEVEL_WARN));
}

void logNotInitialized(void** state)
{
    (void) state;

    LogResult res = logInfo(dummyCategory, "Dummy message");
    assert_int_equal(LOG_NOT_INITIALIZED, res);

    res = logInfo(dummyCategory, "Dummy message %s", "lol");
    assert_int_equal(LOG_NOT_INITIALIZED, res);

    res = logOff(dummyCategory, "Dummy message");
    assert_int_equal(LOG_NOT_INITIALIZED, res);
}

void initializeLogOK(void** state)
{
    (void) state;

    //assert_int_equal(initLogger(0, NULL, 0, NULL, &getTime), LOG_OK);
    assert_int_equal(LOG_OK, initLogger(1, dummyCategories, 1, dummyLoggers));
    assert_int_equal(expectedArg, arg);
    assert_int_equal(LOG_ALREADY_INITIALIZED, initLogger(0, NULL, 0, NULL));
}

void setBadLogLevel(void** state)
{
    (void) state;

    assert_int_equal(LOG_INVALID_PARAMETER, setLevels(0xFF));
}

void testGetCategories(void** state)
{
    (void) state;

    LogCategory* const* categories;
    uint8_t nbCategories = getCategories(&categories);
    assert_int_equal(1, nbCategories);
    assert_int_equal(&dummyCategory, categories[0]);
}

void testGetLoggers(void** state)
{
    (void) state;

    Logger* const* loggers = NULL;
    uint8_t nbLoggers = getLoggers(&loggers);
    assert_int_equal(1, nbLoggers);
    assert_int_equal(&dummyLogger, loggers[0]);
}

void testLogLevel(void** state)
{
    dummyCategory.currentLogLevel = LEVEL_INFO;

    (void) state;
    curLevel = LEVEL_OFF;
    logOff(dummyCategory, "DummyMessage");
    assert_false(publishCalled);

    publishCalled = false;
    logFatal(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_FATAL, curLevel);

    publishCalled = false;
    logError(dummyCategory, "DummyMessage %s", "lol");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_ERROR, curLevel);

    publishCalled = false;
    logWarn(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_WARN, curLevel);

    publishCalled = false;
    logInfo(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_INFO, curLevel);

    publishCalled = false;
    logDebug(dummyCategory, "DummyMessage");
    assert_false(publishCalled);

    publishCalled = false;
    logDebug(dummyCategory, "DummyMessage %s", "lol");
    assert_false(publishCalled);

    setLevels(LEVEL_TEST);

    publishCalled = false;
    logDebug(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_DEBUG, curLevel);

    publishCalled = false;
    logDebug(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_DEBUG, curLevel);

    publishCalled = false;
    logTrace(dummyCategory, "DummyMessage");
    assert_false(publishCalled);

    dummyLogger.currentLogLevel = LEVEL_TEST;

    publishCalled = false;
    logTrace(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_TRACE, curLevel);

    publishCalled = false;
    logTest(dummyCategory, "DummyMessage %s", "lol");
    assert_true(publishCalled);
    assert_int_equal(LEVEL_TEST, curLevel);
}

void testLocationInfo(void** state)
{
    (void) state;

    publishCalled = false;
    logInfo(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(__LINE__ - 2, curLine);
    assert_string_equal(__FUNCTION__, curFct);
    assert_string_equal(__FILE__, curFile);
}

void testNoLocationInfoWithoutArg(void** state)
{
    (void) state;

    publishCalled = false;
    nfLog0(&dummyCategory, LEVEL_INFO, "Some message");
    assert_true(publishCalled);
    assert_ptr_equal(NULL, curLinePtr);
    assert_ptr_equal(NULL, curFct);
    assert_ptr_equal(NULL, curFile);
}

void testNoLocationInfoWithArg(void** state)
{
    (void) state;

    publishCalled = false;
    nfLog1(&dummyCategory, LEVEL_INFO, "Some message: %d", 23);
    assert_true(publishCalled);
    assert_ptr_equal(NULL, curLinePtr);
    assert_ptr_equal(NULL, curFct);
    assert_ptr_equal(NULL, curFile);
}

void testLogWithVaArgWithLocInfo(void** state)
{
    (void) state;

    publishCalled = false;
    vlogInfo(dummyCategory, "Some format string", dummyVaList);
    assert_true(publishCalled);
    assert_int_equal(__LINE__ - 2, curLine);
    assert_string_equal(__FUNCTION__, curFct);
    assert_string_equal(__FILE__, curFile);
}

void testLogWithVaArgWithoutLocInfo(void** state)
{
    (void) state;

    publishCalled = false;
    nfLogv(&dummyCategory, LEVEL_INFO, "Some format string", dummyVaList);
    assert_true(publishCalled);
    assert_ptr_equal(NULL, curLinePtr);
    assert_ptr_equal(NULL, curFct);
    assert_ptr_equal(NULL, curFile);
}

void testLogInfo(void** state)
{
    (void) state;

    publishCalled = false;
    logInfo(dummyCategory, "DummyMessage");
    assert_true(publishCalled);
    assert_int_equal(FORMAT_FULL, curFormat);
    assert_ptr_equal(&dummyCategory, curCategory);
    assert_string_equal("DummyMessage", curFormatStr);
}

void testLogLevelNames(void** state)
{
    (void) state;

    assert_string_equal("OFF", logLevelNames[LEVEL_OFF]);
    assert_string_equal("FATAL", logLevelNames[LEVEL_FATAL]);
    assert_string_equal("ERROR", logLevelNames[LEVEL_ERROR]);
    assert_string_equal("WARN", logLevelNames[LEVEL_WARN]);
    assert_string_equal("INFO", logLevelNames[LEVEL_INFO]);
    assert_string_equal("DEBUG", logLevelNames[LEVEL_DEBUG]);
    assert_string_equal("TRACE", logLevelNames[LEVEL_TRACE]);
    assert_string_equal("TEST", logLevelNames[LEVEL_TEST]);
}
