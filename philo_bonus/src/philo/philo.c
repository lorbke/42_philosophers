/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:22:37 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/07 01:23:41 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include "waitress.h"
#include <stdio.h> // printf
#include <stdlib.h> // exit

static bool	check_death(void)
{
	sem_t	*status;

	status = sem_open(STATUS_SEM, 0);
	if (status == SEM_FAILED)
		return (false);
	sem_close(status);
	return (true);
}

void	print_action(t_philo *philo, char *str)
{
	sem_wait(philo->info->print_sem);
	if (check_death() == true)
	{
		sem_post(philo->info->print_sem);
		return ;
	}
	printf("%lldms %d %s\n", get_time() - philo->info->start_time,
		philo->num, str);
	sem_post(philo->info->print_sem);
}

static void	*philo_routine(void *arg)
{
	t_philo			*philo;
	int				i;
	int				meals;

	philo = (t_philo *)arg;
	if (philo->num % 2 == 0)
		sniper_usleep(philo->info->eat_time / 2);
	meals = 0;
	while (!check_death())
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
	sem_wait(philo->fed_sem);
	philo->fed = true;
	sem_post(philo->fed_sem);
	return (NULL);
}

int	philo_start(t_philo *philo)
{
	pthread_t		waitress;

	if (pthread_create(&waitress, NULL, &waitress_routine, philo) == 1)
		return (1);
	philo_routine(philo);
	pthread_join(waitress, NULL);
	return (0);
}
