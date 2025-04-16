/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:10:18 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/17 01:33:44 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_action(t_rules *rules, int id, char *msg)
{
	long long	time_ms;

	pthread_mutex_lock(&(rules->print_lock));
	if (!rules->died)
	{
		time_ms = get_timestamp() - rules->start_time;
		printf("%lld %d %s\n", time_ms, id, msg);
	}
	pthread_mutex_unlock(&(rules->print_lock));
}

static void	*monitor_death(void *arg)
{
	t_philo	*philos;
	t_rules	*rules;
	int		i;
	int		finish;

	philos = (t_philo *)arg;
	rules = philos[0].rules;
	while (!rules->died)
	{
		i = 0;
		while (i < rules->num_philo && !rules->died)
		{
			pthread_mutex_lock(&(rules->print_lock));
			if (get_timestamp() - philos[i].last_eat_time >
			    rules->time_to_die)
			{
				rules->died = 1;
				printf("%lld %d died\n",
				get_timestamp() - rules->start_time, philos[i].id);
			}
			pthread_mutex_unlock(&(rules->print_lock));
			i++;
		}
		if (rules->must_eat_count > 0)
		{
			finish = 0;
			i = 0;
			while (i < rules->num_philo)
			{
				if (philos[i].eat_count >= rules->must_eat_count)
					finish++;
				i++;
			}
			if (finish == rules->num_philo)
				rules->died = 1;
		}
	}
	return ((NULL));
}

int	create_threads(t_rules *rules, t_philo *philos)
{
	pthread_t	death_thread;
	int			i;

	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&(philos[i].thread_id), NULL,
			philo_routine, &(philos[i])) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&death_thread, NULL, monitor_death, philos) != 0)
		return (1);
	pthread_detach(death_thread);
	i = 0;
	while (i < rules->num_philo)
	{
		pthread_join(philos[i].thread_id, NULL);
		i++;
	}
	return (0);
}

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
	free(rules->forks);
	free(philos);
	return (0);
}
