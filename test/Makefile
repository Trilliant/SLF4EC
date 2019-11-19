################################################################################
#            ___________      .__.__  .__  .__               __                #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_              #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\             #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |               #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|               #
#                                                  \/     \/                   #
#                                         Networks Inc.                        #
# File:    Makefile                                                            #
#                                                                              #
# Author:  Jérémie Faucher-Goulet                                              #
#                                                                              #
#   Copyright © Trilliant Inc. 2017                                            #
#   www.trilliantinc.com                                                       #
#                                                                              #
################################################################################


################################## IMPORTANT ##################################
#
# By default this makefile search and run all test suite.
# Each test suite must be defined in a source file with a file that match the pattern testSuite_SOMETING.c (Case insensitive)
#
# The variable TEST_SUITE can be used to run only a subset of the test suite.
# For example, if you want to run the test suite defined in test/data/testSuite_Data.c and test/utils/testSuite_Utils.c the variable need to be set to "data utils" 
#TEST_SUITE:=slf4ec
# The value of TEST_SUITE can also be defined with command line argument when you call make. Ex: make all "TEST_SUITE=data utils"
#
################################################################################
################################################################################
#                                  NOTE
# This makefile must absolutely be executed from the root directory of the
# project. Without this, code coverage report will not match with actual
# location of the source files.
################################################################################
SHELL := /bin/bash -o pipefail
.DEFAULT_GOAL := all
mkfile := $(abspath $(lastword $(MAKEFILE_LIST)))
ROOT :=  $(abspath $(dir $(mkfile))..)

################################################################################
# Manual options
################################################################################

# Define to '@' if you want a silent build, define it to '' otherwise
SILENT_MODE ?= @

# Uncomment this line if you don't want the coverage report to be opened automatically 
# DISABLE_WEB_BROWSER := 1

################################################################################
# Configurations
################################################################################

LIB_NAME ?= slf4ec
TEST_SUITE_PREFIX := testSuite_
UNIT_TESTING := 1

# Make sure environment is properly configured
include $(ROOT)/tools/rules/environment.mk

TESTDIR := test
SRCDIR := src
INCLUDEDIR := include
CMOCKERY2DIR := contrib/cmockery2

BINDIR := $(abspath $(ROOT)/bin/test)

COVERAGE_HTML_DIR := $(BINDIR)/coverage_report_html

################################################################################
# Compiler options
################################################################################

CC := gcc
OBJDUMP := objdump
LDFLAGS += -pthread

# If PYTHON_HOME and GCOVR_HOME is defined, than we can extract coverage data
ifneq ($(and $(PYTHON_HOME),$(GCOVR_HOME)),)
    CFLAGS += -g3 -O0 --coverage
    LDFLAGS += -lgcov --coverage
endif
#####################################################################################################################################################################################################################################
################################################################################
# Compiler input
################################################################################
DEFINES += UNIT_TESTING UNIT_TESTING HAVE_INTTYPES_H _UINTPTR_T

################################################################################
# Test suite list
################################################################################

ifdef TEST_SUITE
    testSuiteSearchExp:=-iregex '.*$(TEST_SUITE_PREFIX)\($(subst $(space),\|,$(strip $(TEST_SUITE)))\).mk'
else
    testSuiteSearchExp:=-name '$(TEST_SUITE_PREFIX)*.mk'
endif

relative_BINDIR := $(subst $(ROOT)/,,$(BINDIR))

# Get all the "*.mk" files matching the test suite search expression
all_suite_mk_files :=  $(abspath $(shell $(FIND_EXEC) $(ROOT)/$(TESTDIR) $(testSuiteSearchExp)))

# Get the file name only first. Then remove the "$(TEST_SUITE_PREFIX)" at begining and the ".c" at the end
testSuiteList := $(patsubst $(TEST_SUITE_PREFIX)%.mk,%,$(notdir $(all_suite_mk_files)))

################################################################################
################################################################################
# Start of the MakeTest macro [$(1): name of testSuite]
################################################################################
################################################################################
define MakeTest
################################################################################
# Test suite path 
################################################################################
# $(1): name of testSuite from testSuiteList
suite_path := $$(abspath $$(dir $$(filter %$$(TEST_SUITE_PREFIX)$(1).mk,$$(all_suite_mk_files))))

################################################################################
# Test suite makefile 
################################################################################
suite_mk := $$(suite_path)/$$(TEST_SUITE_PREFIX)$(1).mk

