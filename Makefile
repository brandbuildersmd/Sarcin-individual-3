CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = -lm

TARGET = print_queue_system
SOURCES = main.c print_queue.c
HEADERS = print_queue.h
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
	@echo "✓ Compilare completă: $(TARGET)"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "✓ Compilat: $@"

run: $(TARGET)
	@echo "🚀 Rulare program..."
	@./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "✓ Fișiere de compilare șterse"

help:
	@echo "Comenzi disponibile:"
	@echo "  make        - Compilare program"
	@echo "  make run    - Compilare și rulare"
	@echo "  make clean  - Ștergere fișiere compilate"
	@echo "  make help   - Afișare ajutor"

.PHONY: all run clean help
