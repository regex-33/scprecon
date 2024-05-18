#include "../inc/minishell.h"

int	get_env_count(char **env)
{
	int	count;

	count = 0;
	while (env[count] != NULL)
		count++;
	return (count);
}

char	**allocate_new_environ(int env_count)
{
	char	**new_environ;

	new_environ = malloc(env_count * sizeof(char *));
	if (!new_environ)
		return (perror("minishell"), NULL);
	init_array(new_environ, env_count);
	return (new_environ);
}


int	copy_variable_to_new_environment(char *env_var, char **new_environ, int *j)
{
	new_environ[*j] = strdup(env_var);
	if (!new_environ[*j])
	{
		while (*j > 0)
			free(new_environ[(*j)--]);
		return (free(new_environ) ,perror("minishell"), 1);
	}
	(*j)++;
	return (0);
}

int	copy_env_except_variable(char **env, char **new_environ, char *variable,
		int *j)
{
	int		i;
	char	*equals_pos;
	int		var_length;

	i = 0;
	while (env[i] != NULL)
	{
		equals_pos = ft_strchr(env[i], '=');
		if (!equals_pos)
		{
			i++;
			continue ;
		}
		var_length = equals_pos - env[i];
		if (!ft_strncmp(variable, env[i], var_length))
			i++;
		else
		{
			if (copy_variable_to_new_environment(env[i], new_environ, j))
				return (1);
			i++;
		}
	}
	new_environ[*j] = NULL;
	return (0);
}
int	ft_unset(char **variable, char ***env_ptr, int *unset_path)
{
	char	**new_environ;
	int		j;
	char	**env;
	int		index;

	if (!variable || !*variable)
		return (0);
	index = 1;
	while (variable[index])
	{
		env = *env_ptr;
		j = 0;
		if (!ft_strncmp(variable[index], "PATH", 4))
			*unset_path = 0;
		new_environ = allocate_new_environ(get_env_count(env));
		if (!new_environ)
			return (1);
		if (copy_env_except_variable(env, new_environ, variable[index], &j))
			return (1);
		free_array(env);
		*env_ptr = new_environ;
		index++;
	}
	return (0);
}
