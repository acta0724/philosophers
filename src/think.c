/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:41:44 by iwasakatsuy       #+#    #+#             */
/*   Updated: 2025/04/18 18:42:09 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

void	think(t_philo *philo)
{
	long long	start;

	start = get_timestamp();
	print_action(philo->rules, philo->id, "is thinking");
	if (philo->rules->time_to_die - (philo->rules->time_to_eat * 2) - \
			(philo->rules->time_to_sleep) > 0)
	{
		while (1)
		{
			if (get_timestamp() - start >= philo->rules->time_to_die - \
				(philo->rules->time_to_eat * 2) - (philo->rules->time_to_sleep))
				break ;
		}
	}
}
