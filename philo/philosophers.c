/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:22:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 17:41:36 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_check_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->master);
	if (philo->info->alive == false)
	{
		pthread_mutex_unlock(&philo->info->master);
		return (true);
	}
	pthread_mutex_unlock(&philo->info->master);
	return (false);
}

void	philo_print(t_philo *philo, char *str)
{
	if (!philo_check_death(philo))
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, str);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	int				meals;

	philo = (t_philo *)arg;
	philo->last_meal = get_time() - philo->info->start_time;
	meals = 0;
	while (!philo_check_death(philo))
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
	pthread_mutex_lock(&philo->info->master);
	philo->done = true;
	pthread_mutex_unlock(&philo->info->master);
	return (NULL);
}
