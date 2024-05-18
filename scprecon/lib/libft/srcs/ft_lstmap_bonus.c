/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 15:18:41 by yachtata          #+#    #+#             */
/*   Updated: 2023/12/24 14:26:43 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static t_list	*m(t_list *l, void *(*f)(void *), void (*d)(void *), t_list **t)
{
	t_list	*curr;
	t_list	*head;
	t_list	*saver;

	head = *t;
	while (l != NULL)
	{
		curr = f(l->content);
		saver = ft_lstnew(curr);
		if (!saver)
		{
			if (d && curr)
				d(curr);
			ft_lstclear_libft(&head, d);
			return (NULL);
		}
		(*t)->next = saver;
		l = l->next;
		*t = (*t)->next;
	}
	return (head);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*temp;
	void	*curr;

	if (!lst || !f)
		return (NULL);
	curr = f(lst->content);
	temp = ft_lstnew(curr);
	if (!temp)
	{
		if (del && curr)
			del(curr);
		return (NULL);
	}
	lst = lst->next;
	return (m(lst, f, del, &temp));
}