include $$(suite_mk)

################################################################################
# Test suite Definitions
################################################################################
suite_manual_def := $$(strip $$(Test/Def/$(1))) 
suite_all_def := $$(suite_manual_def) $$(DEFINES)

################################################################################
# Test suite Includes
################################################################################
suite_inc_dir := $$(subst $$(ROOT)/,,$$(sort $$(abspath $$(dir $$(shell $$(FIND_EXEC) $$(suite_path) -name "*.h")))))
suite_manual_inc_dir := $$(strip $$(Test/Inc/$(1)))
suite_all_inc_dir := $$(suite_inc_dir) $$(suite_manual_inc_dir) $$(cmockery2_inc_dir)

################################################################################
# Test suite Sources
################################################################################
suite_src := $$(subst $$(ROOT)/,,$$(shell $$(FIND_EXEC) $$(suite_path) -name "*.c"))
suite_manual_src := $$(strip $$(Test/Src/$(1)))
suite_all_src := $$(suite_src) $$(suite_manual_src)

################################################################################
# Test suite Objects
################################################################################
suite_bin_dir := $$(relative_BINDIR)/$$(TEST_SUITE_PREFIX)$(1)
suite_manual_obj := $$(strip $$(Test/Obj/$(1)))
suite_obj := $$(call get_obj_from_src,$$(suite_all_src))
suite_all_obj := $$(suite_obj) $$(suite_manual_obj) $$(cmockery2_obj)

################################################################################
# Test suite executable
################################################################################
suite_exec_dir := $$(relative_BINDIR)/$$(TEST_SUITE_PREFIX)$(1)
suite_exec_name := $$(TEST_SUITE_PREFIX)$(1).$$(HOST_BINARY_EXT)
suite_exec := $$(suite_exec_dir)/$$(suite_exec_name)

# This global variable is augmented with the current suite exec
# The evaluation (eval) ensure the all_exec value is updated after each eval/call
# of the MakeTest macro. This is because $$(suite_exec) is updated after each call
# of the macro.
$$(eval all_exec += $$(ROOT)/$$(suite_exec))

################################################################################
# Test suite auto genereated dependencies
################################################################################
# Pull in dependency info from previously generated *.d files
-include $$(patsubst %.o,$$(ROOT)/%.d,$$(suite_all_obj))

################################################################################
# Test suite build rules
################################################################################
$$(foreach src, $$(suite_all_src), $$(eval $$(call Build/Rule,$$(src),$(ROOT)/$$(suite_exec_dir))))

$$(eval $$(call Build/Dir,$$(sort $$(dir $$(suite_obj)))))

###############################################################################
# Test suite link rules
################################################################################
.PHONY: $(1)
$(1): pre_$(1) $$(ROOT)/$$(suite_exec)

.PHONY: pre_$(1)
pre_$(1)::
	@echo
	@echo "Building test suite [$(1)]"
	
$$(eval $$(call Build/Link,$$(suite_exec)))

$$(eval $$(call Build/Dir,$$(suite_exec_dir)))

endef
################################################################################
################################################################################
# End of the MakeTest macro
################################################################################
################################################################################

################################################################################
# Recipes
################################################################################

################################################################################
# Build Rule
# $(1): source file name to build (relative path)
# $(2): Path where the test suite is being built
################################################################################
define Build/Rule
$(ROOT)/$(call get_obj_from_src,$(1)): $(ROOT)/$(1) $(suite_mk) $(mkfile) | $(dir $(call get_obj_from_src,$(1)))
	@echo "Compiling [$$<]"
	$(SILENT_MODE) $(CCACHE) $(CC) $(CFLAGS) $(addprefix -D,$(suite_all_def)) $(addprefix -I$(ROOT)/,$(suite_all_inc_dir)) -MD -MP -MF "$$(@:%.o=%.d)" -MT"$$(@:%.o=%.d)" -MT"$$(@:%.o=%.o)" -o "$$@" "$$<" \
		2>&1 | tee -a "$(2)/gcc-compile.err"
endef

