/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 21:10:23 by armitite          #+#    #+#             */
/*   Updated: 2025/01/01 21:22:37 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	monitoring_check(t_philo *p, t_data *data, long long last_meal)
{
	pthread_mutex_unlock(&data->mutex_data);
	if ((time_now() - last_meal) > data->starving_t)
	{
		data->death = 1;
		printf("%lld %d is dead\n", (time_now() - p->time), p->n);
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
	pthread_mutex_unlock(&data->mutex_print);
	return (0);
}

void	*monitoring(void *arg)
{
	t_philo **p = (t_philo **) arg;
    t_data  *data;
    int i;
	long long last_meal;

    data = p[0]->data;
    ft_usleep(150);
    while (1)
    {
        i = 0;
        while (i < data->p_total)
        {
            pthread_mutex_lock(&data->mutex_print);
			pthread_mutex_lock(&data->mutex_data);
			last_meal = p[i]->t_last_meal;
			if (monitoring_check(p[i], data, last_meal) == 1)
				return NULL;
            i++;
			usleep(1000);
        }
    }
}
