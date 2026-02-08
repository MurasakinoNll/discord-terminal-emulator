CC      := gcc
CFLAGS  := -g -O3 -Wextra -std=c17 -lreadline -Isrcs/headers
SRC     := srcs
BIN     := bins
ASM     := asms

TARGET  := dumbshell

SRCS    := $(wildcard $(SRC)/*.c)
OBJS    := $(SRCS:$(SRC)/%.c=%.o)
ASMS    := $(SRCS:$(SRC)/%.c=$(ASM)/%.s)

.PHONY: all clean asm

all: $(BIN)/$(TARGET)

# Final link step
$(BIN)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

# Compile .c -> .o
%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Generate assembly
asm: $(ASMS)

$(ASM)/%.s: $(SRC)/%.c
	@mkdir -p $(ASM)
	$(CC) $(CFLAGS) -S $< -o $@

clean:
	$(RM) *.o
	$(RM) -r $(BIN)
	$(RM) -r $(ASM)
	$(RM) core*

