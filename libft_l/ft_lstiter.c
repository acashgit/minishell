/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjodge <mjodge@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/13 20:51:49 by acash             #+#    #+#             */
/*   Updated: 2022/01/18 14:55:49 by mjodge           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_vars *lst, void (*f)(void *))
{
	if (!f)
		return ;
	while (lst)
	{
		f(lst->var);
		lst = lst->next;
	}
}
