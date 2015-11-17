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

# Needed so the compiler will stop on error
SHELL := /bin/bash -o pipefail

# Define to '@' if you want a silent build, define it to '' otherwise
SILENT_MODE ?= @

#################################################################################
# Paths and project structure
#################################################################################

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT := $(abspath $(dir $(mkfile_path)))

CONTRIB_HOME := $(abspath $(ROOT)/contrib)
CMOCKERY2_HOME := $(abspath $(CONTRIB_HOME)/cmockery2)
DOCDIR_REL := docs
DOCDIR := $(abspath $(ROOT)/$(DOCDIR_REL))
EXAMPLEDIR_REL := example
EXAMPLEDIR := $(abspath $(ROOT)/$(EXAMPLEDIR_REL))
INCLUDEDIR_REL := include
INCLUDEDIR := $(abspath $(ROOT)/$(INCLUDEDIR_REL))
SRCDIR_REL := src
SRCDIR := $(abspath $(ROOT)/$(SRCDIR_REL))
TESTDIR_REL := test
TESTDIR := $(abspath $(ROOT)/$(TESTDIR_REL))
TOOLDIR_REL := tools
TOOLDIR := $(abspath $(ROOT)/$(TOOLDIR_REL))
CLANG_HOME := $(TOOLDIR)/clang-format

#################################################################################
# Configurations
#################################################################################

PROJECT_NAME ?= SLF4EC
PROJECT_VERSION ?= $(shell (git describe --long | sed -ne 's/^\(.*\)-\([0-9]\{1,4\}\)-g\([a-f0-9]\{1,40\}\)$$/\1+\2.\3/p'))
DATE_NOW_UTC := $(shell date -u "+%Y-%m-%d %H:%M:%S")
HOST := $(shell hostname)
BRANCH ?= $(HOST)

BINDIR_REL ?= bin/x86
BINDIR := $(abspath $(ROOT)/$(BINDIR_REL)/$(COMPILER))

ifdef DEBUG
    BINDIR := $(abspath $(BINDIR)/debug)
else
    BINDIR := $(abspath $(BINDIR)/release)
endif

OBJDIR_SRC := $(abspath $(BINDIR)/obj/src)
OBJDIR_TEST := $(abspath $(BINDIR)/obj/test)

OUTPUT_LIB := $(BINDIR)/$(PROJECT_NAME).a

# If on Windows, edit PATH to have MSYS and MINGW if they exists
ifeq ($(findstring Windows,$(OS)),Windows)
    ifneq ($(and $(MSYS_HOME),$(MINGW_HOME)),)
        override PATH := $(shell (echo "/$(MSYS_HOME)\bin" | sed 's/\\/\//g' | sed 's/://')):$(shell (echo "/$(MINGW_HOME)\bin" | sed 's/\\/\//g' | sed 's/://')):$(PATH)
    endif
endif

AR := ar
CC := gcc
DOX := "$(DOXYGEN_HOME)/doxygen"
CPPCHECK := "$(CPPCHECK_HOME)/cppcheck"
RATS := "$(RATS_HOME)/rats"
VERA := "$(VERA_HOME)/bin/vera++"
SONAR := $(SONAR_RUNNER_HOME)/bin/sonar-runner
CLANG_EXEC := clang-format
ifeq ($(findstring Windows,$(OS)),Windows)
CLANG_EXEC = clang-format.exe
endif

#################################################################################
# Compiler options
#################################################################################

ifeq ($(COMPILER), iar)
else
    CFLAGS += -Wall -Wextra -fdiagnostics-show-option
    ifdef DEBUG
        CFLAGS += -g3 -O0
    endif
endif

TEST_DEFINES += -DUNIT_TESTING

# If PYTHON_HOME and GCOVR_HOME is defined, then we can extract test coverage data
ifneq ($(and $(PYTHON_HOME),$(GCOVR_HOME)),)
    TEST_CFLAGS += -O0 --coverage
    TEST_LDFLAGS += -lgcov --coverage
endif

DEFINES += -DHAVE_INTTYPES_H -D_UINTPTR_T -DCOMPILED_LOG_LEVEL=LEVEL_MAX -DUSE_LOCATION_INFO

TEST_CFLAGS += $(CFLAGS)
TEST_LDFLAGS += $(LDFLAGS)
TEST_DEFINES += $(DEFINES)

INCLUDES += \
	-I"$(CMOCKERY2_HOME)/src" \
	-I"$(CMOCKERY2_HOME)/src/cmockery" \

