BIN_DIR := $(CURDIR)/bin
CFLAGS := -g -std=gnu11
SRC_DIR := $(CURDIR)/src

engine: $(SRC_DIR)/engine.c
	@gcc $(CFLAGS) $(SRC_DIR)/engine.c -o $(BIN_DIR)/engine
