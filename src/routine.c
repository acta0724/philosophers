/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:09:35 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 21:14:31 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	pthread_mutex_lock(&(rules->last_eat_lock));
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(&(rules->last_eat_lock));
	if (philo->id % 2 == 0)
		start_sleep(rules->time_to_eat, rules);
	while (1)
	{
		pthread_mutex_lock(&(rules->death_lock));
		if (rules->died)
		{
			pthread_mutex_unlock(&(rules->death_lock));
			return (NULL);
		}
		pthread_mutex_unlock(&(rules->death_lock));
		eat(philo);
		if (rules->must_eat_count > 0 && \
			philo->eat_count >= rules->must_eat_count)
		{
			pthread_mutex_lock(&(rules->finish_lock));
			philo->finished = 1;
			pthread_mutex_unlock(&(rules->finish_lock));
			return (NULL);
		}
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}
