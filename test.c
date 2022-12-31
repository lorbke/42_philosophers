#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_info
{
	__darwin_suseconds_t	starve_time;
	__darwin_suseconds_t	eat_time;
	__darwin_suseconds_t	sleep_time;
	int						eat_count;
}	t_info;

typedef struct s_philo
{
	int				num;
	bool			alive;
	bool			done;
	t_info			*info;
	pthread_mutex_t	*fork;
	pthread_t		thread;
}	t_philo;

void	philo_forks_get(t_philo *philo, __darwin_suseconds_t timestamp)
{
	if (pthread_mutex_lock(philo->fork) == 0)
	{
		printf("%dms %d has taken a fork\n", timestamp, philo->num);
		printf("%dms %d is eating\n", timestamp, philo->num);
		usleep(philo->info->eat_time);
		pthread_mutex_unlock(philo->fork);
		printf("%dms %d is sleeping\n", timestamp, philo->num);
		usleep(philo->info->sleep_time);
	}
	else
		pthread_mutex_unlock(philo->fork);
	
}

void	*philo_routine(void *arg)
{
	t_philo					*philo;
	struct timeval			time;
	__darwin_suseconds_t	timestamp;
	__darwin_suseconds_t	timestamp_old;
	int						i;

	philo = (t_philo *)arg;
	gettimeofday(&time, NULL);
	timestamp = time.tv_usec / 1000;
	i = 0;
	while (philo->alive == true && i < philo->info->eat_count)
	{
		timestamp_old = timestamp;
		gettimeofday(&time, NULL);
		timestamp = time.tv_usec / 1000; // converting to milliseconds
		if (timestamp - timestamp_old > philo->info->starve_time)
		{
			printf("----------------%dms %d died\n", timestamp, philo->num);
			philo->alive = false; // potential data race with dining loop thread
			return (NULL);
		}
		printf("%dms %d is thinking\n", timestamp, philo->num);
		philo_forks_get(philo, timestamp);
		i++;
	}
	printf("-------------%d is done eating\n", philo->num);
	philo->done = true;
	return (NULL);
}

void	philos_create(t_philo *philos, t_info *info, int num)
{
	pthread_mutex_t	fork;
	int				i;

	pthread_mutex_init(&fork, NULL);
	i = 0;
	while (i < num)
	{
		philos[i].num = i + 1;
		philos[i].alive = true;
		philos[i].done = false;
		philos[i].info = info;
		philos[i].fork = &fork;
		pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
		i++;
	}
}

void	philos_kill(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		philos[i].alive = false;
		i++;
	}
}

void	philos_free(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		write(1, "x",1);
		pthread_join(philos[i].thread, NULL);
		write(1, "y",1);
		i++;
	}
	// free(philos);
}

void	philos_dining(t_info *info, int num)
{
	t_philo			*philos;
	int				i;
	int				counter;

	philos = malloc(sizeof(t_philo) * num);
	philos_create(philos, info, num);
	while (1)
	{
		i = 0;
		counter = 0;
		while (i < num)
		{
			if (philos[i].alive == false)
			{
				philos_kill(philos, num);
				philos_free(philos, num);
				return ;
			}
			if (philos[i].done == true)
				counter++;
			if (counter == num)
			{
				philos_free(philos, num);
				return ;
			}
			i++;
		}
	}
}

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			philo;
	t_philo			philo2;
	pthread_mutex_t	fork;

	info.starve_time = atoi(argv[2]);
	info.eat_time = atoi(argv[3]) * 1000;
	info.sleep_time = atoi(argv[4]) * 1000;
	info.eat_count = atoi(argv[5]);

	philos_dining(&info, atoi(argv[1]));

	return (0);
}
