CC := gcc

CFLAGS := -std=c99 -g -Wall -Wextra -Wpedantic

TESTFLAGS := \
	-ggdb3 -Wconversion -Wshadow \
	-Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
	-fsanitize=address,undefined,leak
TESTLDFLAGS := -lasan -lm -lrt 

TARGET_EXEC := main 

BUILD_DIR := ./bin
DOC_DIR := ./docs/html
INC_DIR := ./include
OBJ_DIR := ./obj
SRC_DIR := ./src
TEST_DIR := ./tests
TEST_BIN := ./tests/bin

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.c)
TEST_EXEC := $(patsubst $(TEST_DIR)/%.c, $(TEST_BIN)/%, $(TEST_SRCS))

.PHONY: all tests clean

all: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
	
$(TEST_BIN)/test_%_priv: $(TEST_DIR)/test_%_priv.c $(SRC_DIR)/%.c | $(TEST_BIN)
	@$(CC) $(CFLAGS) -I$(INC_DIR) $< -o $@ 

$(TEST_BIN)/test_%_publ: $(TEST_DIR)/test_%_publ.c $(SRC_DIR)/%.c | $(TEST_BIN)
	@$(CC) $(CFLAGS) -I$(INC_DIR) $^ -o $@ 

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(TEST_BIN):
	@mkdir $(TEST_BIN)

# Tests all units, output shown only in case of failure. (No news is good news.)
tests: CFLAGS += $(TESTLDFLAGS)
tests: CFLAGS += $(TESTFLAGS)
tests: $(TEST_EXEC)
	@for test in $^; do \
		./$$test > /dev/null 2>&1; \
		exit_code=$$?; \
		if [ ! $$exit_code -eq 0 ]; then \
			echo "\nFail: $$test\nExit code: $$exit_code\n"; \
			exit 1; \
		fi; \
	done; \

# Test a specific module or interface showing detailed output.
test_%: CFLAGS += $(TESTLDFLAGS) 
test_%: CFLAGS += $(TESTFLAGS) 
test_%: $(TEST_BIN)/test_%
	./$(TEST_BIN)/$@ 

# Add a dummy file as a dependency for the docs target
.DUMMY: $(DOC_DIR)/dummy

# Update the docs target with the dummy file dependency
docs: $(DOC_DIR)/dummy

$(DOC_DIR)/dummy: 
	@doxygen Doxyfile
	@touch $@

clean:
	@rm -rf $(BUILD_DIR) $(DOC_DIR) $(OBJ_DIR) $(TEST_BIN)
