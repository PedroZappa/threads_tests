/*
 *	Threaded Sum
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_data
{
	int		n_threads;
	int		n_numbers;
	int		n_per_thread;
	int		res;
	int		*idx;
	t_mutex mtx_sum;
	t_mutex mtx_printf;
	t_mutex mtx_main;
} t_data;

void	*routine(void *arg);
int		*gen_list(int n);

int *list;

/// @brief		Multi-threaded sum of an array of numbers
/// @param argc	Number of arguments
/// @param argv	Arguments
///				- n_threads: Number of threads
///				- n_numbers: Number of numbers
/// @return		0
int main(int argc, char **argv)
{
	t_data *data;
	int g_sum;
	int i;

	if (argc != 3)
	{
		perror("Error: Invalid number of arguments");
		exit(1);
	}
	/** Initialize data **/
	data = malloc(sizeof(t_data));
	data->n_threads = atoi(argv[1]);
	pthread_t threads[data->n_threads];
	data->n_numbers = atoi(argv[2]);
	data->n_per_thread = (data->n_numbers / data->n_threads);
	list = gen_list(data->n_numbers);
	if (pthread_mutex_init(&data->mtx_sum, NULL))
	{
		perror("Error: pthread_mutex_init");
		exit(1);
	}
	if (pthread_mutex_init(&data->mtx_printf, NULL))
	{
		perror("Error: pthread_mutex_init");
		exit(1);
	}
	if (pthread_mutex_init(&data->mtx_main, NULL))
	{
		perror("Error: pthread_mutex_init");
		exit(1);
	}

	/** Create threads **/
	for (i = 0; i < data->n_threads; i++)
	{
		data->idx = malloc(sizeof(int));
		*data->idx = (i * data->n_per_thread);
		printf("main: Creating Thread %d: starts at idx %d\n", i, *data->idx);
		if (pthread_create(&threads[i], NULL, routine, data) != 0)
		{
			perror("Error: pthread_create");
			exit(1);
		}
		usleep(100);
		free(data->idx);
	}

	/** Join threads **/
	g_sum = 0;
	for (i = 0; i < data->n_threads; i++)
	{
		int *res;
		if (pthread_join(threads[i], (void **)&res) != 0)
		{
			perror("Error: pthread_join");
			exit(1);
		}
		pthread_mutex_lock(&data->mtx_main);
		g_sum += *res;
		free(res);
		pthread_mutex_unlock(&data->mtx_main);
	}
	printf("Global Sum: %d\n", g_sum);

	/** Clean up **/
	pthread_mutex_destroy(&data->mtx_sum);
	pthread_mutex_destroy(&data->mtx_printf);
	pthread_mutex_destroy(&data->mtx_main);
	free(data);
	free(list);
	return (0);
}

/// @brief		Sums numbers between two indexes
/// @param arg	Pointer to the index to start at
/// @return		Pointer to the sum (Reference)
void	*routine(void *ref)
{
	t_data	*data = (t_data *)ref;
	int		*sum = malloc(sizeof(int));
	*sum = 0;
	for (int i = 0; i < data->n_per_thread; i++)
		*sum += *(list + *data->idx + i);
	pthread_mutex_lock(&data->mtx_printf);
	printf("Thread %d: idx = %d\n", *data->idx, *data->idx);
	printf("Thread Local Sum: %d\n", *sum);
	pthread_mutex_unlock(&data->mtx_printf);
	return ((void *)sum);
}

int	*gen_list(int n)
{
	int *list = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		list[i] = (i + 1);
	return (list);
}
