#include "minishell.h"

int	is_file(char *path)
{
	struct stat	sb;

	if (stat(path, &sb))
		return (0);
	return ((sb.st_mode & S_IFMT) != S_IFDIR);
}

char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (!path_dirs)
		return (NULL);
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_pathname = ft_strjoin(path_dirs[i], tmp);
		if (!cmd_pathname)
			return (free(tmp), NULL);
		// printf("cmd_pathname: %s\n", cmd_pathname);
		if (is_file(cmd_pathname))
			return (free(tmp), cmd_pathname);
		// printf("cmd_pathname: %s\n", cmd_pathname);
		free(cmd_pathname);
	}
	return (free(tmp), NULL);
}

void	print_err(char *mid, char *suffix)
{
	ft_putstr_fd("minishell: ", 2);
	if (mid && !suffix)
		ft_putendl_fd(mid, 2);
	if (mid && suffix)
		ft_putstr_fd(mid, 2);
	if (suffix)
		ft_putendl_fd(suffix, 2);
}

int	check_abs_cmd(char *pathname)
{
	struct stat	sb;

	if (stat(pathname, &sb))
	{
		if (errno == ENOENT)
			return (print_err(pathname, " : command not found."), 127);
		if (errno == EACCES)
			return (print_err(pathname, " : Permission denied."), 126);
	}
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (print_err(pathname, " : is a directory."), 126);
	if (access(pathname, X_OK))
		return (print_err(pathname, " : Permission denied."), 126);
	return (0);
}

int	init_command(t_prexec *pexec, t_context *ctx, char **args)
{
	char	**path_dirs;

	pexec->err = 0;
	pexec->cmd_name = NULL;
	if (!*args)
		return (1);
	path_dirs = grep_paths(ctx->env, &ctx->hidden_path);
	if (ft_strchr(args[0], '/'))
	{
		pexec->err = check_abs_cmd(args[0]);
		if (pexec->err)
			return (pexec->err);
		pexec->cmd_name = ft_strdup(args[0]);
	}
	else
	{
		pexec->cmd_name = ft_which(args[0], path_dirs);
		if (!pexec->cmd_name)
		{
			pexec->err = 127;
			print_err(args[0], " : command not found");
		}
		else if (access(pexec->cmd_name, X_OK))
		{
			pexec->err = 126;
			print_err(pexec->cmd_name, " : Permission denied.");
		}
	}
	pexec->args = args;
	return (pexec->err);
}

char	**get_cmd_args(char **cmd_args, char **path_dirs)
{
	char	*cmd_pathname;

	if (!cmd_args || !path_dirs)
		return (NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
	if (!cmd_pathname)
	{
		if (!ft_strchr(cmd_args[0], '/'))
		{
			ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
			ft_putendl_fd(cmd_args[0], STDERR_FILENO);
			return (ft_free_arr(cmd_args), NULL);
		}
		return (ft_free_arr(cmd_args), perror("minishell"), NULL);
	}
	free(cmd_args[0]);
	cmd_args[0] = cmd_pathname;
	return (cmd_args);
}

void	print_fd_err(int fd)
{
	ft_putstr_fd("minishell: ", 2);
	if (fd < 0)
		ft_putstr_fd("file descriptor out of range", 2);
	else
		ft_putnbr_fd(fd, 2);
	ft_putstr_fd(": ", 2);
	perror(NULL);
}

int	append_file_content_to_alldomains_file(t_list *redir_list, t_context *ctx)
{
	t_redir *redir;
	char *content;
	int fd;
	int fd_new;

	(void)ctx;
	if (ctx->save_all)
		return 1;
	fd_new = open("alldomains.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_new < 0)
	{
		perror("Error opening alldomains.txt");
		return (1);
	}
	while (redir_list)
	{
		redir = redir_list->content;
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0)
		{
			perror("Error opening input file");
			close(fd_new);
			return (1);
		}

		content = get_next_line(fd);
		printf("content: %s\n", content);
		while (content && *content != '\n')
		{
			ft_putstr_fd(content, fd_new);
			free(content);
			content = get_next_line(fd);
		}
		close(fd);
		redir_list = redir_list->next;
	}
	close(fd_new);
	return (0);
}


int exec_cmd(t_list * redir_list, char **args, t_context *ctx)
{
	pid_t pid;
	t_prexec pexec;
	int status;

	status = select_buildin_commands(args, redir_list, ctx);
	if (status != -1)
		return (status);
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 0);
	if (pid == 0)
	{
		// signal(SIGQUIT, SIG_DFL);
		if (redirect(redir_list, ctx))
			return (exit(EXIT_FAILURE), 0);
		reset_redir(redir_list, 0);
		if (init_command(&pexec, ctx, args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
		{
			return (perror("minishell"), exit(1), 0);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		return (status);
	}
	return (1);
}