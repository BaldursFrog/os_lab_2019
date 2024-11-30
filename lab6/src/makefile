# Общие переменные
CC = gcc
CFLAGS = -Wall -Wextra -pthread

# Исходные файлы
SERVER_SRC = server2.c common.c
CLIENT_SRC = client2.c common.c

# Цели для компиляции
server: $(SERVER_SRC)
	$(CC) $(CFLAGS) -o server $(SERVER_SRC)

client: $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o client $(CLIENT_SRC)

# Цель для очистки
clean:
	rm -f server client

# Цель по умолчанию
all: server client

# Цель для запуска сервера и клиента
run-server: server
	./server --port 20001 --tnum 4

run-client: client
	./client --k 4 --mod 7 --servers servers.txt

.PHONY: all clean run-server run-client