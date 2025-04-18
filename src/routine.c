/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:09:35 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 08:57:30 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static void	take_forks(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&(rules->forks[philo->left_fork]));
		print_action(rules, philo->id, "has taken a fork");
		pthread_mutex_lock(&(rules->forks[philo->right_fork]));
		print_action(rules, philo->id, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&(rules->forks[philo->right_fork]));
		print_action(rules, philo->id, "has taken a fork");
		pthread_mutex_lock(&(rules->forks[philo->left_fork]));
		print_action(rules, philo->id, "has taken a fork");
	}
}

static void	eat(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	take_forks(philo);
	pthread_mutex_lock(&(rules->last_eat_lock));
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(&(rules->last_eat_lock));
	print_action(rules, philo->id, "is eating");
	eating_sleep(rules->time_to_eat, rules);
	pthread_mutex_lock(&(rules->eat_count_lock));
	philo->eat_count++;
	pthread_mutex_unlock(&(rules->eat_count_lock));
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
	long long	start;

	start = get_timestamp();
	print_action(philo->rules, philo->id, "is thinking");
	if (philo->rules->time_to_die - (philo->rules->time_to_eat * 2) - (philo->rules->time_to_sleep) > 0)
	{
		while (1)
		{
			if (get_timestamp() - start >= philo->rules->time_to_die - \
				(philo->rules->time_to_eat * 2) - (philo->rules->time_to_sleep))
				break ;
		}
	}
}

static void	start_sleep(long long time_in_ms, t_rules *rules)
{
	while (1)
	{
		if (get_timestamp() - rules->start_time > time_in_ms)
			break ;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_rules	*rules;

	philo = (t_philo *)arg;
	rules = philo->rules;
	while (get_timestamp() < rules->start_time)
		usleep(3);
	rules->start_time = get_timestamp();
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
			break ;
		}
		pthread_mutex_unlock(&(rules->death_lock));
		eat(philo);
		if (rules->must_eat_count > 0 && \
			philo->eat_count >= rules->must_eat_count)
		{
			pthread_mutex_lock(&(rules->finish_lock));
			philo->finished = 1;
			pthread_mutex_unlock(&(rules->finish_lock));
			break ;
		}
		philo_sleep(philo);
		think(philo);
	}
	return (NULL);
}
