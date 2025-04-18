/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:10:18 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:46:13 by iwasakatsuy      ###   ########.fr       */
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

long long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (sign == 1 && result > (LONG_MAX - (str[i] - '0')) / 10)
			return ((int)LONG_MAX);
		else if (sign == -1 && result > (LONG_MAX - (str[i] - '0')) / 10)
			return ((int)LONG_MIN);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(result * sign));
}
