/*
 * @file
 *
 * Logger for printing to STDOUT
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

#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#ifdef UNIT_TESTING
#include "testStdout.h"
#endif

#include "slf4ecCtrl.h"
#include "stdout.h"

static void logFull(const LogRecord* const logRecord);
static void logMsgOnly(const LogRecord* const logRecord);

#define PRINTF_WITH_LOCATION "\n[%s][%s][%" PRIu64 "]%s:%d(%s) - ", logLevelNames[*record->level], record->category->name, logTimeApi(), \
                             record->file + (fileLength > MAX_FILE_LENGTH ? (fileLength - MAX_FILE_LENGTH) : 0), *record->line,        \
                             record->function + (fctLength > MAX_FCT_LENGHT ? (fctLength - MAX_FCT_LENGHT) : 0)
#define PRINTF_WITHOUT_LOCATION "\n[%s][%s][%" PRIu64 "] - ", logLevelNames[*record->level], record->category->name, logTimeApi()
#define VPRINTF record->formatStr, *record->vaList

void initStdOut(const void* const param)
{
    // Nothing to initialize
    (void) param;
}

void logToStdOut(const LogRecord* const logRecord, const LogFormat format)
{
    switch (format)
    {
        case FORMAT_MSG_ONLY:
            logMsgOnly(logRecord);
            break;
        case FORMAT_FULL:
        default:
            logFull(logRecord);
            break;
    }
}

static void logMsgOnly(const LogRecord* const record)
{
#ifdef UNIT_TESTING
    char suffixMsg[4096];
    char fullMsg[8192];
    fullMsg[0] = '\n';
    fullMsg[1] = '\0';

    vsprintf(suffixMsg, VPRINTF);
    strcat(fullMsg, suffixMsg);
    outputMessage(fullMsg);
#else
    printf("\n");
    vprintf(VPRINTF);
#endif
}

// SONAR cannot parse this function
static void logFull(const LogRecord* const record)
{
    if (record->file == NULL || record->line == NULL || record->function == NULL)
    {
#ifdef UNIT_TESTING
        char suffixMsg[4096];
        char fullMsg[8192];

        sprintf(fullMsg, PRINTF_WITHOUT_LOCATION);
        vsprintf(suffixMsg, VPRINTF);
        strcat(fullMsg, suffixMsg);

        outputMessage(fullMsg);
#else
        printf(PRINTF_WITHOUT_LOCATION);
        vprintf(VPRINTF);
#endif
    }
    else
    {
        size_t fileLength = strlen(record->file);
        size_t fctLength = strlen(record->function);

#ifdef UNIT_TESTING
        char suffixMsg[4096];
        char fullMsg[8192];

        sprintf(fullMsg, PRINTF_WITH_LOCATION);
        vsprintf(suffixMsg, VPRINTF);
        strcat(fullMsg, suffixMsg);

        outputMessage(fullMsg);
#else
        printf(PRINTF_WITH_LOCATION);
        vprintf(VPRINTF);
#endif
    }
}
