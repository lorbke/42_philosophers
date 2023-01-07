/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 20:58:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/07 00:45:50 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "waitress.h"
#include "philo_bonus.h" // t_info, t_philo, DIE
#include <pthread.h> // pthread_mutex_lock, pthread_mutex_unlock
#include <stdio.h> // printf
#include <stdlib.h> // exit

static bool	check_death_all(void)
{
	sem_t	*status;

	status = sem_open(STATUS_SEM, 0);
	if (status == SEM_FAILED)
		return (false);
	sem_close(status);
	return (true);
}

static bool	check_death(t_philo *philo)
{
	sem_wait(philo->eat_sem);
	sem_wait(philo->fed_sem);
	if (philo->fed == false && get_time() - philo->info->start_time
		- philo->last_meal > philo->info->starve_time)
	{
		sem_post(philo->eat_sem);
		sem_post(philo->fed_sem);
		return (true);
	}
	sem_post(philo->eat_sem);
	sem_post(philo->fed_sem);
	return (false);
}

static void	kill_philos(void)
{
	sem_t	*status;

	sem_unlink(STATUS_SEM);
	status = sem_open(STATUS_SEM, O_CREAT, SEM_PERMS, 0);
}

void	*waitress_routine(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_death_all() == true)
			return (NULL);
		if (check_death(philo))
		{
			sem_wait(philo->info->print_sem);
			printf("%lldms %d %s\n", get_time() - philo->info->start_time,
				philo->num, DIE);
			kill_philos();
			sem_post(philo->info->print_sem);
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
