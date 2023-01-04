/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter_mutex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:58:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 21:15:46 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "waitress.h"
# include "philo.h" // t_info, t_philo
#include <pthread.h> // pthread_mutex_lock, pthread_mutex_unlock
#include <stdio.h> // printf

static bool	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	if (get_time() - philo->info->start_time - philo->last_meal
		> philo->info->starve_time)
	{
		pthread_mutex_unlock(&philo->eat_mutex);
		pthread_mutex_lock(&philo->info->print_mutex);
		printf("\033[31m%ldms %d %s\033[0m\n", get_time()
			- philo->info->start_time, philo->num, DIE);
		pthread_mutex_unlock(&philo->info->print_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->eat_mutex);
	return (false);
}

static void	kill_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].info->philo_count)
	{
		pthread_mutex_lock(&philos[i].status_mutex);
		philos[i].status = false;
		pthread_mutex_unlock(&philos[i].status_mutex);
		i++;
	}
}

void	*waitress_routine(void *arg)
{
	t_philo			*philos;
	int				i;
	int				counter;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		counter = 0;
		while (i < philos[0].info->philo_count)
		{
			if (check_death(&philos[i]))
			{
				kill_philos(philos);
				return (NULL);
			}
			if (philos[i].fed == true)
				counter++;
			i++;
		}
		if (counter == philos[0].info->philo_count)
			break ;
	}
	return (NULL);
}
