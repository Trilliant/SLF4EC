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
SHELL := /bin/bash -o pipefail
.DEFAULT_GOAL := all
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT := $(abspath $(dir $(mkfile_path)))

#################################################################################
# Manual options
#################################################################################

# Define to '@' if you want a silent build, define it to '' otherwise
export SILENT_MODE ?= @

# Uncomment this line if you don't want your default web browser to be launched
# DISABLE_WEB_BROWSER := 1

#################################################################################
# Paths and project structure
#################################################################################
TEST_DIR := test
ARTIFACT_DIR := artifacts
EXAMPLE_DIR := example
CONTRIB_DIR := contrib
DOC_DIR := docs
TOOL_DIR := tools
CLANG_DIR := $(TOOL_DIR)/clang-format
CMOCKERY2_DIR := $(CONTRIB_DIR)/cmockery2

#################################################################################
# Configuration
#################################################################################

# Setup current environment
include $(ROOT)/tools/rules/environment.mk

SLF4EC_VERSION ?= $(shell cd $(ROOT) && (git describe --long | sed -ne 's/^\(.*\)-\([0-9]\{1,4\}\)-g\([a-f0-9]\{1,40\}\)$$/\1+\2.\3/p'))
SLF4EC_BRANCH ?= $(shell cd $(ROOT) && (git symbolic-ref --short -q HEAD))
DATE_NOW_UTC := $(shell date -u "+%Y-%m-%d %H:%M:%S")
HOST := $(shell hostname)

# Correct BRANCH for SONAR which doesn't accept the '/' character in a branch name
# "feature/some_feature" becomes "feature_some_feature" 
SONAR_BRANCH := $(shell echo $(SLF4EC_BRANCH) | sed -e 's/\//_/g')

#################################################################################
# Build config
#################################################################################

# Default achitecture is x86
ifdef ARCH
    SLF4EC_ARCH			?= ${ARCH}
endif
SLF4EC_ARCH				?= x86

# Default compiler is GCC
ifdef COMPILER
    SLF4EC_COMPILER		?= ${COMPILER}
endif

#################################################################################
# Compiler config
#################################################################################
include $(ROOT)/$(TOOL_DIR)/rules/makeRules.mk
include $(ROOT)/slf4ec.mk
ifneq ($(ARCH),)
    include $(ROOT)/$(SLF4EC_ARCH).mk
endif

SLF4EC_PACKAGE := $(ROOT)/$(SLF4EC_BINDIR)/$(SLF4EC_ARCH)/$(SLF4EC_COMPILER)/$(SLF4EC_USELOC)/$(SLF4EC_NAME).zip

ifeq ($(SLF4EC_ARCH),x86)
    AR := ar
    CC := gcc
endif

# If PYTHON_HOME and GCOVR_HOME is defined, or LCOV is available, then we can extract coverage data
ifneq ($(or $(and $(PYTHON_HOME),$(GCOVR_HOME)),$(LCOV_HOME)),)
    TEST_CFLAGS += -O0 --coverage
    TEST_LDFLAGS += -lgcov --coverage
endif

#################################################################################
# Compiler input
#################################################################################

libSrc := \
	$(subst $(ROOT)/,,$(shell $(FIND_EXEC) $(ROOT)/$(SLF4EC_SRCDIR) -name "*.c"))

testSrc := \
	$(subst $(ROOT)/,,$(shell $(FIND_EXEC) $(ROOT)/$(TEST_DIR) -name "*.c")) \
	$(CMOCKERY2_DIR)/src/cmockery.c \
	$(libSrc)

expSrc := \
	$(subst $(ROOT)/,,$(shell $(FIND_EXEC) $(ROOT)/$(EXAMPLE_DIR) -name "*.c"))

libInc := \
	$(subst $(ROOT)/,,$(sort $(abspath $(dir $(shell $(FIND_EXEC) $(ROOT)/$(SLF4EC_INCDIR) -name "*.h")))))

testInc := \
	$(subst $(ROOT)/,,$(sort $(abspath $(dir $(shell $(FIND_EXEC) $(ROOT)/$(TEST_DIR) -name "*.h"))))) \
	$(libInc) \
	$(CMOCKERY2_DIR)/src \
	$(CMOCKERY2_DIR)/src/cmockery

expInc := \
	$(libInc) \
	$(subst $(ROOT)/,,$(sort $(abspath $(dir $(shell $(FIND_EXEC) $(ROOT)/$(EXAMPLE_DIR) -name "*.h")))))

allInc := $(strip $(sort $(libInc) $(testInc) $(expInc)))

