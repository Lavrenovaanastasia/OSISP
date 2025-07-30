CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
SRC_DIR = src
BUILD_DIR = build
CONFIG_DIR = config
LOG_DIR = log
TARGET = $(BUILD_DIR)/lab4
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/daemon.cpp
HEADERS = $(SRC_DIR)/daemon.h
PID_FILE = daemon.pid
CONFIG_FILE = $(CONFIG_DIR)/signals.conf

all: $(BUILD_DIR) $(LOG_DIR) $(CONFIG_DIR) $(CONFIG_FILE) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LOG_DIR):
	mkdir -p $(LOG_DIR)

$(CONFIG_DIR):
	mkdir -p $(CONFIG_DIR)

$(CONFIG_FILE):
	echo -e "SIGHUP\nSIGTERM\nSIGUSR1" > $(CONFIG_FILE)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(LOG_DIR) $(CONFIG_DIR) $(PID_FILE)

.PHONY: all clean run