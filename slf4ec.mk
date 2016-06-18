#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    slf4ec.mk                                                            #
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
SLF4EC_ROOT			:= $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

#################################################################################
# Constants
#################################################################################

SLF4EC_NAME := slf4ec

#################################################################################
# Current configuration
#################################################################################

ifdef ARCH
    SLF4EC_ARCH			?= ${ARCH}
endif

ifdef COMPILER
    SLF4EC_COMPILER		?= ${COMPILER}
endif

ifdef DEBUG
    SLF4EC_CONFIG		:= debug
else
    SLF4EC_CONFIG		:= release
endif

ifdef USE_LOCATION_INFO
    SLF4EC_USELOC		:= withLoc
else
    SLF4EC_USELOC		:= withoutLoc
endif

#################################################################################
# Paths and project structure
#################################################################################
SLF4EC_INCDIR			:= include
SLF4EC_SRCDIR			:= src
SLF4EC_BINDIR			:= bin

SLF4EC_PATH		:= $(SLF4EC_BINDIR)/$(SLF4EC_ARCH)/$(SLF4EC_COMPILER)/$(SLF4EC_USELOC)/$(SLF4EC_CONFIG)

SLF4EC_LIB		:= $(SLF4EC_NAME).a

# Generated lib file
SLF4EC_FILE		:= $(SLF4EC_PATH)/$(SLF4EC_LIB)
