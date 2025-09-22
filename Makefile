# Define the source directory and object directory
SRC_DIR = src
OBJ_DIR = bin/obj

# Define the output file
OUTPUT = bin/lib.exe

# Define the sources and objects
SOURCES = $(shell C:/msys64/usr/bin/find.exe $(SRC_DIR) -name '*.cpp')
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
	@pwsh -Command "Start-Process pwsh -WindowStyle Maximized -ArgumentList '-Command','$(patsubst \c%, C:%, $(subst /,\, $(realpath $(OUTPUT))))'"

# Define the include dependencies
-include $(DEPENDENCIES)
