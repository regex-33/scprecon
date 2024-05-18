#include "minishell.h"

int	handle_shell_level(char *str, char **env, int *i)
{
	int		level;
	char	*shell_level;

	level = ft_atoi(str + 6);
	if (level > 1000)
	{
		ft_putstr_fd("minishell: warning: shell level (", 2);
		ft_putstr_fd(str + 6, 2);
		ft_putstr_fd(") too high, resetting to 1\n", 2);
		level = 0;
	}
	else if (level < 0)
		level = -1;
	shell_level = ft_itoa(level + 1);
	if (!ft_strcmp(shell_level, "1000"))
		*env = ft_strdup("SHLVL="); 
	else
		*env = ft_strjoin("SHLVL=", shell_level);
	if (!*env)
		return (free(shell_level), 1);
	(*i)++;
	return (free(shell_level), 0);
}

char	**creat_temp_env(t_context *ctx)
{
	char	**temp_env;
	int		level;

	level = 0;
	ctx->unset_path = 1;
	temp_env = allocate_new_environ(5);
	if (!temp_env)
		return (perror("minishell"), NULL);
	temp_env[4] = NULL;
	temp_env[0] = ft_strdup(FIRST_ENV);
	if (!temp_env[0])
		return (free_array(temp_env), perror("minishell"), NULL);
	temp_env[1] = ft_strdup(SECOND_ENV);
	if (!temp_env[1])
		return (free_array(temp_env), perror("minishell"), NULL);
	if (!ft_strncmp(temp_env[1], "SHLVL=", 6))
		if (handle_shell_level(temp_env[1], &temp_env[1], &level))
			return (free_array(temp_env), perror("minishell"), NULL);
	temp_env[2] = ft_strdup(THIRD_ENV);
	if (!temp_env[2])
		return (free_array(temp_env), perror("minishell"), NULL);
	temp_env[3] = ft_strdup(TEMP_PATH);
	if (!temp_env[3])
		return (free_array(temp_env), perror("minishell"), NULL);
	return (temp_env);
}

char	**ft_creat_env(t_context *ctx)
{
	char		**env;
	extern char	**environ;
	int			i;

	i = 0;
	if (!*environ)
		return (creat_temp_env(ctx));
	env = allocate_new_environ(get_env_count(environ));
	if (!env)
		return (perror("minishell"), NULL);
	while (environ[i])
	{
		if (!ft_strncmp(environ[i], "SHLVL=", 6))
		{
			if (handle_shell_level(environ[i], &env[i], &i))
				return (free_array(env), perror("minishell"), NULL);
			continue ;
		}
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
			return (free_array(env), perror("minishell"), NULL);
		i++;
	}
	env[i] = NULL;
	return (env);
}
