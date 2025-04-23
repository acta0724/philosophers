/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:09:35 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/23 21:46:48 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	philo_actions(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&(rules->death_lock));
	if (rules->died)
	{
		pthread_mutex_unlock(&(rules->death_lock));
		return (1);
	}
	pthread_mutex_unlock(&(rules->death_lock));
	if (eat(philo))
		return (1);
	if (rules->must_eat_count > 0 && \
		philo->eat_count >= rules->must_eat_count)
	{
		pthread_mutex_lock(&(rules->finish_lock));
		philo->finished = 1;
		pthread_mutex_unlock(&(rules->finish_lock));
		return (1);
	}
	philo_sleep(philo);
	think(philo);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	smart_sleep(rules->start_time - get_timestamp(), rules);
	if (philo->id % 2 == 0)
	{
		start_sleep(rules->time_to_eat, rules);
	}
	while (1)
	{
		if (philo_actions(philo))
			return (NULL);
	}
	return (NULL);
}
