#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "rtLoop.h"
#include "tcpServer.h"
#include "packetReading.h"

pthread_mutex_t mutex;

int main()
{
	char buf[BUF_SIZE] = {0};
	pthread_t th_1, th_2;

	pthread_mutex_init(&mutex, NULL);

	pthread_create(&th_1, NULL, tcpServer, (void *)buf);
	pthread_create(&th_2, NULL, rt_loop, (void *)buf);

	pthread_join(th_1, NULL);
	pthread_join(th_2, NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
}
