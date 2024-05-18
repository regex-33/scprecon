#include "../../inc/minishell.h"

int excute_failed(char **args)
{
	struct stat sb;

	stat(args[0], &sb);
	if (check_existence(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": No such file or directory", 2), 1);
	else if (!is_directory(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": Not a directory", 2), 1);
	else if (!check_permission(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": Permission denied", 2), 1);
	return (1);
}

int	excute_success(t_context *ctx)
{
	char **args;
	char	last_path[1024] = "";

	args = malloc(sizeof(char *) * 3);
	if (!args)
		return (1);
	init_array(args, 3);
	args[0] = ft_strdup("export");
	if (!args[0])
		return (free_array(args) ,1);
	args[1] = ft_strjoin("OLDPWD=", ctx->last_pwd);
	if (!args[1])
		return (free_array(args) ,1);
	args[2] = NULL;
	ft_export(args, &ctx->env, 1, &ctx->unset_path);
	free_array(args);
	getcwd(last_path, sizeof(last_path));
	if (ctx->last_pwd)
		free(ctx->last_pwd);
	ctx->last_pwd = ft_strdup(last_path);
	if (!ctx->last_pwd)
		return (1);
	return (0);
}

int	home_path(t_context *ctx)
{
	char	*home;
	char	last_path[1024] = "";

	home = get_value("HOME", ctx->env);
	if (!home)
		return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
	if (last_path[0] == '\0')
	{
		if (chdir(home) == -1)
			return (perror("minishell"), 1);
		getcwd(last_path, sizeof(last_path));
		ctx->last_pwd = ft_strdup(last_path);
		if (!ctx->last_pwd)
			return (perror("minishell"), 1);
	}
	else
		chdir(home);
	return (0);
}

int	ft_change_dir(char **path, t_context *ctx)
{
	if (path && path[0])
	{
		if (chdir(path[0]) == -1)
			return (excute_failed(path));
		if (excute_success(ctx)) 
			return (perror("minishell") ,1);
	}
    else
	{
		if (home_path(ctx))
			return (perror("minishell") ,1);
	}
	return (0);
}
