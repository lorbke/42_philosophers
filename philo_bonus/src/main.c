/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:19:44 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/07 16:04:08 by lorbke           ###   ########.fr       */
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
#include <fcntl.h> // sem_open flags
#include <signal.h> // terminate signal
#include <sys/wait.h> // waitpid

int			parse(int argc, char **argv, t_info *info);

static void	clean_up(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].info->philo_count)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	while (--i >= 0)
	{
		sem_close(philos[i].eat_sem);
		sem_unlink(EAT_SEM);
		sem_close(philos[i].fed_sem);
		sem_unlink(FED_SEM);
	}
	sem_close(philos[0].info->forks);
	sem_unlink(FORKS_SEM);
	sem_close(philos[0].info->print_sem);
	sem_unlink(PRINT_SEM);
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
		{
			philo_start(&philos[i]);
			return (0);
		}
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
		philos[i].fed = false;
		philos[i].info = info;
		philos[i].last_meal = 0;
		sem_unlink(EAT_SEM);
		philos[i].eat_sem = sem_open(EAT_SEM, O_CREAT, SEM_PERMS, 1);
		sem_unlink(FED_SEM);
		philos[i].fed_sem = sem_open(FED_SEM, O_CREAT, SEM_PERMS, 1);
		i++;
	}
}

static void	forever_alone(t_info *info)
{
	if (!info->meal_count)
		return ;
	printf("%dms %d %s\n", 0, 1, FORK);
	sniper_usleep(info->starve_time * 1000);
	printf("%lldms %d %s\n", info->starve_time + 1, 1, DIE);
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
