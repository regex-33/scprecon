#include "minishell.h"

int	get_status(int new_status, int flags)
{
	static int	status;

	if (flags == SET_STATUS)
		status = new_status;
	return (status);
}

// Set: get_status(your_status, SET_STATUS);
// Get: get_status(0, 0);

int	is_builtin(char *cmd_name)
{
	if (!ft_strcmp(cmd_name, "cd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "exit"))
		return (1);
	else if (!ft_strcmp(cmd_name, "echo"))
		return (1);
	else if (!ft_strcmp(cmd_name, "env"))
		return (1);
	else if (!ft_strcmp(cmd_name, "export"))
		return (1);
	else if (!ft_strcmp(cmd_name, "unset"))
		return (1);
	return (0);
}

char	**grep_paths(char **env, int *hidden_path)
{
	char	*path_env;
	char	**path_dirs;

	// printArray(env);
	path_env = get_value("PATH", env);
	if (!path_env)
	{
		if (*hidden_path)
		{
			path_env = ft_strdup(TEMP_PATH + 5);
			*hidden_path = 0;
		}
		else
		{
			printf("i am here\n");
			*hidden_path = 0;
			path_env = ft_strdup(".");
		}
	}
	// printf("path_env : %s\n", path_env);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (perror(COMMAND_NOT_FOUND), NULL);
	return (path_dirs);
}

int	parse_existing_variable(char *str, int flag)
{
	int	add_to_value;

	add_to_value = 0;
	if (!str)
		return (perror("minishell"), -1);
	if (!is_valid_identifier(str))
	{
		if (flag)
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(str, 2);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		}
		return (-1);
	}
	if (ft_strchr(str, '+') != NULL)
		add_to_value = 1;
	return (add_to_value);
}

int	init_context(t_context *ctx, int save_all)
{
	ctx->env = NULL;
	ctx->save_all = save_all;
	ctx->unset_path = 0;
	ctx->hidden_path = 0;
	ctx->env = ft_creat_env(ctx);
	if (ctx->env == NULL)
		return (1);
	if (get_value("PATH", ctx->env) == NULL)
		ctx->hidden_path = 1;
	ctx->last_pwd = ft_strdup(get_value("PWD", ctx->env));
	if (!ctx->last_pwd)
		ctx->last_pwd = ft_strdup("");
	return (0);
}

int	free_context(t_context *ctx)
{
	if (ctx->env)
		free_array(ctx->env);
	if (ctx->last_pwd)
		free(ctx->last_pwd);
	return (0);
}