################################################################################
# Link Builder [$(1): name of testSuite]
################################################################################
define Build/Link 
$(ROOT)/$(1): $(addprefix $(ROOT)/,$(suite_all_obj)) | $(suite_exec_dir)
	@echo "Linking [$$@]"
	$(SILENT_MODE) $(CC) $(LDFLAGS) -Wl,-Map="$$(patsubst %.$(HOST_BINARY_EXT),%.map,$$@)" -o "$$@" \
	$(addprefix $(ROOT)/,$(suite_obj)) \
    -Wl$(comma)--start-group \
	$(addprefix $(ROOT)/,$(suite_manual_obj)) \
    -Wl$(comma)--end-group \
   	$(addprefix $(ROOT)/,$(cmockery2_obj)) \
	2>&1 | tee "$$(@D)/gcc-link.err"
	$(SILENT_MODE) $(OBJDUMP) -h -S "$$@" > "$$(patsubst %.$(HOST_BINARY_EXT),%.lss,$$@)"
endef

################################################################################
# Directory Builder [$(1): a list of directory to create (relative path)]
################################################################################
define Build/Dir
$(1):
ifneq (@,$(SILENT_MODE))
	@echo "Creating directory [$$@]"
endif
	$(SILENT_MODE) mkdir -p "$(ROOT)/$$@"	
endef

# Helper macro [$(1): a list of source file name (relative path)]
define get_obj_from_src
$(patsubst %.c,$(suite_bin_dir)/obj/%.o,$(1))
endef

################################################################################
# cmockery2 Build rules
################################################################################
cmockery2_inc_dir := $(CMOCKERY2DIR)/src $(CMOCKERY2DIR)/src/cmockery
cmockery2_src := $(CMOCKERY2DIR)/src/cmockery.c
cmockery2_obj := $(addprefix $(relative_BINDIR)/obj/,$(cmockery2_src:%.c=%.o))

$(BINDIR)/obj/$(CMOCKERY2DIR)/%.o: $(ROOT)/$(CMOCKERY2DIR)/%.c $(mkfile) | $(dir $(cmockery2_obj))
	@echo "Compiling [$<]"
	$(SILENT_MODE) $(CCACHE) $(CC) $(CFLAGS) $(addprefix -D,$(DEFINES)) $(addprefix -I$(ROOT)/,$(cmockery2_inc_dir)) -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" \
		2>&1 | tee -a "$(@D)/gcc-compile.err"

$(eval $(call Build/Dir,$(sort $(dir $(cmockery2_obj)))))

################################################################################
# Run test suite rule
################################################################################

$(foreach testSuite,$(testSuiteList), $(eval $(call MakeTest,$(testSuite))))

define TestSuite/Run
	@echo
	@echo "Running test suite [$(1)]"
	$(SILENT_MODE) cd $(dir $(1)) && $(1) 2>&1
endef

###################################################################################################################################################################################################################################
################################################################################
# Targets
################################################################################

.PHONY: all
all: coverage

.PHONY: coverage
coverage: runTests
ifneq ($(and $(PYTHON_HOME),$(GCOVR_HOME)),)
	$(SILENT_MODE) rm -rf "$(COVERAGE_HTML_DIR)" && mkdir -p "$(COVERAGE_HTML_DIR)"
	@echo
	@echo "[Generating HTML coverage report]"
	$(SILENT_MODE) '$(PYTHON_HOME)/python' '$(GCOVR_HOME)/scripts/gcovr' -r $(ROOT) --exclude 'test/.*' --html --html-details -o '$(COVERAGE_HTML_DIR)/index.html'
    ifndef JENKINS_HOME
        ifneq ($(DISABLE_WEB_BROWSER),1)
			@echo "Open coverage report in browser"
			$(SILENT_MODE) $(call Launch/Web,$(COVERAGE_HTML_DIR)/index.html)
        endif
    endif
	@echo
	@echo "[Generating XML coverage report]"
	$(SILENT_MODE) '$(PYTHON_HOME)/python' '$(GCOVR_HOME)/scripts/gcovr' -r $(ROOT) --exclude 'test/.*' --xml -o '$(BINDIR)/$(LIB_NAME)_gcovr.xml'
endif

.PHONY: runTests
runTests: build
	@echo
	@echo "[Executing tests suites: $(testSuiteList)]"
	$(SILENT_MODE) $(foreach exec,$(all_exec),$(call TestSuite/Run,$(exec)))

.PHONY: build
build: $(testSuiteList)

.PHONY: clean
clean:
	@echo
	@echo "[Deleting test output]"
	$(SILENT_MODE) rm -rf $(BINDIR)
