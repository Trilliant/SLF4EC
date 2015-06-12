/*
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

#ifndef TEST_LOG_H_
#define TEST_LOG_H_

#include "log.h"
#include "slf4ecCtrl.h"
#include "testStdout.h"

#define LOG_TESTS                           \
    unit_test(initializeBadParams),         \
        unit_test(setLevelsNotInitialized), \
        unit_test(logNotInitialized),       \
        unit_test(initializeLogOK),         \
        unit_test(setBadLogLevel),          \
        unit_test(testGetCategories),       \
        unit_test(testGetLoggers),          \
        unit_test(testLogLevel),            \
        unit_test(testLocationInfo),        \
        unit_test(testLogInfo),             \
        unit_test(testLogLevelNames),       \
        STDOUT_TESTS

void initializeBadParams(void** state);
void setLevelsNotInitialized(void** state);
void logNotInitialized(void** state);
void initializeLogOK(void** state);
void setBadLogLevel(void** state);
void testGetCategories(void** state);
void testGetLoggers(void** state);
void testLogLevel(void** state);
void testLocationInfo(void** state);
void testLogInfo(void** state);
void testLogLevelNames(void** state);

#endif /* TEST_LOG_H_ */
