/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:22:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/10 15:51:29 by lorbke           ###   ########.fr       */
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
	pthread_mutex_lock(&philo->info->print_mutex);
	if (!check_death(philo))
	{
		printf("%lld %d %s\n", get_time() - philo->info->start_time,
			philo->num, str);
		pthread_mutex_unlock(&philo->info->print_mutex);
	}
	else
		pthread_mutex_unlock(&philo->info->print_mutex);
}

static void	set_fed(t_philo *philo)
{
	pthread_mutex_lock(&philo->fed_mutex);
	philo->fed = true;
	pthread_mutex_unlock(&philo->fed_mutex);
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
	set_fed(philo);
	return (NULL);
}
