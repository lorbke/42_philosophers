/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 15:12:10 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/10 00:21:39 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TIME_H
# define PHILO_TIME_H

/* TYPEDEFS */
typedef long long		t_ms; // milliseconds
typedef long long		t_us; // microseconds

/* PROTOTYPES */
t_ms	get_time(void);
void	sniper_usleep(t_us time);

#endif