libBin	:= $(ROOT)/$(SLF4EC_PATH)
testBin	:= $(ROOT)/$(SLF4EC_BINDIR)/test
expBin	:= $(ROOT)/$(SLF4EC_BINDIR)/example

libObj	:= $(addprefix $(libBin)/obj/, $(libSrc:%.c=%.o))
testObj := $(addprefix $(testBin)/obj/, $(testSrc:%.c=%.o))
expObj	:= $(addprefix $(expBin)/obj/, $(expSrc:%.c=%.o))

COMPILED_LOG_LEVEL ?= LEVEL_MAX
libDef	:= -DCOMPILED_LOG_LEVEL=$(COMPILED_LOG_LEVEL)
ifdef USE_LOCATION_INFO
    libDef 	+= -DUSE_LOCATION_INFO
endif
testDef	:= -DUNIT_TESTING -DHAVE_INTTYPES_H -D_UINTPTR_T

#################################################################################
# Other constants
#################################################################################
LCOV_COVERAGE_HTML_DIR := $(testBin)/coverage_report_html
LCOV_COVERAGE_INFO_FILE := $(LCOV_COVERAGE_HTML_DIR)/coverage.info

#################################################################################
# Rules
#################################################################################
# Pull in dependency info from previously generated *.d files
-include $(libObj:.o=.d)
-include $(testObj:.o=.d)
-include $(expObj:.o=.d)

COMPILE_OPTS_iar = --dependencies=m $(@:%.o=%.d)
COMPILE_OPTS_gcc = -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"

# Rule to build the library
$(libBin)/obj/%.o: $(ROOT)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) $(CC) "$<" $(CFLAGS) $(libDef) $(addprefix -I$(ROOT)/,$(libInc)) $(COMPILE_OPTS_$(COMPILER)) -o "$@"  2>&1 | tee -a "$(libBin)/$(COMPILER)-compile.err"

# Rule to build the tests
$(testBin)/obj/%.o: $(ROOT)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) $(CC) "$<" $(CFLAGS) $(TEST_CFLAGS) $(testDef) $(addprefix -I$(ROOT)/,$(testInc)) $(COMPILE_OPTS_$(COMPILER)) -o "$@"  2>&1 | tee -a "$(testBin)/$(COMPILER)-compile.err"

# Rule to build the example
$(expBin)/obj/%.o: $(ROOT)/%.c $(mkfile_path)
	@[ -d "$(dir $@)" ] || mkdir -p "$(dir $@)"
	@echo "Compiling [$<]"
	$(SILENT_MODE) $(CC) "$<" $(CFLAGS) $(libDef) $(addprefix -I$(ROOT)/,$(expInc)) $(COMPILE_OPTS_$(COMPILER)) -o "$@"  2>&1 | tee -a "$(expBin)/$(COMPILER)-compile.err"

#################################################################################
# Top Targets
#################################################################################

.PHONY: all
all: build preExample example

.PHONY: build
build: preBuild $(ROOT)/$(SLF4EC_FILE)

$(ROOT)/$(SLF4EC_FILE): $(libObj)
	@echo "Generating [$@]"
	$(SILENT_MODE) $(AR) $(ARFLAGS) $@ $^

.PHONY: example
example: $(expObj)
	@echo "Linking [$(expBin)/example.$(HOST_BINARY_EXT)]"
	$(SILENT_MODE) $(CC) -o "$(expBin)/example.$(HOST_BINARY_EXT)" $(LDFLAGS) -Wl,-Map="$(expBin)/example.map" $^ "$(SLF4EC_FILE)" \
	 2>&1 | tee -a "$(expBin)/$(COMPILER)-link.err"

.PHONY: clean
clean:
	@echo
	@echo "[Deleting Output]"
	$(SILENT_MODE) rm -rf $(ROOT)/bin
	$(SILENT_MODE) rm -rf $(ROOT)/$(DOC_DIR)/doxygen
	$(SILENT_MODE) rm -rf $(ROOT)/$(ARTIFACT_DIR)

.PHONY: test
test: preBuildTest buildTest run coverage

.PHONY: buildTest
buildTest: $(testObj)
	@echo
	@echo "[Linking $(SLF4EC_NAME)'s unit tests]"
	$(SILENT_MODE) $(CC) -o $(testBin)/$(SLF4EC_NAME).$(HOST_BINARY_EXT) $(TEST_LDFLAGS) $^ 2>&1 | tee -a "$(testBin)/$(COMPILER)-link.err"

.PHONY: run
run:
	@echo
	@echo "[Executing tests]"
	$(SILENT_MODE) cd $(testBin) && $(testBin)/$(SLF4EC_NAME).$(HOST_BINARY_EXT) 2>&1

