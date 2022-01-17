CC = gcc

linux_server: linux_server.c
	$(CC) -o $@ $<
