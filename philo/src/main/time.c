/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 19:07:45 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 20:41:43 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "time.h"
#include <sys/time.h> // gettimeofday
#include <unistd.h> // usleep
#include <stddef.h> // NULL

t_ms	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

void	sniper_usleep(long time)
{
	t_ms	wake_up;

	wake_up = get_time() + time / 1000;
	while (get_time() < wake_up)
		usleep(200);
}
