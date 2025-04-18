/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:43:26 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 20:13:27 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int check_finish(t_philo *philos, t_rules *rules)
{
	int	i;
	int finish;

	i = 0;
	finish = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_lock(&(rules->eat_count_lock));
		if (philos[i].eat_count >= rules->must_eat_count)
			finish++;
		pthread_mutex_unlock(&(rules->eat_count_lock));
		i++;
	}
	if (finish == rules->num_philo)
	{
		pthread_mutex_lock(&(rules->death_lock));
		rules->died = 2;
		pthread_mutex_unlock(&(rules->death_lock));
		return (1);
	}
	return (0);
}

static int check_death(t_philo *philos, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo && !rules->died)
	{
		pthread_mutex_lock(&(rules->last_eat_lock));
		if (get_timestamp() - philos[i].last_eat_time > rules->time_to_die)
		{
			pthread_mutex_unlock(&(rules->last_eat_lock));
			pthread_mutex_lock(&(rules->finish_lock));
			if (philos[i].finished == 0)
			{
				pthread_mutex_unlock(&(rules->finish_lock));
				pthread_mutex_lock(&(rules->death_lock));
				rules->died = 1;
				pthread_mutex_unlock(&(rules->death_lock));
				pthread_mutex_lock(&(rules->print_lock));
				printf("%lld %d died\n", \
					get_timestamp() - rules->start_time, philos[i].id);
				pthread_mutex_unlock(&(rules->print_lock));
				return (1);
			}
			else
				pthread_mutex_unlock(&(rules->finish_lock));
		}
		else
			pthread_mutex_unlock(&(rules->last_eat_lock));
		i++;
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;
	// int		i;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	while (!rules->died)
	{
		// i = 0;
		// while (i < rules->num_philo && !rules->died)
		// {
		// 	pthread_mutex_lock(&(rules->last_eat_lock));
		// 	if (get_timestamp() - philos[i].last_eat_time > \
		// 		rules->time_to_die)
		// 	{
		// 		pthread_mutex_unlock(&(rules->last_eat_lock));
		// 		pthread_mutex_lock(&(rules->finish_lock));
		// 		if (philos[i].finished == 0)
		// 		{
		// 			pthread_mutex_unlock(&(rules->finish_lock));
		// 			pthread_mutex_lock(&(rules->death_lock));
		// 			rules->died = 1;
		// 			pthread_mutex_unlock(&(rules->death_lock));
		// 			pthread_mutex_lock(&(rules->print_lock));
		// 			printf("%lld %d died\n", \
		// 				get_timestamp() - rules->start_time, philos[i].id);
		// 			pthread_mutex_unlock(&(rules->print_lock));
		// 			return (NULL);
		// 		}
		// 		else
		// 			pthread_mutex_unlock(&(rules->finish_lock));
		// 	}
		// 	else
		// 		pthread_mutex_unlock(&(rules->last_eat_lock));
		// 	i++;
		// }
		if (check_death(philos, rules))
			return (NULL);
		if (check_finish(philos, rules))
			return (NULL);
	}
	return (NULL);
}
