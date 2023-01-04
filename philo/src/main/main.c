/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:19:44 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 21:16:57 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lib.h"
#include "philo.h"
#include "waiter.h"
#include "time.h"
#include <unistd.h> // write
#include <stdlib.h> // malloc
#include <pthread.h> // pthread_create
#include <stdio.h> // printf

static void	init_philos(t_info *info, t_philo *philos)
{	
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		philos[i].num = i + 1;
		philos[i].status = true;
		philos[i].info = info;
		philos[i].last_meal = 0;
		pthread_mutex_init(&philos[i].fork_r, NULL);
		pthread_mutex_init(&philos[i].status_mutex, NULL);
		pthread_mutex_init(&philos[i].eat_mutex, NULL);
		pthread_mutex_init(&philos[i].fed_mutex, NULL);
		philos[i].fork_l = &philos[(i + 1) % info->philo_count].fork_r;
		i++;
	}
}

static int	init_info(t_info *info, int argc, char **argv)
{
	info->philo_count = ft_strtoi(argv[1], NULL, 10);
	info->starve_time = ft_strtoi(argv[2], NULL, 10);
	info->eat_time = ft_strtoi(argv[3], NULL, 10) * 1000;
	info->sleep_time = ft_strtoi(argv[4], NULL, 10) * 1000;
	info->meal_count = 0;
	if (argc == 6)
		info->meal_count = ft_strtoi(argv[5], NULL, 10);
	if (info->starve_time < 0 || info->eat_time < 0
		|| info->sleep_time < 0 || info->meal_count < 0)
	{
		write(2, "Error: parameters can not be negative\n", 38);
		return (1);
	}
	if (argc == 5)
		info->meal_count = -1;
	pthread_mutex_init(&info->print_mutex, NULL);
	info->func_action[0] = &philo_take_forks;
	info->func_action[1] = &philo_eat;
	info->func_action[2] = &philo_sleep;
	info->func_action[3] = &philo_think;
	info->start_time = get_time();
	return (0);
}

static void	forever_alone(t_info *info)
{
	printf("%dms %d %s\n", 0, 1, FORK);
	sniper_usleep(info->starve_time);
	printf("\033[31m%ldms %d %s\033[0m\n", info->starve_time + 1, 1, DIE);
}

static int	check_args(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	if (ft_strtoi(argv[1], NULL, 10) < 1)
	{
		write(2, "Error: number of philosophers must be greater than 0\n", 52);
		return (1);
	}
	return (0);
}

static void	clean_up(pthread_t *waiter, t_philo *philos)
{
	int	i;

	pthread_join(*waiter, NULL);
	i = 0;
	while (i < philos[0].info->philo_count)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	while (--i >= 0)
	{
		pthread_mutex_destroy(&philos[i].fork_r);
		pthread_mutex_destroy(&philos[i].status_mutex);
		pthread_mutex_destroy(&philos[i].eat_mutex);
		pthread_mutex_destroy(&philos[i].fed_mutex);
	}
	pthread_mutex_destroy(&philos[0].info->print_mutex);
	free(philos);
}

void	create_philos(t_philo *philos)
{
	int			i;

	i = 0;
	while (i < philos[0].info->philo_count)
	{
		pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]);
		i++;
	}
}

// time management and overflows (*1000 problem)
// param overflow, negative switched to isdigit check
// helgrind and drd check

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			*philos;
	pthread_t		waiter;

	if (check_args(argc, argv) == 1)
		return (1);
	if (init_info(&info, argc, argv) == 1)
		return (1);
	if (info.philo_count == 1)
	{
		forever_alone(&info);
		return (0);
	}
	philos = malloc(sizeof(t_philo) * info.philo_count);
	init_philos(&info, philos);
	pthread_create(&waiter, NULL, &waiter_routine, philos);
	create_philos(philos);
	clean_up(&waiter, philos);
	return (0);
}
