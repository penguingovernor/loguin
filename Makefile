EXE := $(shell basename $(PWD))

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
DEP_DIR := dep

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP := $(SRC:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

CPPFLAGS := -I$(INC_DIR)
CFLAGS   := -Wall -Werror -Wextra -Wpedantic -Wshadow
LDFLAGS  :=
LDLIBS   :=

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MF $(DEP_DIR)/$*.d -c $< -o $@

$(OBJ_DIR):
	mkdir $@

$(DEP_DIR):
	mkdir $@

clean:
	rm -rf $(OBJ) $(DEP) $(EXE)
