/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:23:47 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/03 21:16:11 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/* INCLUDES */
# include "time.h" // t_ms
# include <stddef.h> // size_t
# include <pthread.h> // pthread_t, pthread_mutex_t
# include <stdbool.h> // bool

/* DEFINES */
# define ACTION_COUNT 4
# define THINK "is thinking"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define FORK "has taken a fork"
# define DIE "died"

/* TYPEDEFS */
typedef struct s_info	t_info;
typedef struct s_philo	t_philo;
typedef void			(*t_func_action)(t_philo *philo);

/* STRUCTS */
struct s_philo
{
	t_info			*info;
	int				num;
	bool			fed;
	bool			status;
	t_ms			last_meal;
	pthread_mutex_t	fork_r;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	status_mutex;
	pthread_mutex_t	eat_mutex;
	pthread_mutex_t	fed_mutex;
	pthread_t		thread;
};

struct s_info
{
	int				philo_count;
	t_ms			starve_time;
	t_ms			eat_time;
	t_ms			sleep_time;
	int				meal_count;
	t_ms			start_time;
	pthread_mutex_t	print_mutex;
	t_func_action	func_action[ACTION_COUNT];
};

/* PROTOTYPES */
void	philo_take_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	print_action(t_philo *philo, char *str);
void	*philo_routine(void *arg);

#endif
