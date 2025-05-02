# Makefile for the Vigenere project

CC = g++
CFLAGS = -Wall -Wextra -std=c++11
OBJ = main.o vigenere.o
TARGET = vigenere

# Переменная для имени файла, по умолчанию test.txt
FILE ?= test.txt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f *.o

test: $(TARGET)
	./$(TARGET) encrypt test.txt encrypted.txt "KEY"
	./$(TARGET) decrypt encrypted.txt decrypted.txt "KEY"

encrypt: $(TARGET)
	./$(TARGET) encrypt $(FILE) encrypted.txt "KEY"

decrypt: $(TARGET)
	./$(TARGET) decrypt encrypted.txt $(FILE) "KEY"
# Позволяет переопределить FILE при вызове make
override FILE := $(shell echo $(file))