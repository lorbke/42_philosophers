/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:58:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/05 19:47:51 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "waitress.h"
#include "philo_bonus.h" // t_info, t_philo, DIE
#include <pthread.h> // pthread_mutex_lock, pthread_mutex_unlock
#include <stdio.h> // printf

static bool	check_death(t_philo *philo)
{
	sem_wait(philo->eat_sem);
	sem_wait(philo->fed_sem);
	if (philo->fed == false && get_time() - philo->info->start_time
		- philo->last_meal > philo->info->starve_time)
	{
		sem_post(philo->eat_sem);
		sem_post(philo->fed_sem);
		sem_wait(philo->info->print_sem);
		printf("\033[31m%lldms %d %s\033[0m\n", get_time()
			- philo->info->start_time, philo->num, DIE);
		sem_post(philo->info->print_sem);
		return (true);
	}
	sem_post(philo->eat_sem);
	sem_post(philo->fed_sem);
	return (false);
}

static void	kill_philos(t_philo *philo)
{
	sem_post(philo->info->status_sem);
}

void	*waitress_routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_death(philo))
		{
			kill_philos(philo);
			return (NULL);
		}
		sem_wait(philo->fed_sem);
		if (philo->fed == true)
		{
			sem_post(philo->fed_sem);
			return (NULL);
		}
		sem_post(philo->fed_sem);
	}
	return (NULL);
}
