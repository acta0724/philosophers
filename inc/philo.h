/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iwasakatsuya <iwasakatsuya@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 23:27:03 by kiwasa            #+#    #+#             */
/*   Updated: 2025/04/18 18:54:52 by iwasakatsuy      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <limits.h>

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
	pthread_mutex_t	finish_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	last_eat_lock;
	pthread_mutex_t	eat_count_lock;
	long long		start_time;
}				t_rules;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	int				finished;
	long long		last_eat_time;
	int				left_fork;
	int				right_fork;
	t_rules			*rules;
	pthread_t		thread_id;
}				t_philo;

// --- libft.c ---
int			ft_atoi(const char *str);

// --- eat.c ---
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);

// --- sleep.c ---
void		philo_sleep(t_philo *philo);
void		start_sleep(long long time_in_ms, t_rules *rules);
void		smart_sleep(long long time_in_ms, t_rules *rules);

// --- routine.c ---
void		*philo_routine(void *arg);

// --- think.c ---
void		think(t_philo *philo);

// --- monitor_death.c ---
void		*monitor_death(void *arg);

// --- utils.c ---
void		print_action(t_rules *rules, int id, char *msg);
long long	get_timestamp(void);
int			ft_isdigit(int c);
int			args_isdigit(char **argv);


// --- init.c ---
int			init_all(int argc, char **argv, t_rules *rules, t_philo **philos);
void		smart_sleep(long long time_in_ms, t_rules *rules);
int			create_threads(t_rules *rules, t_philo *philos);

// --- destroy_all.c ---
int			destroy_all(t_rules *rules, t_philo *philos);

#endif