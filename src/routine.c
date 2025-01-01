/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/01 20:53:38 by armitite          #+#    #+#             */
/*   Updated: 2025/01/01 21:21:52 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleep_n_think(t_philo *p)
{
	print_message(p, "is sleeping");
	ft_usleep(p->data->sleeping_t);
	print_message(p, "is thinking");
}

void    take_forks(t_philo *p)
{
    if (p->n % 2 == 0)
	{
		pthread_mutex_lock(p->left_fork);
		print_message(p, "is taking fork");
		pthread_mutex_lock(p->right_fork);
		print_message(p, "is taking fork");
	}
	else
	{
		pthread_mutex_lock(p->right_fork);
		print_message(p, "is taking fork");
		pthread_mutex_lock(p->left_fork);
		print_message(p, "is taking fork");
	}
}

void	eat(t_philo	*p)
{
    take_forks(p);
	print_message(p, "is eating");
	pthread_mutex_lock(&p->data->mutex_print);
    if (p->data->meals_eaten == (p->data->meals_nbr * p->data->p_total))
	{
		pthread_mutex_unlock(&p->data->mutex_print);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return ;
	}
	p->data->meals_eaten++;
	p->t_last_meal = time_now();
	pthread_mutex_unlock(&p->data->mutex_print);
	ft_usleep(p->data->eating_t);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
}
int    routine_utils(t_philo *p)
{
    pthread_mutex_lock(&p->data->mutex_data);
	p->time = time_now();
	p->t_last_meal = time_now();
	pthread_mutex_unlock(&p->data->mutex_data);
	if (p->n % 2 == 0)
	    ft_usleep(150);
    if (p->data->p_total == 1)
	{
		print_message(p, "is taking fork");
    	ft_usleep(p->data->starving_t);
		return (1);
	}
	return (0);
}

void    *routine(void *arg)
{
	t_philo *p = (t_philo *)arg;
    if (routine_utils(p) == 1)
		return (NULL);
	while (1)
	{
        pthread_mutex_lock(&p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(&p->data->mutex_print), NULL);
        pthread_mutex_unlock(&p->data->mutex_print);
		eat(p);
		pthread_mutex_lock(&p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(&p->data->mutex_print), NULL);
        pthread_mutex_unlock(&p->data->mutex_print);
		sleep_n_think(p);
		pthread_mutex_lock(&p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(&p->data->mutex_print), NULL);
        pthread_mutex_unlock(&p->data->mutex_print);
	}
	return NULL;
}