INCLUDES += \
	-I"$(EXAMPLEDIR)" \
	-I"$(INCLUDEDIR)" \
	-I"$(INCLUDEDIR)/logger" \
	-I"$(TESTDIR)" \
	-I"$(TESTDIR)/logger" \

#################################################################################
# File list
#################################################################################

libSrc := \
	slf4ec.c \
	logger/stdout.c \

libTest := \
	test.c \
	testLog.c \
	logger/testStdout.c \

libSrc_obj := $(addprefix $(OBJDIR_SRC)/, $(libSrc:%.c=%.o))
libTest_obj := $(addprefix $(OBJDIR_TEST)/, $(libSrc:%.c=%.o))
libTest_obj += $(addprefix $(OBJDIR_TEST)/, $(libTest:%.c=%.o))

app := \
	logConfig.c \
	main.c \

app_obj := $(addprefix $(OBJDIR_SRC)/, $(app:%.c=%.o))

framework_src := \
	$(CMOCKERY2_HOME)/src/cmockery.c \

framework_obj := \
	$(OBJDIR_TEST)/cmockery2/cmockery.o \

test_obj := \
	$(framework_obj) \
	$(libTest_obj) \

allSrc := $(addprefix $(SRCDIR_REL)/, $(libSrc))
allSrc += $(addprefix $(EXAMPLEDIR_REL)/, $(app))

#################################################################################
# Rules
#################################################################################

ifeq ($(COMPILER), iar)
$(OBJDIR_SRC)/%.o: $(SRCDIR)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) "$<" $(CFLAGS) $(DEFINES) $(INCLUDES) --dependencies=m $(@:%.o=%.d) -o $@ 2>&1 | tee -a "$(BINDIR)/iar-compile.err"

$(OBJDIR_SRC)/%.o: $(EXAMPLEDIR)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) "$<" $(CFLAGS) $(DEFINES) $(INCLUDES) --dependencies=m $(@:%.o=%.d) -o $@ 2>&1 | tee -a "$(BINDIR)/iar-compile.err"
else
$(OBJDIR_SRC)/%.o: $(SRCDIR)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<" 2>&1 | tee -a "$(BINDIR)/gcc-compile.err"

$(OBJDIR_SRC)/%.o: $(EXAMPLEDIR)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<" 2>&1 | tee -a "$(BINDIR)/gcc-compile.err"
endif
$(OBJDIR_TEST)/cmockery2/cmockery.o: $(CMOCKERY2_HOME)/src/cmockery.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) $(TEST_CFLAGS) $(TEST_DEFINES) $(INCLUDES) -c  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<"

# The rule must absolutely use a relative path because of gcov output
$(OBJDIR_TEST)/%.o: $(TESTDIR_REL)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) $(TEST_CFLAGS) $(TEST_DEFINES) $(INCLUDES) -c  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<" 2>&1 | tee -a "$(BINDIR)/gcc-compile.err"

# The rule must absolutely use a relative path because of gcov output
$(OBJDIR_TEST)/%.o: $(SRCDIR_REL)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) $(TEST_CFLAGS) $(TEST_DEFINES) $(INCLUDES) -c  -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -o "$@" "$<" 2>&1 | tee -a "$(BINDIR)/gcc-compile.err"

#################################################################################
# Targets
#################################################################################

# Pull in dependency info from previously generated *.d files
-include $(libSrc_obj:.o=.d)
-include $(test_obj:.o=.d)
-include $(app_obj:.o=.d)

.PHONY: all
all: build link


.PHONY: link
link: $(app_obj)
	@echo
	@echo "[Linking $(PROJECT_NAME)'s example]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) -o "$(BINDIR)/example.exe" $(LDFLAGS) $^ "$(OUTPUT_LIB)" 2>&1 | tee -a "$(BINDIR)/gcc-link.err"
	@echo

.PHONY: build
build: $(libSrc_obj)
	@echo
	@echo "[Creating $(PROJECT_NAME) library]"
	$(SILENT_MODE) cd $(ROOT) && $(AR) rcs "$(OUTPUT_LIB)" $(libSrc_obj)
	@echo

.PHONY: clean
clean:
	@echo "[Deleting test output]"
	$(SILENT_MODE) rm -rf $(ROOT)/bin
	$(SILENT_MODE) rm -rf $(DOCDIR)/doxygen
	@echo

.PHONY: build_test
build_test: $(test_obj)
	@echo
	@echo "[Linking $(PROJECT_NAME)'s unit tests]"
	$(SILENT_MODE) cd $(ROOT) && $(CC) -o $(BINDIR)/$(PROJECT_NAME).exe $(TEST_LDFLAGS) $^ 2>&1 | tee -a "$(BINDIR)/gcc-link.err"
	@echo

