/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:20:49 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 17:48:20 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	waiter_clean_up(pthread_t *waiter, t_philo *philos)
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
		pthread_mutex_destroy(&philos[i].fork_r);
	pthread_mutex_destroy(&philos[0].info->master);
	free(philos);
}

static bool	waiter_check_death(t_philo *philo)
{
	if (get_time() - philo->info->start_time - philo->last_meal
		> philo->info->starve_time)
	{
		philo->info->alive = false;
		printf("\033[31m%ldms %d %s\033[0m\n", get_time()
			- philo->info->start_time, philo->num, DIE);
		return (true);
	}
	return (false);
}

void	*waiter_routine(void *arg)
{
	t_philo			*philos;
	int				i;
	int				counter;

	philos = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philos[0].info->master);
		i = 0;
		counter = 0;
		while (i < philos[0].info->philo_count
			&& !waiter_check_death(&philos[i]))
		{
			if (philos[i].done == true)
				counter++;
			i++;
		}
		if (counter == philos[0].info->philo_count || !philos[0].info->alive)
			break ;
		pthread_mutex_unlock(&philos[0].info->master);
	}
	pthread_mutex_unlock(&philos[0].info->master);
	return (NULL);
}

static void	waiter_philo_init(t_info *info, t_philo *philo, int num)
{
	philo->num = num;
	philo->done = false;
	philo->info = info;
	philo->last_meal = 0;
}

void	waiter_open_diner(t_info *info)
{
	pthread_t	waiter;
	t_philo		*philos;
	int			i;

	philos = malloc(sizeof(t_philo) * info->philo_count);
	pthread_mutex_init(&info->master, NULL);
	i = 0;
	while (i < info->philo_count)
		pthread_mutex_init(&philos[i++].fork_r, NULL);
	info->start_time = get_time();
	i = 0;
	while (i < info->philo_count)
	{
		waiter_philo_init(info, &philos[i], i + 1);
		philos[i].fork_l = &philos[(i + 1) % info->philo_count].fork_r;
		i++;
	}
	pthread_create(&waiter, NULL, &waiter_routine, philos);
	i = 0;
	while (i < info->philo_count)
	{
		pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i]);
		i++;
	}
	waiter_clean_up(&waiter, philos);
}
