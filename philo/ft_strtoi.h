/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoi.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorbke <lorbke@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/01 21:49:27 by lorbke            #+#    #+#             */
/*   Updated: 2023/01/01 21:59:08 by lorbke           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRTOI_H
# define FT_STRTOI_H

# include <errno.h>
# include <limits.h>
# include <stdlib.h>

int	ft_safemult(int a, int b);
int	ft_safeadd(int a, int b);
int	ft_isdigit(int c);
int	ft_isspace(int c);
int	ft_isdigit(int c);
int	ft_isalnum(int c);
int	ft_isalpha(int c);
int	ft_strtoi(const char *str, const char **endptr, int base);

#endif