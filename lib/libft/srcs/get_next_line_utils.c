/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:08:17 by yachtata          #+#    #+#             */
/*   Updated: 2023/12/24 14:26:17 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	int				i;
	unsigned char	b;

	i = 0;
	b = (unsigned char)c;
	while (s[i] != '\0')
	{
		if (s[i] == b)
			return ((char *)s + i);
		i++;
	}
	if (s[i] == b)
		return ((char *)s + i);
	return (0);
}

char	*ft_strdup(const char *src)
{
	size_t	len;
	char	*ptr;
	size_t	i;

	i = 0;
	len = 0;
	while (src[len])
		len++;
	ptr = (char *)malloc((1 + len) * sizeof(char));
	if (!ptr)
		return (0);
	while (src[i] != '\0')
	{
		ptr[i] = src[i];
		++i;
	}
	ptr[i] = '\0';
	return (ptr);
}

int	ft_lstadd_back(t_get **lst, char *content)
{
	t_get	*crnt;
	t_get	*prev;
	t_get	*creat;

	if (!lst || !content)
		return (0);
	creat = (t_get *)malloc(sizeof(t_get));
	if (!creat)
		return (0);
	creat->content = content;
	creat->next = NULL;
	if (!*lst)
	{
		*lst = creat;
		return (1);
	}
	crnt = *lst;
	while (crnt)
	{
		prev = crnt;
		crnt = crnt->next;
	}
	prev->next = creat;
	return (1);
}

void	ft_lstclear(t_get **lst)
{
	t_get	*temp;

	if (!lst || !*lst)
		return ;
	while (*lst != NULL)
	{
		temp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
}

int	check_newline(t_get *list)
{
	int	i;

	i = 0;
	if (!list)
		return (0);
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			if (list->content[i] == '\n')
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}
