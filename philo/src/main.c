/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:19:44 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/07 01:14:23 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lib.h"
#include "philo.h"
#include "waitress.h"
#include "time.h"
#include <unistd.h> // write
#include <stdlib.h> // malloc
#include <pthread.h> // pthread_create
#include <stdio.h> // printf

int			parse(int argc, char **argv, t_info *info);

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

int	create_philos(t_philo *philos)
{
	int			i;
	int			error;

	i = 0;
	error = 0;
	while (i < philos[0].info->philo_count)
	{
		error += pthread_create
			(&philos[i].thread, NULL, &philo_routine, &philos[i]);
		i++;
	}
	if (error)
		return (1);
	return (0);
}

static void	init_philos(t_info *info, t_philo *philos)
{	
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		philos[i].num = i + 1;
		philos[i].status = true;
		philos[i].fed = false;
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

static void	forever_alone(t_info *info)
{
	if (!info->meal_count)
		return ;
	printf("%dms %d %s\n", 0, 1, FORK);
	sniper_usleep(info->starve_time * 1000);
	printf("%lldms %d %s\n", info->starve_time + 1, 1, DIE);
}

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			*philos;
	pthread_t		waitress;

	if (parse(argc, argv, &info) == 1)
		return (1);
	if (info.philo_count == 1)
	{
		forever_alone(&info);
		return (0);
	}
	philos = malloc(sizeof(t_philo) * info.philo_count);
	if (!philos)
		return (1);
	init_philos(&info, philos);
	if (pthread_create(&waitress, NULL, &waitress_routine, philos) == 1)
		return (1);
	if (create_philos(philos) == 1)
		return (1);
	clean_up(&waitress, philos);
	return (0);
}
