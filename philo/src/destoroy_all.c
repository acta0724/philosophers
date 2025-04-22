/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destoroy_all.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:23:49 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:24:15 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	destroy_all(t_rules *rules, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_destroy(&(rules->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(rules->print_lock));
	pthread_mutex_destroy(&(rules->finish_lock));
	pthread_mutex_destroy(&(rules->death_lock));
	pthread_mutex_destroy(&(rules->last_eat_lock));
	pthread_mutex_destroy(&(rules->eat_count_lock));
	free(rules->forks);
	free(philos);
	return (0);
}
