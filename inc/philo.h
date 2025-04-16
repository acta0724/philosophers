/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 23:27:03 by kiwasa            #+#    #+#             */
/*   Updated: 2025/04/17 02:33:35 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_rules
{
	int				num_philo;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	int				died;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	long long		start_time;
}				t_rules;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	long long		last_eat_time;
	t_rules			*rules;
	pthread_t		thread_id;
}				t_philo;

int			init_all(int argc, char **argv, t_rules *rules, t_philo **philos);
long long	get_timestamp(void);
void		smart_sleep(long long time_in_ms, t_rules *rules);
void		*philo_routine(void *arg);
void		print_action(t_rules *rules, int id, char *msg);
int			create_threads(t_rules *rules, t_philo *philos);
int			destroy_all(t_rules *rules, t_philo *philos);

#endif