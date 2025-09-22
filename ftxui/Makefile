#compiler defines
CXX := clang++
CXXFLAGS := -Iinclude -Isrc -std=c++17 -Wall -Wextra -Werror -pedantic -MMD -MP -DUNICODE

# archiver defines
AR := ar
ARFLAGS := rcs

# folders
SRC := src/ftxui
OBJ := obj/ftxui
DEP := dep/ftxui

# source and object files
SRCS := $(wildcard $(SRC)/*/*.cpp)
OBJS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
DEPS := $(patsubst $(OBJ)/%.o, $(DEP)/%.d, $(OBJS))

# target to get after entire operation
TARGET := lib/libftxui.a

.PHONY: all clean rebuild

all : $(TARGET)

$(TARGET) : $(OBJS)
	@echo "Creating $@"
	@mkdir -p $(dir $@)
	@$(AR) $(ARFLAGS) $@ $^

obj/%.o : src/%.cpp
	@echo "Compiling $< ..."
	@mkdir -p $(dir $@)
	@$(eval D := $(patsubst obj/%.o, dep/%.d, $@))
	@mkdir -p $(dir $(D))
	@$(CXX) $(CXXFLAGS) -c $< -o $@ -MF $(D)

-include $(DEPS)

# lib file can either be removed manually ...
clean:
	@rm -rf obj

# ... or with rebuild which also reruns the entire process of making $(TARGET)
rebuild:
	@rm -rf obj lib
	@$(MAKE) all