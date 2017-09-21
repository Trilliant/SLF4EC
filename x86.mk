#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    x86.mk                                                               #
#                                                                               #
# Author:  Jérémie Faucher-Goulet                                               #
#                                                                               #
#   www.trilliantinc.com                                                        #
#                                                                               #
# The MIT License (MIT)                                                         #
#                                                                               #
# Copyright (c) 2017 Trilliant                                                  #
#                                                                               #
# Permission is hereby granted, free of charge, to any person obtaining a copy  #
# of this software and associated documentation files (the "Software"), to deal #
# in the Software without restriction, including without limitation the rights  #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     #
# copies of the Software, and to permit persons to whom the Software is         #
# furnished to do so, subject to the following conditions:                      #
#                                                                               #
# The above copyright notice and this permission notice shall be included in    #
# all copies or substantial portions of the Software.                           #
#                                                                               #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     #
# THE SOFTWARE.                                                                 #
#################################################################################

#################################################################################
# Cross compiler
#################################################################################
ifeq ($(COMPILER),$(GCC_COMPILER))
    AR := ar
    CC := gcc
    CXX := g++
    OBJDUMP := objdump
endif

#################################################################################
# Dependency checks
#################################################################################

ifneq ($(COMPILER),$(GCC_COMPILER))
    $(error $(COMPILER) is not supported!)
endif
