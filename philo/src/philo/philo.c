/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:22:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 21:15:59 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

static bool	check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->status_mutex);
	if (philo->status == false)
	{
		pthread_mutex_unlock(&philo->status_mutex);
		return (true);
	}
	pthread_mutex_unlock(&philo->status_mutex);
	return (false);
}

void	print_action(t_philo *philo, char *str)
{
	if (!check_death(philo))
	{
		pthread_mutex_lock(&philo->info->print_mutex);
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, str);
		pthread_mutex_unlock(&philo->info->print_mutex);
	}
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	int				meals;

	philo = (t_philo *)arg;
	if (philo->num % 2 == 0)
		sniper_usleep(philo->info->eat_time / 2);
	meals = 0;
	while (!check_death(philo))
	{
		if (philo->info->meal_count != -1 && meals == philo->info->meal_count)
			break ;
		i = 0;
		while (i < ACTION_COUNT)
		{
			philo->info->func_action[i](philo);
			i++;
		}
		meals++;
	}
	pthread_mutex_lock(&philo->fed_mutex);
	philo->fed = true;
	pthread_mutex_unlock(&philo->fed_mutex);
	return (NULL);
}
