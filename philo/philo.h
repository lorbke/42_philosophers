/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:23:47 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/02 19:50:54 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* INCLUDES */
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <time.h>
# include "ft_strtoi.h"

/* DEFINES */
# define ACTION_COUNT 4
# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define FORK "has taken a fork"
# define DIE "died"

/* TYPEDEFS */
typedef long int		t_ms;
typedef struct s_info	t_info;
typedef struct s_philo	t_philo;
typedef void			(*t_func_action)(t_philo *philo);

/* STRUCTS */
struct s_philo
{
	t_info			*info;
	int				num;
	bool			done;
	t_ms			last_meal;
	pthread_mutex_t	fork_r;
	pthread_mutex_t	*fork_l;
	pthread_t		thread;
};

struct s_info
{
	t_ms			starve_time;
	t_ms			eat_time;
	t_ms			sleep_time;
	t_ms			start_time;
	int				meal_count;
	bool			alive;
	pthread_mutex_t	master;
	t_func_action	func_action[ACTION_COUNT];
};

/* PROTOTYPES */
void	philo_take_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	*philo_routine(void *arg);

bool	waiter_check_death(t_philo *philo);
void	waiter_dining(t_info *info, int num);

t_ms	get_time(void);
void	sniper_usleep(t_ms time);

#endif
