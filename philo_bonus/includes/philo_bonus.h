/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 18:23:47 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/05 19:32:24 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

/* INCLUDES */
# include "time.h" // t_ms
# include <stddef.h> // size_t
# include <pthread.h> // pthread_t, pthread_mutex_t
# include <stdbool.h> // bool
# include <semaphore.h> // sem_t
# include <unistd.h> // write

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
	sem_t			*eat_sem;
	sem_t			*fed_sem;
	pid_t			pid;
};

struct s_info
{
	int				philo_count;
	t_ms			starve_time;
	t_ms			eat_time;
	t_ms			sleep_time;
	int				meal_count;
	t_ms			start_time;
	sem_t			*forks;
	sem_t			*status_sem;
	sem_t			*print_sem;
	t_func_action	func_action[ACTION_COUNT];
};

/* PROTOTYPES */
void	philo_take_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	print_action(t_philo *philo, char *str);
int		philo_start(t_philo *philo);

#endif
