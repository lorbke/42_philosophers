/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/04 15:12:10 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/04 17:11:34 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIME_H
# define TIME_H

/* TYPEDEFS */
typedef long long		t_ms;

/* PROTOTYPES */
t_ms	get_time(void);
void	sniper_usleep(t_ms time);

#endif
