/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:08:09 by yachtata          #+#    #+#             */
/*   Updated: 2023/12/24 14:26:06 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	create_nodes(t_get **list, int fd)
{
	char	*buffer;
	int		bytsread;

	if (!list || fd < 0)
		return (0);
	if (check_newline(*list))
		return (1);
	while (1)
	{
		buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (!buffer)
			return (0);
		bytsread = read(fd, buffer, BUFFER_SIZE);
		if (bytsread < 0)
		{
			free(buffer);
			return (0);
		}
		buffer[bytsread] = '\0';
		ft_lstadd_back(list, buffer);
		if (bytsread < BUFFER_SIZE || ft_strchr(buffer, '\n'))
			break ;
	}
	return (1);
}

int	len_to_newline(t_get *list)
{
	int	len;
	int	i;

	len = 0;
	if (!list)
		return (0);
	while (list)
	{
		i = 0;
		while (list->content[i])
		{
			len++;
			if (list->content[i] == '\n')
				return (len);
			i++;
		}
		list = list->next;
	}
	return (len);
}

char	*take_line(t_get *line)
{
	int		len;
	char	*buffer;
	int		i;

	len = len_to_newline(line);
	if (len <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (len + 2));
	if (!buffer)
		return (NULL);
	len = 0;
	while (line)
	{
		i = 0;
		while (line->content[i] && line->content[i] != '\n')
			buffer[len++] = line->content[i++];
		if (line->content[i] == '\n')
		{
			buffer[len++] = '\n';
			break ;
		}
		line = line->next;
	}
	buffer[len] = '\0';
	return (buffer);
}

char	*allocate_after_newline(t_get *list)
{
	int		i;
	t_get	*last;
	char	*temp;

	temp = NULL;
	while (list != NULL)
	{
		if (list->next == NULL)
			break ;
		list = list->next;
	}
	last = list;
	i = 0;
	while (last->content[i])
	{
		if (last->content[i] == '\n')
		{
			temp = ft_strdup(last->content + i + 1);
			return (temp);
		}
		i++;
	}
	return (temp);
}

char	*get_next_line(int fd)
{
	static t_get	*head = NULL;
	char			*last;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1024)
		return (NULL);
	if (read(fd, &line, 0) < 0)
	{
		ft_lstclear(&head);
		return (NULL);
	}
	create_nodes(&head, fd);
	line = take_line(head);
	if (!line)
	{
		ft_lstclear(&head);
		return (NULL);
	}
	last = allocate_after_newline(head);
	ft_lstclear(&head);
	ft_lstadd_back(&head, last);
	return (line);
}
