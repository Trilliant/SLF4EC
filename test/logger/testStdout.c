/*
 * @file
 *
 * Tests for stdout.c
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

#include "testStdout.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "stdout.h"
#include "slf4ecTypes.h"

LogCategory stdoutCategory = {"stdoutCategory", LEVEL_WARN};
char message[8192];

void outputMessage(char* logMessage)
{
    strcpy(message, logMessage);
}

void callInit(void** state)
{
    (void) state;

    initStdOut(NULL);
}

void callPublishFullFormatWithoutFile(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = NULL;
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "] - dummyMessage", stdoutCategory.name, -1LLU);

    // Check result
    assert_string_equal(expected, message);
}

void callPublishFullFormatWithoutLine(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    const char* const dummyFile = __FILE__;
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = NULL, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "] - dummyMessage", stdoutCategory.name, -1LLU);

    // Check result
    assert_string_equal(expected, message);
}

void callPublishFullFormatWithoutFunction(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = __FILE__;
    const char* const dummyFct = NULL;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "] - dummyMessage", stdoutCategory.name, -1LLU);

    // Check result
    assert_string_equal(expected, message);
}

void callPublishFullFormatWith(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = __FILE__;
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    const char* realFct = dummyFct + (strlen(dummyFct) - 10);
    const char* realFile = dummyFile + (strlen(dummyFile) - 15);
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "]%s:%d(%s) - dummyMessage", stdoutCategory.name, -1LLU, realFile, dummyLine, realFct);

    // Check result
    assert_string_equal(expected, message);
}

void tstSmlFct(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = __FILE__;
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    const char* realFile = dummyFile + (strlen(dummyFile) - 15);
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "]%s:%d(%s) - dummyMessage", stdoutCategory.name, -1LLU, realFile, dummyLine, dummyFct);

    // Check result
    assert_string_equal(expected, message);
}

void tstSmlFile(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = "smallFile";
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_FULL);

    // Build expected result
    char expected[8192];
    sprintf(expected, "\n[ERROR][%s][%" PRIu64 "]%s:%d(%s) - dummyMessage", stdoutCategory.name, -1LLU, dummyFile, dummyLine, dummyFct);

    // Check result
    assert_string_equal(expected, message);
}

void callPublishMsgOnly(void** state)
{
    (void) state;

    // Prepare data
    uint8_t dummyLevel = LEVEL_ERROR;
    unsigned int dummyLine = __LINE__;
    const char* const dummyFile = __FILE__;
    const char* const dummyFct = __FUNCTION__;
    va_list dummyVaList;

    // Execute test
    LogRecord record = {.category = &stdoutCategory, .file = dummyFile, .formatStr = "dummyMessage", .function = dummyFct, .level = &dummyLevel, .line = &dummyLine, .vaList = &dummyVaList};
    logToStdOut(&record, FORMAT_MSG_ONLY);

    // Build expected result
    char expected[8192];
    sprintf(expected, "\ndummyMessage");

    // Check result
    assert_string_equal(expected, message);
}
