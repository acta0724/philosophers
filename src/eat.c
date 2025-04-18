/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:34:57 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:36:14 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	take_forks(t_philo *philo)
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

void	eat(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	take_forks(philo);
	pthread_mutex_lock(&(rules->last_eat_lock));
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(&(rules->last_eat_lock));
	print_action(rules, philo->id, "is eating");
	smart_sleep(rules->time_to_eat, rules);
	pthread_mutex_lock(&(rules->eat_count_lock));
	philo->eat_count++;
	pthread_mutex_unlock(&(rules->eat_count_lock));
	pthread_mutex_unlock(&(rules->forks[philo->id - 1]));
	pthread_mutex_unlock(&(rules->forks[philo->id % rules->num_philo]));
}
