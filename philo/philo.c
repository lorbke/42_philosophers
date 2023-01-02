/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:19:44 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/02 19:43:39 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	info_init(t_info *info, int argc, char **argv)
{
	info->starve_time = ft_strtoi(argv[2], NULL, 10);
	info->eat_time = ft_strtoi(argv[3], NULL, 10) * 1000;
	info->sleep_time = ft_strtoi(argv[4], NULL, 10) * 1000;
	if (argc == 6)
		info->meal_count = ft_strtoi(argv[5], NULL, 10);
	if (info->starve_time < 0 || info->eat_time < 0
		|| info->sleep_time < 0 || info->meal_count < 0)
	{
		write(2, "Error: parameters can not be negative\n", 38);
		return (1);
	}
	if (argc == 5)
		info->meal_count = -1;
	info->alive = true;
	pthread_mutex_init(&info->master, NULL);
	info->func_action[0] = &philo_take_forks;
	info->func_action[1] = &philo_eat;
	info->func_action[2] = &philo_sleep;
	info->func_action[3] = &philo_think;
	return (0);
}

static void	forever_alone(t_info *info)
{
	printf("%dms %d %s\n", 0, 1, FORK);
	sniper_usleep(info->starve_time);
	printf("\033[31m%ldms %d %s\033[0m\n", info->starve_time + 1, 1, DIE);
}

// time management
// helgrind etc.

int	main(int argc, char **argv)
{
	t_info			info;
	int				philo_num;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error: wrong number of arguments\n", 33);
		return (1);
	}
	if (info_init(&info, argc, argv) == 1)
		return (1);
	philo_num = ft_strtoi(argv[1], NULL, 10);
	if (philo_num == 1)
	{
		forever_alone(&info);
		return (0);
	}
	else if (philo_num < 1)
	{
		write(2, "Error: number of philosophers must be greater than 0\n", 52);
		return (1);
	}
	waiter_dining(&info, philo_num);
	return (0);
}
