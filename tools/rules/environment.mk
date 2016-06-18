#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    environment.mk                                                       #
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

# Disable built-in rules to help speed things up and avoid unwanted behaviors.
include $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/makeRules.mk

#################################################################################
## Basic constraints
#################################################################################
ifneq ($(OS),Windows_NT)
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        override OS := $(UNAME_S)
        export OS
    else
        $(error Unsupported OS)
    endif
endif

#################################################################################
## Environment variables setup
#################################################################################

# Force PATH to what the makefiles in this project needs
ifeq ($(OS),Windows_NT)
    NEW_PATH := $(shell echo /'$(MINGW_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin:\
                $(shell echo /'$(MSYS_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin:\
                $(shell echo /'$(JDK_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin:\
                $(shell echo /'$(PYTHON_HOME)' | sed -e 's/\\/\//g' -e 's/://'):

    ifeq ($(COMPILER),$(IAR_COMPILER))
        IAR_LICENSE_MGR_EXEC = $(shell echo /'$(IAR_LICENSE_MGR_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin/LightLicenseManager.exe
        NEW_PATH += $(shell echo /'$(IAR_LICENSE_MGR_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin:
    endif

    # Add git to PATH so sonar can call 'git blame'
    ifdef GIT_HOME
	    NEW_PATH += $(shell echo /'$(GIT_HOME)' | sed -e 's/\\/\//g' -e 's/://')/bin:
    endif

	# Properly format path by:
	# 1. Removing spaces that were added because NEW_PATH is defined over multiple lines
	# 2. Removing extra forward slashes (/) which happens when this makefile is called multiple times
	override PATH := $(subst //,/,$(subst : ,:,$(NEW_PATH)))$(PATH)
endif
export PATH
$(info Environment PATH set to: $(PATH))

# Correct PATH on Windows for variables needing it
ifeq ($(OS),Windows_NT)
    override LCOV_HOME := $(shell echo '$(LCOV_HOME)' | sed -e 's/\\/\//g' -e 's/://' -e 's/\(^[^\/].*\)/\/\1/')
endif

# Correct PATH on Windows for variables needing it
ifeq ($(OS),Windows_NT)
    override ZIP_7_HOME := $(shell echo '$(ZIP_7_HOME)' | sed -e 's/\\/\//g' -e 's/://' -e 's/\(^[^\/].*\)/\/\1/')
endif

# Define a literal string that contains only a space
space :=
space +=

# Define a literal string that contains a comma
comma := ,

#################################################################################
## Programs
#################################################################################

# Defines the file extension to use for binaries on the host platform
ifeq ($(OS),Windows_NT)
    HOST_BINARY_EXT := exe
else ifeq ($(OS),Linux)
    HOST_BINARY_EXT := bin
endif

# Correct for "find" executable
ifeq ($(OS),Windows_NT)
    FIND_EXEC = /bin/find
else ifeq ($(OS),Linux)
    FIND_EXEC = find
endif

# Executable for lauching the web browser. Usage: $(call Lauch/Web,__URL__)
ifeq ($(OS),Windows_NT)
    Launch/Web = start $(1) 2>/dev/null
else ifeq ($(OS),Linux)
    Launch/Web = xdg-open $(1) &  2>/dev/null
endif

# Correct for the proper python executable
ifeq ($(OS),Windows_NT)
    PYTHON_EXEC = python
else ifeq ($(OS),Linux)
    PYTHON_EXEC = python3
endif

# 7-zip executable
ifeq ($(OS),Windows_NT)
    ZIP7_EXEC = $(ZIP_7_HOME)/7za.exe
else ifeq ($(OS),Linux)
    ZIP7_EXEC = $(ZIP_7_HOME)/bin/7za
endif

# LCOV executables
LCOV_CMD := $(LCOV_HOME)/lcov
GENHTML_CMD := $(LCOV_HOME)/genhtml

# Doxygen executable
DOX := $(DOXYGEN_HOME)/doxygen

# Sonar executable
SONAR := $(SONAR_RUNNER_HOME)/bin/sonar-runner

# RATS executable
RATS := $(RATS_HOME)/rats

# Vera++ executable
VERA := $(VERA_HOME)/bin/vera++

# Cppcheck executable
CPPCHECK := $(CPPCHECK_HOME)/cppcheck

#################################################################################
## Dependency checks
#################################################################################

# Sanity check that, if enabled, the SILENT_MODE has the proper value which will not interfere with the compilation process
ifdef SILENT_MODE
    ifneq ($(SILENT_MODE),@)
        ifneq ($(SILENT_MODE),)
            $(error SILENT_MODE must be set to '@' or ''. Was set to "$(SILENT_MODE)")
        endif
    endif
endif

# Check for availability of GCVOR
ifndef GCOVR_HOME
    $(warning GCOVR_HOME must point to gcovr to generate XML coverage report)
endif

# Check for availability of PYTHON
ifndef PYTHON_HOME
    $(warning PYTHON_HOME must point to Python to generate XML coverage report or use clang-format)
endif

# Check for availability of Doxygen
ifndef DOXYGEN_HOME
    $(warning DOXYGEN_HOME must point to where Doxygen is installed to support generation of documentation)
endif

# Check for availability of LCOV
ifndef LCOV_HOME
    $(warning LCOV_HOME must point to where LCOV is installed to support generation of HTML coverage report)
endif

# Tools only available on Windows
ifeq ($(OS),Windows_NT)
    # Check for availability of MSYS
    ifndef MSYS_HOME
        $(warning MSYS_HOME must point to where MSYS is installed in order to support the build process)
    endif
    
    # Check for availability of MingW
    ifndef MINGW_HOME
        $(warning MINGW_HOME must point to where MingW is installed in order to support the build process)
    endif
    
    # Check for availability of cppcheck
    ifndef CPPCHECK_HOME
        $(warning CPPCHECK_HOME must point to where cppcheck is installed in order to support the cppcheck analysis done by Sonar)
    endif

    # Check for availability of RATS
    ifndef RATS_HOME
        $(warning RATS_HOME must point to where RATS is installed in order to support the RATS analysis done by Sonar)
    endif

    # Check for availability of Vera++
    ifndef VERA_HOME
        $(warning VERA_HOME must point to where Vera++ is installed in order to support the Vera++ analysis done by Sonar)
    endif
endif

# Check for availability of Java JDK (needed by SONAR)
ifndef JDK_HOME
    $(warning JDK_HOME must point to a valid java JDK (needed by Sonar, Ant and Ivy))
endif

# Check for availability of Sonar
ifndef SONAR_RUNNER_HOME
    $(warning SONAR_RUNNER_HOME must point to where sonar-runner is installed to support sonar execution)
endif
