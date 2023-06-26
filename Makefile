CC := gcc
CFLAGS := -std=c99 -g -O0 -Wall -Wextra -Wpedantic
LDFLAGS := -lm -lrt 

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
	
$(TEST_BIN)/test_%_priv: $(TEST_DIR)/test_%_priv.c | $(TEST_BIN)
	$(CC) $(CFLAGS) $^ -o $@ -I$(INC_DIR)

$(TEST_BIN)/test_%_publ: $(TEST_DIR)/test_%_publ.o $(OBJ_DIR)/%.o | $(TEST_BIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TEST_BIN):
	mkdir $(TEST_BIN)

# Build and run tests for all units
tests: CFLAGS += -Wconversion -Wshadow -Wswitch-default -Wno-unused-parameter -Wno-unused-variable -Wunused-function
tests: $(TEST_EXEC)
	@for test in $^; do \
			echo "Running: $$test"; \
			$$test; \
			echo "+------------------------------------------------------------+"; \
	done; \

# Build and test a specific module or interface
test_%: CFLAGS += -Wconversion -Wshadow -Wswitch-default -Wno-unused-parameter -Wno-unused-variable -Wunused-function
test_%: $(TEST_BIN)/test_%
	@echo "Running: $(TEST_BIN)/$@"; \
	./$(TEST_BIN)/$@

# Add a dummy file as a dependency for the docs target
.DUMMY: $(DOC_DIR)/dummy
# Update the docs target with the dummy file dependency
docs: $(DOC_DIR)/dummy

$(DOC_DIR)/dummy: 
	doxygen Doxyfile
	@touch $@

clean:
	rm -rf $(BUILD_DIR) $(DOC_DIR) $(OBJ_DIR) $(TEST_BIN)
