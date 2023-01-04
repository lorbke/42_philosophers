/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 17:42:48 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/04 21:11:28 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

#define THREAD_NUM 4

typedef struct s_info
{
	bool	dead;
	sem_t	*dead_sem;
}	t_info;

void	philo(t_info *info)
{
	int		value;

	sem_wait(info->dead_sem);
	info->dead = true;
	printf("philo dead\n");
	sem_post(info->dead_sem);
	exit(0);
}

void	*waitress_routine(void *arg)
{
	t_info	*info;
	int		value;

	sleep(1);
	info = (t_info *)arg;
	sem_wait(info->dead_sem);
	if (info->dead == true)
	{
		sem_post(info->dead_sem);
		printf("dead\n");
		return (NULL);
	}
	sem_post(info->dead_sem);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_info		info;
	pthread_t	waitress;
	int			pid;

	info.dead = false;
	info.dead_sem = sem_open("dead_sem", O_CREAT, 0666, 1);
	pthread_create(&waitress, NULL, &waitress_routine, &info);
	pid = fork();
	if (pid == 0)
	{
		philo(&info);
	}
	waitpid(pid, NULL, 0);
	pthread_join(waitress, NULL);
	sem_close(info.dead_sem);
	return (0);
}
