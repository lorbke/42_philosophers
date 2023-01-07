/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 15:05:29 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/04 18:14:58 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lib.h"
#include "philo.h"
#include "waitress.h"
#include "time.h"
#include <unistd.h> // write
#include <stdlib.h> // malloc
#include <pthread.h> // pthread_create
#include <stdio.h> // printf

static int	init_info(t_info *info, int argc, char **argv)
{
	info->philo_count = ft_strtoi(argv[1], NULL, 10);
	info->starve_time = ft_strtoi(argv[2], NULL, 10);
	info->eat_time = ft_strtoi(argv[3], NULL, 10) * 1000;
	info->sleep_time = ft_strtoi(argv[4], NULL, 10) * 1000;
	if (info->philo_count < 0 || info->starve_time < 0 || info->eat_time < 0
		|| info->sleep_time < 0)
		return (1);
	info->meal_count = -1;
	if (argc == 6)
		info->meal_count = ft_strtoi(argv[5], NULL, 10);
	pthread_mutex_init(&info->print_mutex, NULL);
	info->func_action[0] = &philo_take_forks;
	info->func_action[1] = &philo_eat;
	info->func_action[2] = &philo_sleep;
	info->func_action[3] = &philo_think;
	info->start_time = get_time();
	return (0);
}

static int	check_args(int argc, char **argv)
{
	int	count;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	count = ft_strtoi(argv[1], NULL, 10);
	argv++;
	while (*argv)
	{
		if (!ft_strisdigit(*argv))
		{
			write(2, "Error: parameters must be numbers only\n", 39);
			return (1);
		}
		argv++;
	}
	if (count < 1)
	{
		write(2, "Error: number of philosophers must be greater than 0\n", 53);
		return (1);
	}
	return (0);
}

int	parse(int argc, char **argv, t_info *info)
{
	if (check_args(argc, argv) == 1)
		return (1);
	if (init_info(info, argc, argv) == 1)
	{
		write(2, "Error: integer overflow\n", 24);
		return (1);
	}
	return (0);
}
