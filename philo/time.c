/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 19:07:45 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/02 19:07:58 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_ms	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

void	sniper_usleep(t_ms time)
{
	t_ms	wake_up;

	wake_up = get_time() + time / 1000;
	usleep(time - time / 10);
	while (wake_up > get_time())
		;
}
