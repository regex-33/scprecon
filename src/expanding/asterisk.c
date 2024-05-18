#include "minishell.h"

/*  check match filename    */
// mi*shell
int	match_wildcard(const char *pattern, const char *filename)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			/* '*' at the end matches any remaining characters */
			if (*pattern == '\0')
				return (1);
			while (*filename)
			{
				if (match_wildcard(pattern, filename))
					return (1);
				filename++;
			}
			return (0);
		}
		else if (*filename == '\0' || (*pattern != '?'
				&& *pattern != *filename)) // chars between * and your char
			return (0);
		pattern++;
		filename++;
	}
	return (*filename == '\0' && *pattern == '\0');
}
/*
The	opendir(void) function in C is used to open a directory stream corresponding
to the directory named by a given path. It returns a pointer to a DIR stream,
which can then be used to read the contents of the directory using functions
like	readdir(void). This function is part of the <dirent.h> header and is commonly
 used for directory traversal and file listing operations.*/


int	move_temp_list_to_list(t_list **list, t_list **temp)
{
	t_list	*new;
	char	*str;
	t_list *head;

	head = *temp;
	merge_sort_linkedlist(temp);
	while (*temp != NULL)
	{
		str = ft_strdup((*temp)->content);
		if (!str)
			return (freeLinkedList(head), perror("minishell"), 0);
		new = ft_lstnew(str);
		if (!new)
			return (freeLinkedList(head), perror("minishell"), 0);
		ft_lstadd_back_libft(list, new);
		*temp = (*temp)->next;
	}
	return (freeLinkedList(head), 1);
}

int	expand_wildcard_add_node(t_list **temp, const char *pattern, const char *entry_name)
{
	t_list	*new;
	char	*str;

	if (pattern[0] != '.' && entry_name[0] == '.')
		return (1);
	str = ft_strdup(entry_name);
	if (!str)
		return (perror("minishell"), 0);
	new = ft_lstnew(str);
	if (!new)
		return (perror("minishell"), 0);
	ft_lstadd_back_libft(temp, new);
	return (1);
}

int	expand_wildcard(const char *pattern, t_list **matches)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*temp = NULL;
	t_list			*new;

	dir = opendir(".");
	if (dir == NULL)
		return (0);
	while ((entry = readdir(dir)) != NULL)
	{
		if (match_wildcard(pattern, entry->d_name))
		{
			if (!expand_wildcard_add_node(&temp, pattern, entry->d_name))
				return (freeLinkedList(temp), closedir(dir), 0);
		}
	}
	if (!temp)
	{
		new = ft_lstnew(ft_strdup(pattern));
		if (!new)
			return (freeLinkedList(temp), closedir(dir), 0);
		ft_lstadd_back_libft(&temp, new);
	}
	return (closedir(dir), move_temp_list_to_list(matches, &temp));
}

void	*expand_asterisk(char *command, t_list **list)
{
	char	**token;
	t_list	*new;
	int		i;

	i = 0;
	token = ft_split(command, ' ');
	while (token[i])
	{
		if (ft_strchr(token[i], '*'))
		{
			if(!expand_wildcard(token[i], list))
				return (free_array(token), free(command), perror("minishell"), NULL);
		}
		else
		{
			new = ft_lstnew(ft_strdup(token[i]));
			if (!new)
				return (free_array(token), free(command), perror("minishell"), NULL);
			ft_lstadd_back_libft(list, new);
		}
		i++;
	}
	return (free_array(token), free(command), list);
}
