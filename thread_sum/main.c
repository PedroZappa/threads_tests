/*
 *	Threaded Sum
 * */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_data
{
	int n_threads;
	int n_numbers;
	int n_per_thread;
	int *idx;
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
	data = malloc(sizeof(t_data));
	data->n_threads = atoi(argv[1]);
	pthread_t threads[data->n_threads];
	data->n_numbers = atoi(argv[2]);
	data->n_per_thread = (data->n_numbers / data->n_threads);
	list = gen_list(data->n_numbers);
	for (i = 0; i < data->n_threads; i++)
	{
		data->idx = malloc(sizeof(int));
		*data->idx = (i * data->n_per_thread);
		if (pthread_create(&threads[i], NULL, routine, (void *)data) != 0)
		{
			perror("Error: pthread_create");
			exit(1);
		}
	}
	g_sum = 0;
	for (i = 0; i < data->n_threads; i++)
	{
		t_data *res;
		if (pthread_join(threads[i], (void **)&res) != 0)
		{
			perror("Error: pthread_join");
			exit(1);
		}
		g_sum += *res->idx;
		free(res->idx);
	}
	printf("Global Sum: %d\n", g_sum);
	free(data->idx);
	free(data);
	free(list);
	return (0);
}

/// @brief		Sums numbers between two indexes
/// @param arg	Pointer to the index to start at
/// @return		Pointer to the sum (Reference)
void	*routine(void *ref)
{
	t_data *data;
	int sum = 0;
	data = (t_data *)ref;
	for (int i = 0; i < data->n_per_thread; i++)
		sum += *(list + *data->idx + i);
	printf("Thread Local Sum: %d\n", sum);
	data->idx = &sum;
	return (ref);
}

int	*gen_list(int n)
{
	int *list = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		list[i] = (i + 1);
	return (list);
}
