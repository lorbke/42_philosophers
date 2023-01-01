#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "ft_strtoi.h"

typedef struct s_info
{
	int				starve_time;
	int				eat_time;
	int				sleep_time;
	int				eat_count;
	long int		start_time;
	bool			alive;
	pthread_mutex_t	master;
}	t_info;

typedef struct s_philo
{
	int				num;
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
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

bool	philo_check_death(t_philo *philo, int last_meal)
{
	pthread_mutex_lock(&philo->info->master);
	if (philo->info->alive == false)
	{
		pthread_mutex_unlock(&philo->fork_r);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_unlock(&philo->info->master);
		return (true);
	}
	if (get_time() - philo->info->start_time - last_meal
		> philo->info->starve_time)
	{
		philo->info->alive = false;
		pthread_mutex_unlock(&philo->fork_r);
		pthread_mutex_unlock(philo->fork_l);
		pthread_mutex_unlock(&philo->info->master);
		printf("----------------%ldms %d died\n", get_time()
			- philo->info->start_time, philo->num);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->master);
	return (false);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	struct timeval	time;
	long int		timestamp;
	long int		last_meal;
	int				i;

	philo = (t_philo *)arg;
	i = 0;
	timestamp = philo->info->start_time;
	last_meal = timestamp;
	while (1)
	{
		if (philo->info->eat_count != -1 && i == philo->info->eat_count)
			break ;
		if (philo_check_death(philo, last_meal) == true)
			return (NULL);
		timestamp = get_time() - philo->info->start_time;
		// left first if odd, right first if even
		printf("%ldms %d is thinking\n", timestamp, philo->num);
		pthread_mutex_lock(&philo->fork_r);
		if (philo_check_death(philo, last_meal) == true)
			return (NULL);
		timestamp = get_time() - philo->info->start_time;
		printf("%ldms %d has taken a fork\n", timestamp, philo->num);
		pthread_mutex_lock(philo->fork_l);
		if (philo_check_death(philo, last_meal) == true)
			return (NULL);
		timestamp = get_time() - philo->info->start_time;
		printf("%ldms %d has taken a fork\n", timestamp, philo->num);
		printf("%ldms %d is eating\n", timestamp, philo->num);
		last_meal = timestamp;
		usleep(philo->info->eat_time - philo->info->eat_time / 10);
		while (get_time() - philo->info->start_time - timestamp
			< philo->info->eat_time / 1000);
		if (philo_check_death(philo, last_meal) == true)
			return (NULL);
		timestamp = get_time() - philo->info->start_time;
		pthread_mutex_unlock(&philo->fork_r);
		pthread_mutex_unlock(philo->fork_l);
		printf("%ldms %d is sleeping\n", timestamp, philo->num);
		usleep(philo->info->sleep_time - philo->info->sleep_time / 10);
		while (get_time() - philo->info->start_time - timestamp
			< philo->info->sleep_time / 1000);
		i++;
	}
	pthread_mutex_lock(&philo->info->master);
	philo->done = true;
	pthread_mutex_unlock(&philo->info->master);
	return (NULL);
}

void	philos_create(t_philo *philos, t_info *info, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		philos[i].num = i + 1;
		philos[i].done = false;
		philos[i].info = info;
		pthread_mutex_init(&philos[(i + 1) % num].fork_r, NULL);
		philos[i].fork_l = &philos[(i + 1) % num].fork_r;
		pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]);
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
	free(philos);
}

void	philos_dining(t_info *info, int num)
{
	t_philo			*philos;
	int				i;
	int				counter;

	info->start_time = get_time();
	philos = malloc(sizeof(t_philo) * num);
	philos_create(philos, info, num);
	while (1)
	{
		pthread_mutex_lock(&info->master);
		i = 0;
		counter = 0;
		while (i < num)
		{
			if (philos[i].done == true)
				counter++;
			i++;
		}
		if (info->alive == false || counter == num)
			break ;
		pthread_mutex_unlock(&info->master);
	}
	pthread_mutex_unlock(&info->master);
	philos_free(philos, num);
}

void	info_init(t_info *info, int argc, char **argv)
{
	info->starve_time = ft_strtoi(argv[2], NULL, 10);
	info->eat_time = ft_strtoi(argv[3], NULL, 10) * 1000;
	info->sleep_time = ft_strtoi(argv[4], NULL, 10) * 1000;
	info->alive = true;
	if (argc == 6)
		info->eat_count = ft_strtoi(argv[5], NULL, 10);
	else
		info->eat_count = -1;
	pthread_mutex_init(&info->master, NULL);
}

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			philo;
	t_philo			philo2;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	info_init(&info, argc, argv);
	philos_dining(&info, atoi(argv[1]));
	return (0);
}
