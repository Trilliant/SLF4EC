/**
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

#ifndef TEST_STDOUT_H_
#define TEST_STDOUT_H_

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <inttypes.h>
#include <cmockery.h>

#define STDOUT_TESTS                                     \
    unit_test(callInit),                                 \
        unit_test(callPublishFullFormatWith),            \
        unit_test(callPublishFullFormatWithoutFile),     \
        unit_test(callPublishFullFormatWithoutLine),     \
        unit_test(callPublishFullFormatWithoutFunction), \
        unit_test(tstSmlFct),                            \
        unit_test(tstSmlFile),                           \
        unit_test(callPublishMsgOnly)

void callInit(void** state);
void callPublishFullFormatWith(void** state);
void callPublishFullFormatWithoutFile(void** state);
void callPublishFullFormatWithoutLine(void** state);
void callPublishFullFormatWithoutFunction(void** state);
void callPublishMsgOnly(void** state);
void tstSmlFct(void** state);
void tstSmlFile(void** state);

void outputMessage(char* message);

#endif /* TEST_STDOUT_H_ */
