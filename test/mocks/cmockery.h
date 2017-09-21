/**
 * @file
 *
 * Includes every dependencies needed by cmockery
 *
 * @author Jérémie Faucher-Goulet
 *
 * @copyright Trilliant Inc. © 2016 - http://www.trilliantinc.com
 */

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include_next "cmockery.h"

/* Needed because original function "will_return" does a call to cast_to_largest_integral_type(),
 * which strips the upper 32-bits of a 64-bit integer. This happens because there is an intermediate
 * cast using size_t
 */
#define will_return64(function, value)          \
    _will_return(#function, __FILE__, __LINE__, \
                 value, 1)
