# Project settings
PROJECT_NAME = almunecar
BUILD_DIR = build
LIBS = utils primitive-types math hashes digital-signature elliptic-curves # List of libraries in dependency order
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
BENCHMARKS_DIR = benchmarks

# Output directories
LIB_BUILD_DIR = $(BUILD_DIR)/lib
INCLUDE_BUILD_DIR = $(BUILD_DIR)/include/$(PROJECT_NAME)
OBJ_BUILD_DIR = $(BUILD_DIR)/obj
TESTS_BUILD_DIR = $(BUILD_DIR)/tests
BENCHMARKS_BUILD_DIR = $(BUILD_DIR)/benchmarks

# Installation directories
PREFIX = /usr/local
INSTALL_LIB_DIR = $(PREFIX)/lib
INSTALL_INCLUDE_DIR = $(PREFIX)/include/$(PROJECT_NAME)

# CLI
CLI_SRC = cli/almunecar.c 
CLI_OUTPUT = $(BUILD_DIR)/$(PROJECT_NAME)
CLI_OUTPUT_INSTALL = $(HOME)/.local/bin/$(PROJECT_NAME)

# Compiler settings
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -std=c99
LDFLAGS = -shared

.PHONY: build install clean test help

help:
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build:
	@for lib in $(LIBS); do \
		$(MAKE) build_$$lib > /dev/null 2>&1; \
	done

build_%: 
	@$(MAKE) headers_$*
	@$(MAKE) $(LIB_BUILD_DIR)/lib$*.so

install: headers_install $(patsubst %, $(INSTALL_LIB_DIR)/libalmunecar_%.so, $(LIBS)) ## Build all libraries