.PHONY: coverage
coverage:
# If PYTHON_HOME and GCOVR_HOME is defined, than we can extract coverage data
ifneq ($(and $(PYTHON_HOME),$(GCOVR_HOME)),)
	@echo
	@echo "[Generating XML coverage report]"
    # Used mainly for Cobertura plugin in SonarQube
	$(SILENT_MODE) '$(PYTHON_HOME)/python' '$(GCOVR_HOME)/scripts/gcovr' -r $(ROOT) --xml -o '$(testBin)/$(SLF4EC_NAME)_gcovr.xml'
endif
ifneq ($(LCOV_HOME),)
	@echo
	@echo "[Generating HTML coverage report]"
	$(SILENT_MODE) rm -rf "$(LCOV_COVERAGE_HTML_DIR)" && mkdir -p "$(LCOV_COVERAGE_HTML_DIR)"
	@echo "Generating $(LCOV_COVERAGE_INFO_FILE)"
	$(SILENT_MODE) $(LCOV_CMD) --capture --directory $(testBin) --quiet --no-compat-libtool --output-file $(LCOV_COVERAGE_INFO_FILE) 1>/dev/null 
	@echo "Generating HTML site in : $(LCOV_COVERAGE_HTML_DIR)"
	$(SILENT_MODE) $(GENHTML_CMD) $(LCOV_COVERAGE_INFO_FILE) --quiet --output-directory $(LCOV_COVERAGE_HTML_DIR)
    ifndef JENKINS_HOME
        ifneq ($(DISABLE_WEB_BROWSER),1)
			@echo "Opening coverage report in web browser..."
			$(SILENT_MODE) $(call Launch/Web,$(LCOV_COVERAGE_HTML_DIR)/index.html)
        endif
    endif
endif

#################################################################################
# Extra Targets
#################################################################################

# Files where Git should ignore changes, yet the file MUST be in the repository
.PHONY: git-assume-unchanged
git-assume-unchanged:
	$(SILENT_MODE) cd $(ROOT) && git update-index --assume-unchanged ./.settings/language.settings.xml

.PHONY: format
format:
ifdef PYTHON_HOME
	@echo
	@echo "[Formatting source code]"
	$(SILENT_MODE) cd $(ROOT) && "$(PYTHON_HOME)/$(PYTHON_EXEC)" "$(CLANG_DIR)/formatAll.py" -d "src,example,test,include" -b "$(CLANG_DIR)/clang-format"
else
	@echo "ERROR: Cannot execute format without PYTHON_HOME"
	exit 1
endif

.PHONY: doc
doc:
ifdef DOXYGEN_HOME
	@echo
	@echo "[Generating documentation]"
	mkdir -p "$(ROOT)/$(DOC_DIR)/doxygen"
	( cd $(ROOT) ; cat Doxyfile ; echo "PROJECT_NUMBER=$(SLF4EC_VERSION)" ) | ( cd $(ROOT) ; "$(DOX)" - )
    ifndef JENKINS_HOME
        ifneq ($(DISABLE_WEB_BROWSER),1)
			@echo "Opening documemtation in web browser..."
			$(SILENT_MODE) $(call Launch/Web,$(ROOT)/$(DOC_DIR)/doxygen/html/index.html)
        endif
    endif
else
	@echo "ERROR: Cannot generate documentation without doxygen"
	exit 1
endif

# Sonar analysis will need include paths from GCC
ifeq ($(COMPILER),$(GCC_COMPILER))
    # Extract GCC compiler specs
    RAW_GCC_SPECS := $(shell (export PATH="$(PATH)" && $(CC) -v -x c -E /dev/null 2>&1) )
    
    # Filter out for the section specifying default include paths
    INCLUDE_SPECS := $(strip $(shell echo '$(RAW_GCC_SPECS)' | sed -n 's/^.*search starts here:\(.*\)End of search list.*$$/\1/p' ))
    
    # Correct the extracted paths by:
    # 1. Treating them one by one (using the foreach loop).
    # 2. Correcting the path towards POSIX-style.
    # 3. Turning the path into absolute (removing stuff such as '../..').
    GCC_DEF_PATHS := $(foreach path,$(INCLUDE_SPECS),$(abspath $(shell echo '$(path)' | sed -e 's/\\/\//g' -e 's/://' -e 's/\(^[^\/].*\)/\/\1/' )))
    
    # If on windows, Sonar will need Windows-style paths
    ifeq ($(OS),Windows_NT)
        GCC_DEF_PATHS_WIN := $(foreach path,$(GCC_DEF_PATHS),$(shell echo '$(path)' | sed -e 's/^\///' -e 's/\//\\/g' -e 's/^./\0:/' ))
    else
        GCC_DEF_PATHS_WIN := $(GCC_DEF_PATHS)
    endif
