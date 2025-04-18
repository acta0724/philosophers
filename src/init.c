/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:07:42 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 08:57:04 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

static int	ft_atoi(const char *str)
{
	long	num;
	int		sign;
	int		i;

	num = 0;
	sign = 1;
	i = 0;
	while ((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	return ((int)(num * sign));
}

static int	parse_args(int argc, char **argv, t_rules *rules)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
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

int	init_all(int argc, char **argv, t_rules *rules, t_philo **philos)
{
	int	i;

	if (parse_args(argc, argv, rules))
	{
		printf("Error in arguments.\n");
		return (1);
	}
	rules->died = 0;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->num_philo);
	if (!rules->forks)
		return ((1));
	i = 0;
	while (i < rules->num_philo)
	{
		pthread_mutex_init(&(rules->forks[i]), NULL);
		i++;
	}
	pthread_mutex_init(&(rules->print_lock), NULL);
	pthread_mutex_init(&(rules->finish_lock), NULL);
	pthread_mutex_init(&(rules->death_lock), NULL);
	pthread_mutex_init(&(rules->last_eat_lock), NULL);
	pthread_mutex_init(&(rules->eat_count_lock), NULL);
	*philos = malloc(sizeof(t_philo) * rules->num_philo);
	if (!(*philos))
		return (1);
	i = 0;
	while (i < rules->num_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].eat_count = 0;
		(*philos)[i].last_eat_time = 0;
		(*philos)[i].rules = rules;
		(*philos)[i].finished = 0;
		(*philos)[i].left_fork = i;
		(*philos)[i].right_fork = (i + 1) % rules->num_philo;
		i++;
	}
	rules->start_time = 0;
	return (0);
}

long long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	smart_sleep(long long time_in_ms, t_rules *rules)
{
	long long	start;

	start = get_timestamp();
	while (1)
	{
		pthread_mutex_lock(&(rules->death_lock));
		if (rules->died)
		{
			pthread_mutex_unlock(&(rules->death_lock));
			break ;
		}
		pthread_mutex_unlock(&(rules->death_lock));
		if (get_timestamp() - start >= time_in_ms)
			break ;
	}
}

void	eating_sleep(long long time_in_ms, t_rules *rules)
{
	long long	start;

	start = get_timestamp();
	while (1)
	{
		pthread_mutex_lock(&(rules->death_lock));
		if (rules->died)
		{
			pthread_mutex_unlock(&(rules->death_lock));
			break ;
		}
		pthread_mutex_unlock(&(rules->death_lock));
		if (get_timestamp() - start >= time_in_ms)
			break ;
	}
}
