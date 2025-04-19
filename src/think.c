/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:41:44 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/19 21:54:38 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	think(t_philo *philo)
{
	long long	think_time;

	think_time = philo->rules->time_to_die - (philo->rules->time_to_eat * 2) - \
		(philo->rules->time_to_sleep);
	print_action(philo->rules, philo->id, "is thinking");
	if (think_time > 0)
	{
		usleep(think_time * 1000);
	}
}
