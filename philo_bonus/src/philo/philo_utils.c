/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 15:50:53 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/05 19:55:59 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h> // REMOVE

void	philo_think(t_philo *philo)
{
	print_action(philo, THINK);
}

void	philo_eat(t_philo *philo)
{
	print_action(philo, EAT);
	sem_wait(philo->eat_sem);
	philo->last_meal = get_time() - philo->info->start_time;
	sem_post(philo->eat_sem);
	sniper_usleep(philo->info->eat_time);
	sem_post(philo->info->forks);
	sem_post(philo->info->forks);
}

void	philo_take_forks(t_philo *philo)
{
	sem_wait(philo->info->forks);
	print_action(philo, FORK);
	sem_wait(philo->info->forks);
	print_action(philo, FORK);
}

void	philo_sleep(t_philo *philo)
{
	print_action(philo, SLEEP);
	sniper_usleep(philo->info->sleep_time);
}
