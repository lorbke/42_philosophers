/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:20:49 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/02 18:58:56 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	join_free_destroy(t_philo *philos, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&philos[i].fork_r);
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free(philos);
}

static void	philos_create(t_philo *philos, t_info *info, int num)
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
		usleep(100);
		i++;
	}
}

bool	philo_check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->master);
	if (philo->info->alive == false)
	{
		pthread_mutex_unlock(&philo->info->master);
		return (true);
	}
	if (get_time() - philo->info->start_time - philo->last_meal
		> philo->info->starve_time)
	{
		philo->info->alive = false;
		pthread_mutex_unlock(&philo->info->master);
		printf("\033[31m%ldms %d %s\033[0m\n", get_time()
			- philo->info->start_time, philo->num, DIE);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->master);
	return (false);
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
	join_free_destroy(philos, num);
}
