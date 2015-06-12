#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    Makefile                                                             #
#                                                                               #
# Author:  Jérémie Faucher-Goulet                                               #
#                                                                               #
#   www.trilliantinc.com                                                        #
#                                                                               #
# The MIT License (MIT)                                                         #
#                                                                               #
# Copyright (c) 2015 Trilliant                                                  #
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
# Override standard Makefile
#################################################################################

BINDIR_REL ?= bin/cortex-m4

include Makefile

#################################################################################
# Cross compiler
#################################################################################
ifeq ($(COMPILER), iar)
    CROSS_COMPILE := $(ARM_IAR_HOME)\bin
    CC := "$(CROSS_COMPILE)\iccarm.exe"
else
    CROSS_COMPILE := "$(ARM_GCC_HOME)/bin/arm-none-eabi-"
    CC := $(CROSS_COMPILE)gcc
    OBJCOPY := $(CROSS_COMPILE)objcopy
    OBJDUMP := $(CROSS_COMPILE)objdump
    SIZE := $(CROSS_COMPILE)size
endif

#################################################################################
# Compiler options
#################################################################################

ifeq ($(COMPILER), iar)
    CFLAGS += --cpu=Cortex-M4 --cpu_mode=thumb
else
    CFLAGS += -c -mcpu=cortex-m4 -mthumb -g -mabi=aapcs
    LDFLAGS += -mcpu=cortex-m4 -mthumb -lm -mabi=aapcs --gc-sections
endif


#################################################################################
# Makefile dependency checks
#################################################################################

# Check for ARM Toolchain
ifndef ARM_GCC_HOME
    $(warning ARM_GCC_HOME must point to the ARM Toolchain to support compilation)
endif

ifeq ($(findstring Windows,$(OS)),Windows)
    ifndef ARM_IAR_HOME
        $(warning ARM_IAR_HOME must point to the directory where IAR is installed)
    endif
endif