CXX = g++
CC = gcc
CXXFLAGS = -std=c++20 -Iinclude -Wall -O2
CFLAGS = -Iinclude

LDFLAGS = -lglfw3 -lgdi32 -lopengl32

SRC_DIR = src
OBJ_DIR = obj

CXX_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) \
       $(C_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = engine.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean