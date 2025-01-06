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

# Compiler settings
CC = gcc
CFLAGS = -fPIC -Wall -Wextra -std=c99
LDFLAGS = -shared

.PHONY: all build clean test help

help:
	@grep -E '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: $(patsubst %, $(LIB_BUILD_DIR)/lib%.so, $(LIBS)) headers ## Build all libraries

# Build shared library for each lib
$(LIB_BUILD_DIR)/lib%.so: libs/%/src/*.c | $(LIB_BUILD_DIR) $(OBJ_BUILD_DIR)
	@$(CC) $(CFLAGS) -Ilibs/$*/$(INCLUDE_DIR) -I$(INCLUDE_BUILD_DIR) \
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

test: build ## Run test for all libs. To run only the tests of a specific lib run do make_<LIB_NAME>, for example: make test_primitive-types.
	@for lib in $(LIBS); do \
	 $(MAKE) test_$$lib; 	\
	done

test_%: build $(TESTS_BUILD_DIR)
	@for test in libs/$*/tests/*.c; do \
		$(CC) $(CFLAGS) -I$(INCLUDE_BUILD_DIR) -L$(LIB_BUILD_DIR) -l$* -o $(BUILD_DIR)/tests/$$(basename $$test .c) $$test; \
		LD_LIBRARY_PATH=$(LIB_BUILD_DIR) $(BUILD_DIR)/tests/$$(basename $$test .c); \
	done

clean: ## Clean build file
	rm -rf $(BUILD_DIR)
