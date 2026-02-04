CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
TARGET = process_exec
SRC = process_exec.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
