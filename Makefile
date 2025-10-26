# Define the source directory and object directory
SRC_DIR = src
OBJ_DIR = bin/obj
save ?= users.json

# Define the output file
OUTPUT = bin/lib

# Define the sources and objects
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Define the compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -Iftxui/include -Iinclude -std=c++17 -g -O0 -MMD -MP
LDFLAGS = -lssl -lcrypto -Lftxui/lib -lftxui

# Define the dependencies
DEPENDENCIES = $(OBJECTS:.o=.d)

# Define the all target
all: $(OUTPUT)

# Define the output target
$(OUTPUT): $(OBJECTS)
	@echo "Linking $@"
	@$(CXX) $^ $(LDFLAGS) -o $@

# Define the object target
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MF $(patsubst %.o, %.d, $@)

# Define the clean target
clean:
	@echo "Cleaning up"
	@rm -rf bin

run: $(OUTPUT)
	@echo "Running $<"
	@./$< $(save)

# Define the include dependencies
-include $(DEPENDENCIES)
