#include "minishell.h"

int	is_valid_chars(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	is_spcial_chars(char c)
{
	return (c != '\0' && (ft_isalpha(c) || c == '_' || c == '?'));
}

char	*get_value(char *name, char **env)
{
	int	i;

	i = 0;
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(get_status(0, 0)));
	while (env && *env)
	{
		i = 0;
		while ((*env)[i] == name[i] && name[i] != '\0')
			i++;
		if ((*env)[i] == '=' && name[i] == '\0')
			return (*env + i + 1);
		env++;
	}
	return (NULL);
}

char	*extract_dollar(const char *str)
{
	const char	*start;
	const char	*end;
	size_t		length;
	char		*substring;

	if (str[0] != '$')
		return ((char *)str);
	start = str;
	end = str + 1;
	while (*end != '\0' && is_valid_chars(*end))
		end++;
	length = end - start;
	substring = (char *)malloc(length + 1);
	if (substring == NULL)
		return (NULL);
	ft_strncpy(substring, start, length);
	substring[length] = '\0';
	return (substring);
}

bool	hasTrailingSpaces(const char *str)
{
	int	len;
	int	i;

	if (!str)
		return (false);
	len = strlen(str);
	if (len == 0)
		return (false);
	i = len - 1;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
		i--;
	return (i < len - 1);
}

int	split_and_add_to_list(t_list **list, t_expanding *expanding, char *value)
{
	char	**value_split;
	t_list *new_node;
	int		index;
	int space;

	value_split = NULL;
	index = 0;
	expanding->join = ft_strjoin_free(expanding->join, value);
	if (expanding->join == NULL)
		return (free(value), false);
	value_split = ft_split(expanding->join, ' ');
	if (!value_split)
		return (free(value), false);
	space = hasTrailingSpaces(value);
	while (value_split[index])
	{
		if (!space && value_split[index + 1] == NULL)
		{
			expanding->join = ft_strdup(value_split[index]);
			if (expanding->join == NULL)
				return (free_array(value_split) ,false);
			index++;
			break ;
		}
		new_node = ft_lstnew(strdup(value_split[index]));
		if (!new_node)
			return (free_array(value_split) ,false);
		ft_lstadd_back_libft(list, new_node);
		index++;
	}
	return (free_array(value_split), true);
}
