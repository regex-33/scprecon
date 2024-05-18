
#include "minishell.h"

int	is_valid_identifier(const char *variable)
{
	size_t	i;

	i = 1;
	if (!ft_isalpha(variable[0]) && variable[0] != '_')
		return (0);
	while (variable[i] != '\0' && variable[i] != '=')
	{
		if (!ft_isalnum(variable[i]) && variable[i] != '_'
			&& variable[i] != '$')
		{
			if (variable[i] == '+' && variable[i + 1] != '=')
				return (0);
		}
		i++;
	}
	return (1);
}

void	print_to_delemeter(char *str, char delimeter)
{
	while (*str && *str != delimeter)
		write(1, str++, 1);
}

void	ft_print(char **variable, int fd, int unset_path)
{
	int		i;
	char	*str;

	i = 0;
	while (variable[i] != NULL)
	{
		if (unset_path && !ft_strncmp(variable[i], "PATH=", 5))
		{
			i++;
			continue ;
		}
		str = ft_strchr(variable[i], '=');
		ft_putstr_fd("declare -x ", fd);
		print_to_delemeter(variable[i], '=');
		if (str)
			ft_putstr_fd("=\"", 1);
		if (str != NULL)
			ft_putstr_fd(str + 1, fd);
		if (str)
			ft_putstr_fd("\"\n", fd);
		else
			ft_putstr_fd("\n", fd);
		i++;
	}
}
int	ft_sort_export_cmd(char **environ_exp, int fd, int unset_path)
{
	char	**variable;
	int		env_count;
	char	**array;

	env_count = 0;
	if (!environ_exp || !*environ_exp)
		return (perror("minishell"), 0);
	variable = allocate_new_environ(get_env_count(environ_exp) + 1);
	if (!variable)
		return (perror("minishell"), 0);
	array = variable;
	while (environ_exp[env_count])
	{
		variable[env_count] = ft_strdup(environ_exp[env_count]);
		if (variable[env_count] == NULL)
		{
			while (env_count > 0)
				free(variable[env_count--]);
			return (free(variable), perror("minishell"), 0);
		}
		env_count++;
	}
	variable[env_count] = NULL;
	return (merge_sort(variable, 0, env_count - 1), ft_print(variable, fd,
			unset_path), free_array(variable), 1);
}

int	check_variable_name(char *variable, int *unset_path)
{
	int		to_equal;
	char	*str;

	if (*unset_path && !ft_strncmp(variable, "PATH=", 5))
		*unset_path = 0;
	to_equal = ft_strchr(variable, '=') - variable;
	str = ft_substr(variable, 0, to_equal);
	if (!str)
		return (perror("minishell"), 0);
	if (ft_strchr(str, ' '))
	{
		if (variable[strlen(str) - 1] == ' ' || variable[strlen(str)
			- 1] == '\t')
			return (ft_putstr_fd("minishell: export: ", 2),
				ft_putstr_fd(&variable[strlen(str) - 1], 2),
				ft_putstr_fd(": not a valid identifier\n", 2), free(str), 0);
		return (ft_putstr_fd("minishell: export: ", 2), ft_putstr_fd(variable,
				2), ft_putstr_fd(": not a valid identifier\n", 2), free(str),
			0);
	}
	return (free(str), 1);
}
