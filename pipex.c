/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acash <acash@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 16:07:49 by mjodge            #+#    #+#             */
/*   Updated: 2022/01/30 20:20:40 by acash            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_and_chain(int *lpipe, int *rpipe, char **cmd, t_infor *st)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		exit(1);
	else if (!pid)
	{
		signal(SIGINT, sig_void2);
		rl_catch_signals = 1;
		do_redir_left(cmd, lpipe, st);
		signal(SIGINT, sig_handler);
		do_redir_right(cmd, rpipe, st);
		do_command(cmd, rpipe, st);
	}
	else
	{
		if (contain(cmd))
			waitpid(pid, NULL, 0);
		else
			waitpid(pid, NULL, WNOHANG);
	}
	st->pip.curr_index++;
	return (pid);
}

int	pipe_cycle(char ***cmd, t_infor *st, int i)
{
	int	pid;

	pipe(st->pip.rpipe);
	pid = fork_and_chain(st->pip.lpipe, st->pip.rpipe, cmd[i], st);
	close(st->pip.lpipe[0]);
	close(st->pip.lpipe[1]);
	st->pip.lpipe[0] = st->pip.rpipe[0];
	st->pip.lpipe[1] = st->pip.rpipe[1];
	i++;
	return (pid);
}

void	if_count_no_one(int i, int flag, char ***cmd, t_infor *st)
{
	if (st->count != 1 && flag == 1)
	{
		st->pip.lpipe[0] = st->pip.rpipe[0];
		st->pip.lpipe[1] = st->pip.rpipe[1];
	}
	if (st->count != 1 && flag == 2)
	{
		st->pid = fork_and_chain(st->pip.lpipe, NULL, cmd[i], st);
		close(st->pip.lpipe[0]);
		close(st->pip.lpipe[1]);
	}
}

void	pipex(char ***cmd, t_infor *st)
{
	int	i;
	int	count;
	int	pid;
	int	status;

	count = get_count_cmd(cmd);
	i = 1;
	st->count = count;
	if (count != 1)
		pipe(st->pip.rpipe);
	pid = fork_and_chain(NULL, st->pip.rpipe, cmd[0], st);
	if_count_no_one(i, 1, cmd, st);
	while (i < count - 1)
		pid = pipe_cycle(cmd, st, i++);
	st->pid = pid;
	if_count_no_one(i, 2, cmd, st);
	waitpid(st->pid, &status, 0);
	ft_convert_error(WEXITSTATUS(status));
	if ((!cmd[++i] && count != 1 && WEXITSTATUS(status) != 2) \
	|| (WEXITSTATUS(status) != 2 && count == 1))
		ft_check_buildins(cmd[count - 1], st, count);
	ft_reinit_pip(st);
}
