/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armitite <armitite@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 19:15:07 by armitite          #+#    #+#             */
/*   Updated: 2024/12/24 17:35:21 by armitite         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_message(t_philo *p, char *str)
{
	pthread_mutex_lock(p->data->mutex_print);
    pthread_mutex_lock(p->data->mutex_data);
    int meals_check = (p->data->meals_eaten == (p->data->meals_nbr * p->data->p_total));
    int death_check = p->data->death;
    pthread_mutex_unlock(p->data->mutex_data);
    if (meals_check)
    {
        pthread_mutex_unlock(p->data->mutex_print);
        return;
    }
    if (death_check != 1)
    {
        printf("%lld %d %s\n", time_now() - p->time, p->n, str);
    }
    pthread_mutex_unlock(p->data->mutex_print);
}

void	sleep_n_think(t_philo *p)
{
	print_message(p, "is sleeping");
	ft_usleep(p->data->sleeping_t);
	print_message(p, "is thinking");
}

void	eat(t_philo	*p)
{
	pthread_mutex_lock(p->left_fork);
	print_message(p, "is taking fork");
	pthread_mutex_lock(p->right_fork);
	print_message(p, "is taking fork");
	print_message(p, "is eating");
	pthread_mutex_lock(p->data->mutex_print);
    if (p->data->meals_eaten == (p->data->meals_nbr * p->data->p_total))
	{
		pthread_mutex_unlock(p->data->mutex_print);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		return ;
	}
	p->data->meals_eaten++;
	p->t_last_meal = time_now();
	pthread_mutex_unlock(p->data->mutex_print);
	ft_usleep(p->data->eating_t);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
}
void	*monitoring(void *arg)
{
	t_philo **p = (t_philo **) arg;
    t_data  *data;
    int i;
	long long last_meal;

    data = malloc(sizeof(t_data) * 1);
    data = p[0]->data;
    ft_usleep(150);
    while (1)
    {
        i = 0;
        while (i < data->p_total)
        {
            pthread_mutex_lock(data->mutex_print);
			pthread_mutex_lock(data->mutex_data);
			last_meal = p[i]->t_last_meal;
			pthread_mutex_unlock(data->mutex_data);
            if ((time_now() - last_meal) > data->starving_t)
            {
                data->death = 1;
                printf("%lld %d is dead\n", (time_now() - p[i]->time), p[i]->n);
                pthread_mutex_unlock(data->mutex_print);
                return (NULL);
            }
            if (data->meals_eaten == (data->meals_nbr * data->p_total))
            {
				data->death = 1;
                printf("%d meals have been eaten\n", data->meals_eaten);
                pthread_mutex_unlock(data->mutex_print);
                return (NULL);
            }
            pthread_mutex_unlock(data->mutex_print);
            i++;
			usleep(1000);
        }
    }
}

void    *routine(void *arg)
{
	t_philo *p = (t_philo *)arg;
	pthread_mutex_lock(p->data->mutex_data);
	p->time = time_now();
	p->t_last_meal = time_now();
	pthread_mutex_unlock(p->data->mutex_data);
	if (p->n % 2 == 0)
	    ft_usleep(150);
	while (1)
	{
        pthread_mutex_lock(p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(p->data->mutex_print), NULL);
        pthread_mutex_unlock(p->data->mutex_print);
		eat(p);
		pthread_mutex_lock(p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(p->data->mutex_print), NULL);
        pthread_mutex_unlock(p->data->mutex_print);
		sleep_n_think(p);
		pthread_mutex_lock(p->data->mutex_print);
		if (p->data->death == 1)
			return (pthread_mutex_unlock(p->data->mutex_print), NULL);
        pthread_mutex_unlock(p->data->mutex_print);
	}
	return NULL;
}

int create_philo(t_philo **philo, t_data *data)
{
    int i;
    pthread_t th[data->p_total];
    pthread_t monitor;

    i = 0;
	data->time = time_now();
    while (i < data->p_total) 
    {
        if (pthread_create(th + i, NULL, &routine, (void *)philo[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
		//philo[i]->time = time_now();
        i++;
    }
    if (pthread_create(&monitor, NULL, &monitoring, (void *)philo) != 0) 
    {
        perror("Failed to create thread");
        return 1;
    }
    i = 0;
    while (i < data->p_total) 
    {
        if (pthread_join(th[i], NULL) != 0) {
            return 3;
        }
        //printf("Thread %d has finished execution\n", i);
        i++;
    }
    if (pthread_join(monitor, NULL) != 0) 
    {
        return 2;
    }
    i = 0;
    while (i < data->p_total)
    {
        pthread_mutex_destroy(&data->mutex_forks[i]);
        i++;
    }
    i = 0;
    while (i < data->p_total)
    {
        //free(philo[i]->data);
        free(philo[i]);
        i++;
    }
    free(philo);
    //free(data);
    pthread_mutex_destroy(data->mutex_print);
	pthread_mutex_destroy(data->mutex_data);
	free(data->mutex_data);
	free(data->mutex_print);
    return (0);
}