/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:09:35 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/17 02:33:55 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	take_forks(t_philo *philo)
{
	t_rules	*rules;
	int		left;
	int		right;

	rules = philo->rules;
	left = philo->id - 1;
	right = philo->id % rules->num_philo;
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&(rules->forks[left]));
		print_action(rules, philo->id, "has taken a fork");
		pthread_mutex_lock(&(rules->forks[right]));
		print_action(rules, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(rules->forks[right]));
		print_action(rules, philo->id, "has taken a fork");
		pthread_mutex_lock(&(rules->forks[left]));
		print_action(rules, philo->id, "has taken a fork");
	}
}

static void	eat(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	take_forks(philo);
	philo->last_eat_time = get_timestamp();
	print_action(rules, philo->id, "is eating");
	smart_sleep(rules->time_to_eat, rules);
	philo->eat_count++;
	pthread_mutex_unlock(&(rules->forks[philo->id - 1]));
	pthread_mutex_unlock(&(rules->forks[philo->id % rules->num_philo]));
}

static void	philo_sleep(t_philo *philo)
{
	print_action(philo->rules, philo->id, "is sleeping");
	smart_sleep(philo->rules->time_to_sleep, philo->rules);
}

static void	think(t_philo *philo)
{
	print_action(philo->rules, philo->id, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	philo->last_eat_time = get_timestamp();
	while (!(rules->died))
	{
		eat(philo);
		if (rules->must_eat_count > 0 && \
			philo->eat_count >= rules->must_eat_count)
			break ;
		philo_sleep(philo);
		think(philo);
	}
	return ((NULL));
}
