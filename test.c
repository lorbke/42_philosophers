#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct s_info
{
	int	starve_time;
	int	eat_time;
	int	sleep_time;
	int	eat_count;
}	t_info;

typedef struct s_philo
{
	int				num;
	bool			alive;
	bool			done;
	t_info			*info;
	pthread_mutex_t	fork_r;
	pthread_mutex_t	*fork_l;
	pthread_t		thread;
}	t_philo;

int	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_usec / 1000);
}

bool	philo_check_death(t_philo *philo, int timestamp)
{
	if (get_time() - timestamp > philo->info->starve_time)
	{
		philo->alive = false;
		printf("----------------%dms %d died\n", timestamp, philo->num);
		return (true);
	}
	return (false);
}

void	*philo_routine(void *arg)
{
	t_philo					*philo;
	struct timeval			time;
	int						timestamp;
	int						i;

	philo = (t_philo *)arg;
	i = 0;
	timestamp = get_time();
	while (i < philo->info->eat_count)
	{
		printf("%dms %d is thinking\n", timestamp, philo->num);
		pthread_mutex_lock(&philo->fork_r);
		printf("%dms %d has taken a fork\n", timestamp, philo->num);
		pthread_mutex_lock(philo->fork_l);
		printf("%dms %d has taken a fork\n", timestamp, philo->num);
		if (philo_check_death(philo, timestamp) == true)
			return (NULL);
		timestamp = get_time();
		printf("%dms %d is eating\n", timestamp, philo->num);
		usleep(philo->info->eat_time);
		if (philo_check_death(philo, timestamp) == true)
			return (NULL);
		timestamp = get_time();
		pthread_mutex_unlock(&philo->fork_r);
		pthread_mutex_unlock(philo->fork_l);
		printf("%dms %d is sleeping\n", timestamp, philo->num);
		usleep(philo->info->sleep_time);
		i++;
	}
	pthread_mutex_lock(&philo->fork_r);
	pthread_mutex_lock(philo->fork_l);
	// printf("-- %p\n", &philo->fork_r);
	// printf("-- %p\n", philo);
	philo->done = true;
	pthread_mutex_unlock(&philo->fork_r);
	pthread_mutex_unlock(philo->fork_l);
	return (NULL);
}

void	philos_create(t_philo *philos, t_info *info, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		philos[i].num = i + 1;
		philos[i].alive = true;
		philos[i].done = false;
		philos[i].info = info;
		pthread_mutex_init(&philos[(i + 1) % num].fork_r, NULL);
		philos[i].fork_l = &philos[(i + 1) % num].fork_r;
		if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]) != 0)
			printf("error %d\n", i + 1);
		i++;
	}
}

void	philos_kill(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_lock(&philos[i].fork_r);
		pthread_mutex_lock(philos[i].fork_l);
		philos[i].alive = false;
		pthread_mutex_unlock(&philos[i].fork_r);
		pthread_mutex_unlock(philos[i].fork_l);
		i++;
	}
}

void	philos_free(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_join(philos[i].thread, NULL);
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
			pthread_mutex_lock(&philos[i].fork_r);
			pthread_mutex_lock(philos[i].fork_l);
			if (philos[i].alive == false)
			{
				philos_kill(philos, num);
				philos_free(philos, num);
				return ;
			}
			// printf("%p\n", &philos[i].fork_r);
			// printf("%p\n", &philos[i]);
			// exit(0);
			if (philos[i].done == true)
				counter++;
			pthread_mutex_unlock(&philos[i].fork_r);
			pthread_mutex_unlock(philos[i].fork_l);
			if (counter == num)
			{
				philos_free(philos, num);
				return ;
			}
			i++;
		}
		// write(1, "1", 1);
	}
}

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			philo;
	t_philo			philo2;

	info.starve_time = atoi(argv[2]);
	info.eat_time = atoi(argv[3]) * 1000;
	info.sleep_time = atoi(argv[4]) * 1000;
	info.eat_count = atoi(argv[5]);

	philos_dining(&info, atoi(argv[1]));

	return (0);
}
