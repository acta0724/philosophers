/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:09:35 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/19 21:49:06 by iwasakatsuy      ###   ########.fr       */
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
	pthread_mutex_lock(&(rules->last_eat_lock));
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(&(rules->last_eat_lock));
	// if (philo->id % 2 == 0)
	// 	start_sleep(rules->time_to_eat, rules);
	while (1)
	{
		if (philo_actions(philo))
			return (NULL);
	}
	return (NULL);
}
