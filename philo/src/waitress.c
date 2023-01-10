/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:58:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/10 01:41:50 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "waitress.h"
#include "time.h"
#include "philo.h" // t_info, t_philo, DIE
#include <pthread.h> // pthread_mutex_lock, pthread_mutex_unlock
#include <stdio.h> // printf
#include <unistd.h> // get_time

static bool	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->eat_mutex);
	pthread_mutex_lock(&philo->fed_mutex);
	if (philo->fed == false && get_time() - philo->info->start_time
		- philo->last_meal > philo->info->starve_time)
	{
		pthread_mutex_unlock(&philo->eat_mutex);
		pthread_mutex_unlock(&philo->fed_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->fed_mutex);
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

static bool	check_fed(t_philo *philo)
{
	pthread_mutex_lock(&philo->fed_mutex);
	if (philo->fed == true)
	{
		pthread_mutex_unlock(&philo->fed_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->fed_mutex);
	return (false);
}

static void	print_death(t_philo *philo, char *str)
{
	printf("%lld %d %s\n", get_time() - philo->info->start_time,
		philo->num, str);
}

void	*waitress_routine(void *arg)
{
	t_philo			*philos;
	int				i;
	int				counter;

	philos = (t_philo *)arg;
	while (1)
	{
		usleep(50);
		i = 0;
		counter = 0;
		while (i < philos[0].info->philo_count)
		{
			if (check_death(&philos[i]))
			{
				pthread_mutex_lock(&philos[0].info->print_mutex);
				print_death(&philos[i], DIE);
				kill_philos(philos);
				pthread_mutex_unlock(&philos[0].info->print_mutex);
				return (NULL);
			}
			if (check_fed(&philos[i++]))
				counter++;
		}
		if (counter == philos[0].info->philo_count)
			break ;
	}
	return (NULL);
}
