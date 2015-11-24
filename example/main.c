/**
 * @file
 *
 * Main file for SLF4EC's example
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

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "logConfig.h"
#include "stdout.h"
#include "slf4ecCtrl.h"

/**
 * Logger to Std Out.
 */
static Logger StdOut = {"StdOut", &initStdOut, 0, FORMAT_FULL, LEVEL_MAX, &logToStdOut};

static LogCategory* const categories[] = {LOG_CATEGORIES};
static Logger* const loggers[] = {&StdOut};
static const uint8_t categoriesLength = sizeof(categories) / sizeof(LogCategory*);
static const uint8_t loggersLength = sizeof(loggers) / sizeof(Logger*);

static uint64_t getTimestamp(void)
{
    return INVALID_TIME;
}

const GetLogTimestamp logTimeApi = &getTimestamp;

int main()
{
    Logger* const* curLoggers = NULL;
    int nbLoggers = 0;
    int i = 0;

    if (initLogger(categoriesLength, categories, loggersLength, loggers) != LOG_OK)
    {
        printf("Failed to initialize logging\n");
        return -1;
    }

    logInfo(GUI, "Application starting...");
    logDebug(Network, "The time is: %s.", __TIME__);
    logDebug(Network, "We are in file: \"%s\", function: \"%s\" and line \"%d\".", __FILE__, __func__, __LINE__);

    setLevels(LEVEL_MIN);
    Network.currentLogLevel = LEVEL_INFO;

    logInfo(GUI, "This line should not be logged");
    logInfo(Network, "While this line should be");

    setLevels(LEVEL_MAX);

    // Use the API to change a specific logger by name.
    nbLoggers = getLoggers(&curLoggers);
    for (i = 0; i < nbLoggers; i++)
    {
        if (strcmp(curLoggers[i]->loggerName, "StdOut") == 0)
        {
            curLoggers[i]->currentLogLevel = LEVEL_FATAL;
        }
    }

    // Or change it's level directly by reference
    StdOut.currentLogLevel = LEVEL_FATAL;

    logInfo(GUI, "This line should not be logged either");
    logFatal(GUI, "While this line should really be");

    StdOut.currentLogLevel = LEVEL_MAX;

    logOff(Network, "This line is kept in source code but removed at compilation. It only documents an older log that was removed");

    logInfo(GUI, "Application stopping...");

    return 0;
}
