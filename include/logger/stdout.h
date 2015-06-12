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

#ifndef STDOUT_H_
#define STDOUT_H_

#include "slf4ecTypes.h"

/**
 * Function to be called when recording a log (for logger configuration)
 *
 * @param [in] logRecord Pointer to the record to be logged.
 * @param [in] format Format to be used when recording this log.
 */
void logToStdOut(const LogRecord* const logRecord, const LogFormat format);

/**
 * Function to be called when initializing this logger (for logger configuration)
 *
 *  @param [in] param Pointer to parameters used when configuring this logger.
 */
void initStdOut(const void* const param);

#endif /* STDOUT_H_ */
