/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_theaths.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:44:40 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:44:48 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	create_threads(t_rules *rules, t_philo *philos)
{
	pthread_t	death_thread;
	int			i;
	int			is_ended;

	rules->start_time = get_timestamp() + (rules->num_philo * 50);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&(philos[i].thread_id), NULL, \
			philo_routine, &(philos[i])) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&death_thread, NULL, monitor_death, philos) != 0)
		return (1);
	if (pthread_join(death_thread, NULL) != 0)
		return (1);
	pthread_mutex_lock(&(rules->death_lock));
	is_ended = rules->died;
	pthread_mutex_unlock(&(rules->death_lock));
	i = 0;
	if (is_ended == 1)
	{
		while (i < rules->num_philo)
		{
			pthread_detach(philos[i].thread_id);
			i++;
		}
	}
	else
	{
		while (i < rules->num_philo)
		{
			if (pthread_join(philos[i].thread_id, NULL) != 0)
				return (1);
			i++;
		}
	}
	return (0);
}
