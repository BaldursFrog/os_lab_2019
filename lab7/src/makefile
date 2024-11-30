CC = gcc
CFLAGS = -Wall -Wextra

# Список всех программ
PROGRAMS = tcpclient tcpserver udpclient udpserver

all: $(PROGRAMS)

# Правила для компиляции TCP-клиента
tcpclient: tcpclient.c
	$(CC) $(CFLAGS) -o tcpclient tcpclient.c

# Правила для компиляции TCP-сервера
tcpserver: tcpserver.c
	$(CC) $(CFLAGS) -o tcpserver tcpserver.c

# Правила для компиляции UDP-клиента
udpclient: udpclient.c
	$(CC) $(CFLAGS) -o udpclient udpclient.c

# Правила для компиляции UDP-сервера
udpserver: udpserver.c
	$(CC) $(CFLAGS) -o udpserver udpserver.c

# Правило для очистки всех скомпилированных файлов
clean:
	rm -f $(PROGRAMS)