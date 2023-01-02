/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:22:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/02 18:53:06 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo)
{
	printf("%ldms %d %s\n", get_time() - philo->info->start_time,
		philo->num, THINK);
}

void	philo_eat(t_philo *philo)
{
	printf("%ldms %d %s\n", get_time() - philo->info->start_time,
		philo->num, EAT);
	philo->last_meal = get_time() - philo->info->start_time;
	if (philo->info->starve_time < philo->info->eat_time)
		sniper_usleep(philo->info->starve_time + 3000);
	else
		sniper_usleep(philo->info->eat_time);
	pthread_mutex_unlock(&philo->fork_r);
	pthread_mutex_unlock(philo->fork_l);
}

void	philo_take_forks(t_philo *philo)
{
	if (philo->num % 2 == 0)
	{
		pthread_mutex_lock(&philo->fork_r);
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, FORK);
		pthread_mutex_lock(philo->fork_l);
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->fork_l);
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, FORK);
		pthread_mutex_lock(&philo->fork_r);
		printf("%ldms %d %s\n", get_time() - philo->info->start_time,
			philo->num, FORK);
	}
}

void	philo_sleep(t_philo *philo)
{
	printf("%ldms %d %s\n", get_time() - philo->info->start_time,
		philo->num, SLEEP);
	if (philo->info->starve_time < philo->info->sleep_time)
		sniper_usleep(philo->info->starve_time + 3000);
	else
		sniper_usleep(philo->info->sleep_time);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	int				meals;

	philo = (t_philo *)arg;
	meals = 0;
	philo->last_meal = philo->info->start_time;
	while (1)
	{
		if (philo->info->meal_count != -1 && meals == philo->info->meal_count)
			break ;
		i = 0;
		while (i < ACTION_COUNT)
		{
			philo->info->func_action[i](philo);
			if (philo_check_death(philo))
			{
				pthread_mutex_unlock(&philo->fork_r);
				pthread_mutex_unlock(philo->fork_l);
				return (NULL);
			}
			i++;
		}
		meals++;
	}
	pthread_mutex_lock(&philo->info->master);
	philo->done = true;
	pthread_mutex_unlock(&philo->info->master);
	return (NULL);
}
