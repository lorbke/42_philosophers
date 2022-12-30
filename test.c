#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct t_info
{
	__darwin_useconds_t	starve_time;
	__darwin_useconds_t	eat_time;
	__darwin_useconds_t	sleep_time;
	__darwin_useconds_t	eat_count;
}	s_info;

typedef struct t_philo
{
	int				num;
	s_info			*info;
	pthread_t		thread;
	pthread_mutex_t	fork_r;
	pthread_mutex_t	*fork_l;
}	s_philo;

void	*philo_routine(void *arg)
{
	s_philo				*philo;
	struct timeval		time;
	__darwin_useconds_t	timestamp;
	int					i;

	philo = (s_philo *)arg;
	i = 0;
	while (i < philo->info->eat_count)
	{
		gettimeofday(&time, NULL);
		timestamp = time.tv_usec;
		// if mutex available
		// print is eating then usleep
		// print is sleeping then usleep
		// else
		printf(" %d %d is thinking\n", timestamp, philo->num);
		i++;
	}
	return NULL;
}

int	main(int argc, char **argv)
{
	s_info	info;
	s_philo	philo;

	info.starve_time = atoi(argv[2]);
	info.eat_time = atoi(argv[3]);
	info.sleep_time = atoi(argv[4]);
	info.eat_count = atoi(argv[5]);

	philo.info = &info;
	philo.num = 1;

	pthread_create(&philo.thread, NULL, philo_routine, &philo);

	pthread_join(philo.thread, NULL);
	return 0;
}
