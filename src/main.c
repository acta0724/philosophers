/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 23:27:20 by kiwasa            #+#    #+#             */
/*   Updated: 2025/04/17 01:42:23 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

int	main(int argc, char **argv)
{
	t_rules		rules;
	t_philo		*philos;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (init_all(argc, argv, &rules, &philos))
		return (1);
	if (create_threads(&rules, philos))
		return (1);
	destroy_all(&rules, philos);
	return (0);
}
