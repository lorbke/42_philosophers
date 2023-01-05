/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:19:44 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/05 19:52:40 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lib.h"
#include "philo_bonus.h"
#include "waitress.h"
#include "time.h"
#include <unistd.h> // write
#include <stdlib.h> // malloc
#include <pthread.h> // pthread functions and types
#include <stdio.h> // printf
#include <fcntl.h> // O_ flags
#include <signal.h> // terminate signal

int			parse(int argc, char **argv, t_info *info);

static void	clean_up(t_philo *philos)
{
	int	i;

	i = 0;
	sem_wait(philos[0].info->status_sem);
	while (i < philos[0].info->philo_count)
	{
		kill(philos[i].pid, SIGTERM);
		i++;
	}
	while (--i >= 0)
	{
		sem_close(philos[i].eat_sem);
		sem_unlink("eat_sem");
		sem_close(philos[i].fed_sem);
		sem_unlink("fed_sem");
	}
	sem_close(philos[0].info->forks);
	sem_unlink("forks");
	sem_close(philos[0].info->print_sem);
	sem_unlink("print");
	sem_close(philos[0].info->status_sem);
	sem_unlink("status");
	free(philos);
}

int	create_philos(t_philo *philos)
{
	int			i;

	i = 0;
	while (i < philos[0].info->philo_count)
	{
		philos[i].pid = fork();
		if (philos[i].pid == 0)
			philo_start(&philos[i]);
		else if (philos[i].pid < 0)
			return (1);
		i++;
	}
	return (0);
}

static void	init_philos(t_info *info, t_philo *philos)
{	
	int	i;

	i = 0;
	while (i < info->philo_count)
	{
		philos[i].num = i + 1;
		philos[i].status = true;
		philos[i].fed = false;
		philos[i].info = info;
		philos[i].last_meal = 0;
		sem_unlink("eat_sem");
		philos[i].eat_sem = sem_open("eat_sem", O_CREAT, 0666, 1);
		sem_unlink("fed_sem");
		philos[i].fed_sem = sem_open("fed_sem", O_CREAT, 0666, 1);
		i++;
	}
}

static void	forever_alone(t_info *info)
{
	if (!info->meal_count)
		return ;
	printf("%dms %d %s\n", 0, 1, FORK);
	sniper_usleep(info->starve_time * 1000);
	printf("\033[31m%lldms %d %s\033[0m\n", info->starve_time + 1, 1, DIE);
}

int	main(int argc, char **argv)
{
	t_info			info;
	t_philo			*philos;

	if (parse(argc, argv, &info) == 1)
		return (1);
	if (info.philo_count == 1)
	{
		forever_alone(&info);
		return (0);
	}
	philos = malloc(sizeof(t_philo) * info.philo_count);
	if (!philos)
		return (1);
	init_philos(&info, philos);
	if (create_philos(philos) == 1)
		return (1);
	clean_up(philos);
	return (0);
}
