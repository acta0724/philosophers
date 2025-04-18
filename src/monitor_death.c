/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_death.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:43:26 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:44:01 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	*monitor_death(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;
	int		i;
	int		finish;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
		usleep(1);
	rules->start_time = get_timestamp();
	usleep(rules->time_to_eat);
	while (!rules->died)
	{
		i = 0;
		while (i < rules->num_philo && !rules->died)
		{
			pthread_mutex_lock(&(rules->last_eat_lock));
			if (get_timestamp() - philos[i].last_eat_time > \
				rules->time_to_die)
			{
				pthread_mutex_lock(&(rules->finish_lock));
				if (philos[i].finished == 0)
				{
					pthread_mutex_lock(&(rules->death_lock));
					rules->died = 1;
					pthread_mutex_unlock(&(rules->death_lock));
					pthread_mutex_lock(&(rules->print_lock));
					printf("%lld %d died\n", \
						get_timestamp() - rules->start_time, philos[i].id);
					pthread_mutex_unlock(&(rules->print_lock));
					pthread_mutex_unlock(&(rules->finish_lock));
					pthread_mutex_unlock(&(rules->last_eat_lock));
					return (NULL);
				}
				pthread_mutex_unlock(&(rules->finish_lock));
			}
			pthread_mutex_unlock(&(rules->last_eat_lock));
			i++;
		}
		if (rules->must_eat_count > 0 && !rules->died)
		{
			finish = 0;
			i = 0;
			while (i < rules->num_philo)
			{
				pthread_mutex_lock(&(rules->finish_lock));
				pthread_mutex_lock(&(rules->eat_count_lock));
				if (philos[i].eat_count >= rules->must_eat_count)
					finish++;
				pthread_mutex_unlock(&(rules->eat_count_lock));
				pthread_mutex_unlock(&(rules->finish_lock));
				i++;
			}
			if (finish == rules->num_philo)
			{
				pthread_mutex_lock(&(rules->print_lock));
				rules->died = 2;
				printf("All philosophers have eaten enough.\n");
				pthread_mutex_unlock(&(rules->print_lock));
				break ;
			}
		}
	}
	return (NULL);
}
