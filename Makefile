# Project settings
PROJECT_NAME = almunecar
BUILD_DIR = build
LIBS = utils primitive-types # List of libraries in dependency order
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests

# Output directories
LIB_BUILD_DIR = $(BUILD_DIR)/lib
INCLUDE_BUILD_DIR = $(BUILD_DIR)/include/$(PROJECT_NAME)
OBJ_BUILD_DIR = $(BUILD_DIR)/obj
TESTS_BUILD_DIR = $(BUILD_DIR)/tests

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

build: $(patsubst %, $(LIB_BUILD_DIR)/lib%.so, $(LIBS)) headers ## Build all libraries

install: $(patsubst %, $(INSTALL_LIB_DIR)/libalmunecar_%.so, $(LIBS)) headers_install ## Build all libraries

# Build shared library for each lib into build directory
$(LIB_BUILD_DIR)/lib%.so: libs/%/src/*.c | $(LIB_BUILD_DIR) $(OBJ_BUILD_DIR)
	@$(CC) $(CFLAGS) -Ilibs/$*/$(INCLUDE_DIR) -I$(INCLUDE_BUILD_DIR) \
		-c $^ -o $(OBJ_BUILD_DIR)/$*_objs.o
	@$(CC) $(LDFLAGS) -o $@ $(OBJ_BUILD_DIR)/$*_objs.o

# Build shared library for each lib into /usr/local/lib directory
$(INSTALL_LIB_DIR)/libalmunecar_%.so: libs/%/src/*.c
	@$(CC) $(CFLAGS) -Ilibs/$*/$(INCLUDE_DIR) -I$(INSTALL_INCLUDE_DIR) \
		-c $^ -o $(OBJ_BUILD_DIR)/$*_objs.o
	@$(CC) $(LDFLAGS) -o $@ $(OBJ_BUILD_DIR)/$*_objs.o

# Create necessary directories
$(LIB_BUILD_DIR) $(INCLUDE_BUILD_DIR) $(OBJ_BUILD_DIR) $(TESTS_BUILD_DIR):
	@mkdir -p $@

# Copy headers to the include directory
headers: $(INCLUDE_BUILD_DIR)
	@for lib in $(LIBS); do \
		mkdir -p $(INCLUDE_BUILD_DIR)/$$lib; \
		cp libs/$$lib/$(INCLUDE_DIR)/*.h $(INCLUDE_BUILD_DIR)/$$lib/; \
	done

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
		$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -o $(TESTS_BUILD_DIR)/$*/$$(basename $$test .c) $$test -L$(LIB_BUILD_DIR) -l$*; \
		FAIL_FAST=$(FAIL_FAST) LD_LIBRARY_PATH=$(LIB_BUILD_DIR) $(TESTS_BUILD_DIR)/$*/$$(basename $$test .c); \
	done

check_fmt: ## Checks formatting and outputs the diff
	@./scripts/fmt.sh libs

fmt: ## Formats the code
	@./scripts/fmt.sh libs --fix

clean: ## Clean build file
	rm -rf $(BUILD_DIR)

__CLI__:
cli_check_fmt: ## Checks formatting for cli program
	@./scripts/fmt.sh cli

cli_fmt: ## Formats the code
	@./scripts/fmt.sh cli --fix

cli_build: build ## Build cli program
	@$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -L$(LIB_BUILD_DIR) \
		$(CLI_SRC) $(patsubst %, -l%, $(LIBS)) -o $(CLI_OUTPUT)

cli_install: cli_build ## Build cli program and install it globally in your system
	cp $(CLI_OUTPUT) $(CLI_OUTPUT_INSTALL)

cli_uninstall: ## Uninstall cli program from your system
	rm $(CLI_OUTPUT_INSTALL)
