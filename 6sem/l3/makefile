CC = gcc
CFLAGS = -Wall -g
OBJS = main.o morse.o
TARGET = morse_filter

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c morse.h
	$(CC) $(CFLAGS) -c main.c

morse.o: morse.c morse.h
	$(CC) $(CFLAGS) -c morse.c

clean:
	rm -f $(TARGET) *.o test_output.txt

test: $(TARGET)
	./$(TARGET)
	@echo "Тестирование завершено. Содержимое test_output.txt:"
	@cat test_output.txt


debug-test: $(TARGET)
	@echo "=== Тест с неподдерживаемыми символами и нераспознанными кодами ==="
	@echo "Исходный текст из test_input.txt:"
	@cat test_input.txt
	@echo "--- Шифровка с фильтрацией ---"
	./$(TARGET) <<EOF
	1
	2
	EOF
	@echo "--- Расшифровка с пометками ---"
	./$(TARGET) <<EOF
	2
	2
	EOF


cli-test: $(TARGET)
	@echo "=== Тест командной строки ==="
	./$(TARGET) -e -i test_input.txt -o test_output.txt
	./$(TARGET) -d -i test_output.txt -o test_decoded.txt
	@echo "--- Исходный текст:"
	@cat test_input.txt
	@echo "--- Зашифрованный текст:"
	@cat test_output.txt
	@echo "--- Расшифрованный текст:"
	@cat test_decoded.txt
	@echo "--- Сравнение исходного и расшифрованного текста:"
	@diff -q test_input.txt test_decoded.txt && echo "✔️ Совпадают" || echo " Не совпадают"