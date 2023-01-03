/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:20:49 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 21:17:59 by lorbke           ###   ########.fr       */
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
	{
		pthread_mutex_destroy(&philos[i].fork_r);
		pthread_mutex_destroy(&philos[i].status_mutex);
		pthread_mutex_destroy(&philos[i].eat_mutex);
		pthread_mutex_destroy(&philos[i].fed_mutex);
	}
	pthread_mutex_destroy(&philos[0].info->print_mutex);
	free(philos);
}

static void	waiter_philo_init(t_info *info, t_philo *philo, int num)
{
	philo->num = num;
	philo->status = true;
	philo->info = info;
	philo->last_meal = 0;
	pthread_mutex_init(&philo->fork_r, NULL);
	pthread_mutex_init(&philo->status_mutex, NULL);
	pthread_mutex_init(&philo->eat_mutex, NULL);
	pthread_mutex_init(&philo->fed_mutex, NULL);
}

void	waiter_open_diner(t_info *info)
{
	pthread_t	waiter;
	t_philo		*philos;
	int			i;

	philos = malloc(sizeof(t_philo) * info->philo_count);
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