# Build shared library for each lib into build directory
$(LIB_BUILD_DIR)/lib%.so: $(OBJ_BUILD_DIR)/% | $(LIB_BUILD_DIR) $(OBJ_BUILD_DIR)
	$(eval include libs/$*/deps.mk)
	@$(CC) $(LDFLAGS) $(wildcard $(OBJ_BUILD_DIR)/$*/*.o) -L$(LIB_BUILD_DIR) $(patsubst %, -l%, $(BUILD_DEPS)) -o $@

# Build objects of each library into build dir
$(OBJ_BUILD_DIR)/%: 
	@mkdir -p $@
	@for file in libs/$*/$(SRC_DIR)/*.c; do \
		$(CC) $(CFLAGS) -Ilibs/$*/$(INCLUDE_DIR) -I$(INCLUDE_BUILD_DIR) -c libs/$*/$(SRC_DIR)/$$(basename $$file) -o $@/$$(basename $$file .c).o; \
	done

# Build shared library for each lib into INSTALL_LIB_DIR directory
$(INSTALL_LIB_DIR)/libalmunecar_%.so: $(OBJ_BUILD_DIR)/%
	$(eval include libs/$*/deps.mk)
	@$(CC) $(LDFLAGS) $(wildcard $(OBJ_BUILD_DIR)/$*/*.o) -L$(INSTALL_LIB_DIR) $(patsubst %, -l%, $(BUILD_DEPS)) -o $@ 

# Create necessary directories
$(LIB_BUILD_DIR) $(INCLUDE_BUILD_DIR) $(OBJ_BUILD_DIR) $(TESTS_BUILD_DIR) $(BENCHMARKS_BUILD_DIR):
	@mkdir -p $@

# Copy headers to the include directory
headers: $(INCLUDE_BUILD_DIR)
	@for lib in $(LIBS); do \
		$(MAKE) headers_$$lib > /dev/null 2>&1; \
	done

headers_%: $(INCLUDE_BUILD_DIR)
	@mkdir -p $(INCLUDE_BUILD_DIR)/$*
	@cp libs/$*/$(INCLUDE_DIR)/*.h $(INCLUDE_BUILD_DIR)/$*/


# Copy headers to the include directory in /usr/local/include/almunecar
headers_install:
	@for lib in $(LIBS); do \
		install -d $(INSTALL_INCLUDE_DIR)/$$lib; \
		install -m 0644 $(INCLUDE_BUILD_DIR)/$$lib/*.h $(INSTALL_INCLUDE_DIR)/$$lib; \
	done

uninstall: ## Uninstalls library installation from your system
	@for lib in $(LIBS); do \
		rm -f $(INSTALL_LIB_DIR)/libalmunecar_$$lib.so; \
	done
	@rm -rf $(INSTALL_INCLUDE_DIR)


FAIL_FAST ?= false
test: build ## Run test for all libs. To run only the tests of a specific lib run do make_<LIB_NAME>, for example: make test_primitive-types. To fail as soon as one test fails pass `FAIL_FAST=true`.
	@for lib in $(LIBS); do \
	 $(MAKE) test_$$lib; 	\
	done

test_%: build $(TESTS_BUILD_DIR)
	@for test in libs/$*/$(TEST_DIR)/*.c; do \
		mkdir -p $(TESTS_BUILD_DIR)/$*; \
		$(eval include libs/$*/deps.mk) \
		$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -o $(TESTS_BUILD_DIR)/$*/$$(basename $$test .c) $$test -L$(LIB_BUILD_DIR) $(patsubst %, -l%, $(TESTS_DEPS)) -l$*; \
		FAIL_FAST=$(FAIL_FAST) LD_LIBRARY_PATH=$(LIB_BUILD_DIR) $(TESTS_BUILD_DIR)/$*/$$(basename $$test .c); \
		if [ $$? -ne 0 ]; then \
			exit 1; \
		fi; \
	done

benchmark: build $(BENCHMARKS_BUILD_DIR) ## Run benchmarks for all libs. To run only the benchmarks of a specific lib run do benchmark_<LIB_NAME>, for example: make benchmark_primitive-types. 
	@for lib in $(LIBS); do \
		$(MAKE) benchmark_$$lib; 	\
	done

benchmark_%: build $(BENCHMARKS_BUILD_DIR) 
	@if [ -d "libs/$*/$(BENCHMARKS_DIR)" ]; then \
		for benchmark in libs/$*/$(BENCHMARKS_DIR)/*.c; do \
			mkdir -p $(BENCHMARKS_BUILD_DIR)/$*; \
			$(eval include libs/$*/deps.mk) \
			$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -o $(BENCHMARKS_BUILD_DIR)/$*/$$(basename $$benchmark .c) $$benchmark -L$(LIB_BUILD_DIR) $(patsubst %, -l%, $(BENCHMARKS_DEPS)) -l$*; \
			FAIL_FAST=$(FAIL_FAST) LD_LIBRARY_PATH=$(LIB_BUILD_DIR) $(BENCHMARKS_BUILD_DIR)/$*/$$(basename $$benchmark .c); \
			if [ $$? -ne 0 ]; then \
				exit 1; \
			fi; \
		done \
	fi

benchmark_prettify: ## Given a file with the output of a benchmark, it runs a prettify script to show it as a markdown table
	@./scripts/prettify_benchmark.sh $(BENCHMARK_FILE)


check_fmt: ## Checks formatting and outputs the diff
	@./scripts/fmt.sh libs

fmt: ## Formats the code
	@./scripts/fmt.sh libs --fix

clean: ## Clean build file
	rm -rf $(BUILD_DIR)

__CLI__:
cli_check_fmt: ## Checks formatting for CLI code
	@./scripts/fmt.sh cli

cli_fmt: ## Formats the CLI code
	@./scripts/fmt.sh cli --fix

cli_build: build ## Build cli program
	@$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -L$(LIB_BUILD_DIR) \
		$(CLI_SRC) $(patsubst %, -l%, $(LIBS)) -o $(CLI_OUTPUT)

cli_install: cli_build ## Build cli program and install it globally in your system
	cp $(CLI_OUTPUT) $(CLI_OUTPUT_INSTALL)

cli_uninstall: ## Uninstall cli program from your system
	rm $(CLI_OUTPUT_INSTALL)