endif

.PHONY: sonar
sonar: test preSonar vera rats cppcheck
ifdef SONAR_RUNNER_HOME
	@echo
	@echo "[Executing sonar analysis]"
	$(SILENT_MODE) cd $(ROOT) && "$(SONAR)" -Dsonar.projectVersion="$(SLF4EC_VERSION)" -Dsonar.branch="$(SONAR_BRANCH)" -Dsonar.artifact.path="$(ROOT)/$(SLF4EC_FILE)" \
	-Dsonar.cxx.includeDirectories="$(subst $(space),$(comma),$(allInc) $(GCC_DEF_PATHS_WIN))"
else
	@echo "ERROR: Cannot execute sonar"
	exit 1
endif

.PHONY: vera
vera:
ifdef VERA_HOME
	@echo
	@echo "[Executing Vera++]"
	$(SILENT_MODE) cd $(ROOT) && "$(VERA)" -s -p trilliant -c "$(testBin)/$(SLF4EC_NAME)-vera.xml" $(libSrc)
else
	@echo "Error: Cannot execute Vera++, skipping..."
endif

.PHONY: rats
rats:
ifdef RATS_HOME
	@echo
	@echo "[Executing RATS]"
    # We need to replace the CRLF by LF (\r\n by \n) on Windows
	@sed -i 's/\r\n/\n/' "$(RATS_HOME)/rats-c.xml"
	@sed -i 's/\r\n/\n/' "$(RATS_HOME)/rats-openssl.xml"
	$(SILENT_MODE) cd $(ROOT) && "$(RATS)" -d "$(RATS_HOME)/rats-c.xml" -d "$(RATS_HOME)/rats-openssl.xml" -w 3 --xml $(libSrc) > "$(testBin)/$(SLF4EC_NAME)-rats.xml"
else
	@echo "Error: Cannot execute RATS, skipping..."
endif

.PHONY: cppcheck
cppcheck:
ifdef CPPCHECK_HOME
	@echo
	@echo "[Executing cppcheck]"
	$(SILENT_MODE) cd $(ROOT) && "$(CPPCHECK)" -v --enable=all --xml-version=2 $(libDef) $(addprefix -I,$(GCC_DEF_PATHS)) $(addprefix -I$(ROOT)/,$(libInc)) $(libSrc) 2> "$(testBin)/$(SLF4EC_NAME)-cppcheck.xml"
else
	@echo "Error: Cannot execute cppcheck, skipping..."
endif

.PHONY: package
package: build
	@echo
	@echo "Packaging [$(SLF4EC_PACKAGE)]"
	$(SILENT_MODE) cd $(SLF4EC_PATH) && $(ZIP7_EXEC) a $(SLF4EC_PACKAGE) $(SLF4EC_NAME).a
	$(SILENT_MODE) cd $(ROOT) && $(ZIP7_EXEC) a -r $(SLF4EC_PACKAGE) $(ROOT)/$(SLF4EC_INCDIR)/
	$(SILENT_MODE) cd $(ROOT) && $(ZIP7_EXEC) a $(SLF4EC_PACKAGE) $(SLF4EC_NAME).mk

.PHONY: artifacts
artifacts: doc sonar
	@rm -rf $(ROOT)/$(ARTIFACT_DIR)
	@mkdir -p $(ROOT)/$(ARTIFACT_DIR)
	$(SILENT_MODE) cd $(ROOT)/$(DOC_DIR) && $(ZIP7_EXEC) a -r  $(ROOT)/$(ARTIFACT_DIR)/doc.zip $(ROOT)/$(DOC_DIR)/* -x!.keep
	$(SILENT_MODE) cd $(testBin) && $(ZIP7_EXEC) a -r  $(ROOT)/$(ARTIFACT_DIR)/tests.zip $(testBin)/* -xr!obj

#################################################################################
# Pre-Targets
#################################################################################

.PHONY: preBuild
preBuild:
	@echo
	@echo [Building $(SLF4EC_NAME)]

.PHONY: preBuildTest
preBuildTest:
	@echo
	@echo "[Building $(SLF4EC_NAME)'s unit tests]"

.PHONY: preExample
preExample: 
	@echo
	@echo "[Building example]"
	$(SILENT_MODE) rm -rf $(expBin)

.PHONY: preSonar
preSonar: 
	@echo
	@echo "[Running SONAR analysis]"
