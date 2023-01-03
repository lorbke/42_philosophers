/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 19:07:45 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 18:14:52 by lorbke           ###   ########.fr       */
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
	t_ms	temp;

	wake_up = get_time() + time / 1000;
	temp = time - 5000;
	if (temp > 0)
		usleep(temp);
	while (wake_up > get_time())
		;
}
