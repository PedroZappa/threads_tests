
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;

void	*routine(void *arg)
{
	(void)arg;
	pthread_mutex_lock(&mutex);
	printf("Whirl Lock!\n");
	pthread_mutex_unlock(&mutex);
	sleep(1);	// Do something time consuming
	return (NULL);
}

int main()
{
	pthread_t th[4];
	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < 4; i++)
		if (pthread_create(&th[i], NULL, routine, NULL) != 0)
			perror("Cannot create thread");
	for (int i = 0; i < 4; i++)
		if (pthread_join(th[i], NULL) != 0)
			perror("Cannot join thread");
	return (0);
}
