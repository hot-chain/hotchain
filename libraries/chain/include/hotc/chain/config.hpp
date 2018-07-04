/*
 * Copyright (c) 2017, Respective Authors.
 *
 * The MIT License
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
#pragma once

#define HOTC_KEY_PREFIX         "HOTC"
#define HOTC_BLOCK_INTERVAL_SEC 3

/** percentage fields are fixed point with a denominator of 10,000 */
#define HOTC_100_PERCENT                               10000
#define HOTC_1_PERCENT                                 (HOTC_100_PERCENT/100)
#define HOTC_DEFAULT_MAX_BLOCK_SIZE                    (256*1024)

#define HOTC_DEFAULT_PRODUCER_PAY_PER_BLOCK            (HOTC_BLOCKCHAIN_PRECISION * int64_t( 10) )
#define HOTC_DEFAULT_MAX_TIME_UNTIL_EXPIRATION         (60*60)

#define HOTC_IRREVERSIBLE_THRESHOLD                      (70 * HOTC_1_PERCENT)


