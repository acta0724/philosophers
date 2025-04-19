/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:34:57 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/19 22:07:54 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	lonely_philo(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	print_action(rules, philo->id, "has taken a fork");
	usleep(rules->time_to_die * 1000);
	pthread_mutex_lock(&(rules->print_lock));
	pthread_mutex_lock(&(rules->death_lock));
	rules->died = 1;
	pthread_mutex_unlock(&(rules->death_lock));
	printf("%lld %d died\n", \
		get_timestamp() - rules->start_time, philo->id);
	pthread_mutex_unlock(&(rules->print_lock));
	return (1);
}

int	take_forks(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	if (rules->num_philo == 1 && lonely_philo(philo))
		return (1);
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
	return (0);
}

int	eat(t_philo *philo)
{
	t_rules	*rules;

	rules = philo->rules;
	if (take_forks(philo))
		return (1);
	pthread_mutex_lock(&(rules->last_eat_lock));
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(&(rules->last_eat_lock));
	print_action(rules, philo->id, "is eating");
	usleep(rules->time_to_eat * 1000);
	pthread_mutex_lock(&(rules->eat_count_lock));
	philo->eat_count++;
	pthread_mutex_unlock(&(rules->eat_count_lock));
	pthread_mutex_unlock(&(rules->forks[philo->id - 1]));
	pthread_mutex_unlock(&(rules->forks[philo->id % rules->num_philo]));
	return (0);
}
