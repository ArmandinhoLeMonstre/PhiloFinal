/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 19:15:07 by armitite          #+#    #+#             */
/*   Updated: 2025/01/02 19:51:19 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philo *p, char *str)
{
	int	meals_check;
	int	death_check;

	pthread_mutex_lock(&p->data->mutex_print);
	pthread_mutex_lock(&p->data->mutex_data);
	meals_check = (p->data->meals_eaten == (p->data->meals_nbr
				* p->data->p_total));
	death_check = p->data->death;
	pthread_mutex_unlock(&p->data->mutex_data);
	if (meals_check)
	{
		pthread_mutex_unlock(&p->data->mutex_print);
		return ;
	}
	if (death_check != 1)
	{
		printf("%lld %d %s\n", time_now() - p->time, p->n, str);
	}
	pthread_mutex_unlock(&p->data->mutex_print);
}

void	free_n_destroy(t_philo **philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->p_total)
	{
		free(philo[i]);
		i++;
	}
	i = 0;
	free(philo);
	while (i < data->p_total)
	{
		pthread_mutex_destroy(&data->mutex_forks[i]);
		i++;
	}
	free(data->mutex_forks);
	pthread_mutex_destroy(&data->mutex_print);
	pthread_mutex_destroy(&data->mutex_data);
}

int	join_threads(pthread_t *th, pthread_t monitor, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->p_total)
	{
		if (pthread_join(th[i], NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_join(monitor, NULL) != 0)
		return (1);
	return (0);
}

int	create_philo(t_philo **philo, t_data *data)
{
	pthread_t	*th;
	pthread_t	monitor;
	int			i;

	i = 0;
	th = malloc(sizeof(pthread_t) * data->p_total);
	data->time = time_now();
	while (i < data->p_total)
	{
		if (pthread_create(th + i, NULL, &routine, (void *)philo[i]) != 0)
		{
			perror("Failed to create thread");
			return (1);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, &monitoring, (void *)philo) != 0)
	{
		perror("Failed to create thread");
		return (1);
	}
	join_threads(th, monitor, data);
	free(th);
	free_n_destroy(philo, data);
	return (0);
}
