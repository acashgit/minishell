/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1export.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acash <acash@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 15:10:42 by mjodge            #+#    #+#             */
/*   Updated: 2022/01/30 19:09:24 by acash            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_print_export(t_infor *st)
{
	t_vars	*dup;

	dup = st->vars;
	while (dup)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putendl_fd(dup->var, 1);
		dup = dup->next;
	}
	return (0);
}

int	ft_export_helper(t_infor *st, char **splited, int i, int len)
{
	t_vars	*dup;

	dup = st->vars;
	while (dup)
	{
		if (splited[i][len] != '=')
			break ;
		if (!ft_strncmp(dup->var, splited[i], len))
		{
			ft_changevar(&splited[i], &(dup), len);
			break ;
		}
		dup = dup->next;
	}
	if (!dup)
		return (0);
	return (1);
}

int	ft_export_print_err(char **splited, int i)
{
	if (!check_name_var(splited[i]))
	{
		ft_putstr_fd("export: ", 2);
		ft_putstr_fd("`", 2);
		ft_putstr_fd(splited[i], 2);
		ft_putstr_fd("': ", 2);
		ft_putstr_fd("not a valid identifier\n", 2);
		ft_myfree(splited);
		return (0);
	}
	return (1);
}

int	ft_export(char *var, t_infor *st)
{
	int		len;
	char	**splited;
	int		i;

	i = 1;
	splited = ft_split_wquots(var);
	len = ft_getlen(splited[i]);
	if (len == 0)
		return (ft_print_export(st));
	while (splited[i])
	{
		if (!ft_export_print_err(splited, i))
			return (1);
		if (!ft_export_helper(st, splited, i, len))
			ft_lstadd_back(&(st->vars), ft_lstnew(ft_strdup(splited[i])));
		i++;
		len = ft_getlen(splited[i]);
	}
	set_envp(st);
	ft_myfree(splited);
	return (0);
}
