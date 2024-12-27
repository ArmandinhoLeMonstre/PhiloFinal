/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 21:37:26 by armitite          #+#    #+#             */
/*   Updated: 2024/12/14 18:27:12 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c <= '9' && c >= '0')
		return (1);
	else
		return (0);
}

int	ft_check_num(char **av, int ac)
{
	int	i;
	int	x;

	i = 0;
	x = 1;
	while (x < ac)
	{
		i = 0;
		while (av[x][i])
		{
			if (ft_isdigit(av[x][i]) == 0)
				return (1);
			i++;
		}
		x++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	unsigned long int	res;
	unsigned long int	sign;
	unsigned long int	i;

	res = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		res = res * 10;
		res = res + (str[i] - 48);
		i++;
	}
	return (res * sign);
}
