/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:36:50 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/23 00:41:28 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	philo_sleep(t_philo *philo)
{
	print_action(philo->rules, philo->id, "is sleeping");
	usleep(philo->rules->time_to_sleep * 1000);
}

void	start_sleep(long long time_in_ms, t_rules *rules)
{
	while (1)
	{
		if (get_timestamp() - rules->start_time > time_in_ms)
		{
			break ;
		}
		else
		{
			usleep(100);
		}
	}
}

void	smart_sleep(long long time_in_ms, t_rules *rules)
{
	long long	start;
	int			i;

	start = get_timestamp();
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&(rules->death_lock));
		if (rules->died && rules->num_philo != 1)
		{
			pthread_mutex_unlock(&(rules->death_lock));
			break ;
		}
		pthread_mutex_unlock(&(rules->death_lock));
		if (get_timestamp() - start >= time_in_ms)
		{
			break ;
		}
		else
		{
			usleep(100);
		}
	}
}
