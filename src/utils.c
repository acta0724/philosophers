/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:10:18 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 17:56:17 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	print_action(t_rules *rules, int id, char *msg)
{
	long long	time_ms;

	pthread_mutex_lock(&(rules->print_lock));
	pthread_mutex_lock(&(rules->death_lock));
	if (!rules->died)
	{
		pthread_mutex_unlock(&(rules->death_lock));
		time_ms = get_timestamp() - rules->start_time;
		printf("%lld %d %s\n", time_ms, id, msg);
	}
	else
	{
		pthread_mutex_unlock(&(rules->death_lock));
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
		usleep(1);
	// rules->start_time = get_timestamp();
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

int	create_threads(t_rules *rules, t_philo *philos)
{
	pthread_t	death_thread;
	int			i;
	int			is_ended;

	rules->start_time = get_timestamp() + (rules->num_philo * 50);
	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_create(&(philos[i].thread_id), NULL, \
			philo_routine, &(philos[i])) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&death_thread, NULL, monitor_death, philos) != 0)
		return (1);
	if (pthread_join(death_thread, NULL) != 0)
		return (1);
	pthread_mutex_lock(&(rules->death_lock));
	is_ended = rules->died;
	pthread_mutex_unlock(&(rules->death_lock));
	i = 0;
	if (is_ended == 1)
	{
		while (i < rules->num_philo)
		{
			pthread_detach(philos[i].thread_id);
			i++;
		}
	}
	else
	{
		while (i < rules->num_philo)
		{
			if (pthread_join(philos[i].thread_id, NULL) != 0)
				return (1);
			i++;
		}
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
	pthread_mutex_destroy(&(rules->finish_lock));
	pthread_mutex_destroy(&(rules->death_lock));
	pthread_mutex_destroy(&(rules->last_eat_lock));
	pthread_mutex_destroy(&(rules->eat_count_lock));
	free(rules->forks);
	free(philos);
	return (0);
}
