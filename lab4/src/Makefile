# Общие настройки
CC = gcc
CFLAGS = -I.
LDFLAGS = -pthread

# Цели по умолчанию
all: zombie process_memory parallel_sum

# Сборка программы zombie
zombie: zombie.c
	$(CC) $(CFLAGS) -o $@ $<

# Сборка программы process_memory
process_memory: process_memory.c
	$(CC) $(CFLAGS) -o $@ $<

# Сборка программы parallel_sum
parallel_sum: parallel_sum.c-- sum.o
	$(CC) $(CFLAGS) -o $@ $< sum.o $(LDFLAGS)

# Сборка библиотеки sum.o
sum.o: sum.c sum.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Очистка сгенерированных файлов
clean:
	rm zombie process_memory parallel_sum sum.o