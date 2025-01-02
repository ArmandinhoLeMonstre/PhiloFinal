/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 21:10:23 by armitite          #+#    #+#             */
/*   Updated: 2025/01/02 22:42:02 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitoring_check(t_philo *p, t_data *data)
{
	if ((time_now() - p->t_last_meal) >= data->starving_t)
	{
		data->death = 1;
		printf("%lld %d died\n", (time_now() - p->time), p->n);
		pthread_mutex_unlock(&data->mutex_print);
		return (1);
	}
	if (data->meals_eaten == (data->meals_nbr * data->p_total))
	{
		data->death = 1;
		printf("%d meals have been eaten\n", data->meals_eaten);
		pthread_mutex_unlock(&data->mutex_print);
		return (1);
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_philo		**p;
	t_data		*data;
	int			i;

	p = (t_philo **)arg;
	data = p[0]->data;
	ft_usleep(150);
	while (1)
	{
		i = 0;
		pthread_mutex_lock(&data->mutex_print);
		while (i < data->p_total)
		{
			if (monitoring_check(p[i], data) == 1)
				return (NULL);
			i++;
		}
		pthread_mutex_unlock(&data->mutex_print);
		usleep(1000);
	}
}
