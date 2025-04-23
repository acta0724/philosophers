/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:07:42 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/23 21:46:31 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	parse_args(int argc, char **argv, t_rules *rules)
{
	if (args_isdigit(argv))
		return (1);
	rules->num_philo = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->must_eat_count = -1;
	if (argc == 6)
		rules->must_eat_count = ft_atoi(argv[5]);
	if (rules->num_philo <= 0 || rules->time_to_die < 0 || \
		rules->time_to_eat < 0 || rules->time_to_sleep < 0 || \
		(argc == 6 && rules->must_eat_count <= 0))
		return (1);
	return (0);
}

static int	init_mutex(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		if (pthread_mutex_init(&(rules->forks[i]), NULL) != 0)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&(rules->print_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(rules->finish_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(rules->death_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(rules->last_eat_lock), NULL) != 0)
		return (1);
	if (pthread_mutex_init(&(rules->eat_count_lock), NULL) != 0)
		return (1);
	return (0);
}

static void	init_philos(t_philo *philos, t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->num_philo)
	{
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].last_eat_time = rules->start_time;
		philos[i].rules = rules;
		philos[i].finished = 0;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1) % rules->num_philo;
		i++;
	}
}

int	init_all(int argc, char **argv, t_rules *rules, t_philo **philos)
{
	if (parse_args(argc, argv, rules))
	{
		printf("Error in arguments.\n");
		return (1);
	}
	rules->died = 0;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philo);
	if (!rules->forks)
		return ((1));
	if (init_mutex(rules))
	{
		free(rules->forks);
		return (1);
	}
	*philos = malloc(sizeof(t_philo) * rules->num_philo);
	if (!(*philos))
		return (1);
	rules->start_time = get_timestamp();
	printf("start_time : %lld\n", rules->start_time);
	rules->start_time = get_timestamp() + (rules->num_philo * 10);
	printf("start_time : %lld\n", rules->start_time);
	init_philos(*philos, rules);
	return (0);
}