.PHONY: run
run:
	@echo "[Executing tests]"
	$(SILENT_MODE) cd $(BINDIR) && "$(BINDIR)/$(PROJECT_NAME).exe"
# If PYTHON_HOME and GCOVR_HOME is defined, than we can extract coverage data
ifneq ($(and $(PYTHON_HOME),$(GCOVR_HOME)),)
	@echo
	@echo "[Generating coverage report]"
	$(SILENT_MODE) cd $(ROOT) && '$(PYTHON_HOME)/python' '$(GCOVR_HOME)/scripts/gcovr' -r . --xml -o '$(BINDIR)/$(PROJECT_NAME)_gcovr.xml'
endif
	@echo

.PHONY: test
test: build build_test run

.PHONY: format
format:
ifdef PYTHON_HOME
	@echo "[Formatting source code]"
	$(SILENT_MODE) cd $(ROOT) && "$(PYTHON_HOME)/python" "$(CLANG_HOME)/formatAll.py" -d "$(SRCDIR_REL),$(EXAMPLEDIR_REL),$(TESTDIR_REL),$(INCLUDEDIR_REL)" -b "$(CLANG_HOME)/$(CLANG_EXEC)"
else
	@echo "ERROR: Cannot execute format without PYTHON_HOME"
endif
	@echo

.PHONY: doc
doc:
ifdef DOXYGEN_HOME
	@echo "[Generating documentation]"
	mkdir -p "$(ROOT)/docs/doxygen"
	( cd $(ROOT) ; cat Doxyfile ; echo "PROJECT_NUMBER=$(PROJECT_VERSION)" ) | ( cd $(ROOT) ; $(DOX) - )
else
	@echo "ERROR: Cannot generate documentation without doxygen"
endif
	@echo

.PHONY: cppcheck
cppcheck:
ifdef CPPCHECK_HOME
	@echo "[Executing cppcheck]"
	cd $(ROOT) && $(CPPCHECK) -v --enable=all --xml-version=2 $(DEFINES) $(INCLUDES) $(SRCDIR_REL) 2> "$(BINDIR)/$(PROJECT_NAME)-cppcheck.xml"
else
	@echo "Error: Cannot execute cppcheck, skipping..."
endif
	@echo

.PHONY: rats
rats:
ifdef RATS_HOME
	@echo "[Executing RATS]"
	cd $(ROOT) && $(RATS) -w 3 --xml $(allSrc) > "$(BINDIR)/$(PROJECT_NAME)-rats.xml"
else
	@echo "Error: Cannot execute RATS, skipping..."
endif
	@echo

.PHONY: vera
vera:
ifdef VERA_HOME
	@echo "[Executing Vera++]"
	cd $(ROOT) && $(VERA) -s -p trilliant -c "$(BINDIR)/$(PROJECT_NAME)-vera.xml" $(allSrc)
else
	@echo "Error: Cannot execute Vera++, skipping..."
endif
	@echo

.PHONY: sonar
sonar: test vera rats cppcheck
ifdef SONAR_RUNNER_HOME
	@echo "[Executing sonar analysis]"
	cd $(ROOT) && "$(SONAR)" -Dsonar.projectVersion="$(PROJECT_VERSION)" -Dsonar.branch=$(BRANCH)
else
	@echo "ERROR: Cannot execute sonar"
endif
	@echo

#################################################################################
## Makefile dependency checks
#################################################################################

# If compiling on Windows, check for availability of MSYS and MINGW
ifeq ($(findstring Windows,$(OS)),Windows)
    ifndef MSYS_HOME
        $(warning MSYS_HOME must point to the path where MSYS is available)
    endif

    ifndef MINGW_HOME
        $(warning MINGW_HOME must point to the path where MINGW is available)
    endif
endif

# Check for Doxygen availability
ifndef DOXYGEN_HOME
$(warning DOXYGEN_HOME must point to where Doxygen is installed to support generation of documentation)
endif

# Check for cppcheck
ifndef CPPCHECK_HOME
$(warning CPPCHECK_HOME must point to where cppcheck is installed in order to support the cppcheck analysis done by Sonar)
endif

# Check for RATS
ifndef RATS_HOME
$(warning RATS_HOME must point to where RATS is installed in order to support the RATS analysis done by Sonar)
endif

# Check for Vera++
ifndef VERA_HOME
$(warning VERA_HOME must point to where Vera++ is installed in order to support the Vera++ analysis done by Sonar)
endif

# Check for Sonar
ifndef SONAR_RUNNER_HOME
$(warning SONAR_RUNNER_HOME must point to where sonar-runner is installed to support sonar execution)
endif