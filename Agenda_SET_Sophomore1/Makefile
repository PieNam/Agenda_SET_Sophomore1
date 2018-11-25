CC := g++
FLAG := -std=c++11 -w
HEAD_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(HEAD_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/Date.o \
		$(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o \
		$(BUILD_DIR)/User.o $(BUILD_DIR)/AgendaService.o \
		$(BUILD_DIR)/AgendaLog.o $(BUILD_DIR)/AgendaUI.o 
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAG) $(INCLUDE) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAG) $(INCLUDE) -c -o $@ $<

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)

