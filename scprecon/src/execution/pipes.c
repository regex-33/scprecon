#include "minishell.h"

int	add_pipe(int pipes[2][2])
{
	int	tmp[2];

	if (pipe(tmp) == -1)
		return (-1);
	pipes[IN_PIPE][READ] = pipes[OUT_PIPE][READ];
	pipes[IN_PIPE][WRITE] = pipes[OUT_PIPE][WRITE];
	pipes[OUT_PIPE][READ] = tmp[READ];
	pipes[OUT_PIPE][WRITE] = tmp[WRITE];
	return (0);
}

int exec_pipe(t_btree *tree, t_context *ctx, int pipes[2][2], int is_root)
{
	if (!tree)
		return (0);
	if (tree->left->type != nt_pipe)
	{
		add_pipe(pipes);
		exec_piped_cmd(tree->left, ctx, pipes);
	}
	else
		exec_pipe(tree->left, ctx, pipes, 0);
	if (!is_root)
	{
		add_pipe(pipes);
		// return (exec_piped_cmd(tree->right, ctx, pipes), append_file_content_to_alldomains_file(tree->right->data, ctx));
		return (exec_piped_cmd(tree->right, ctx, pipes), 0); 
	}
	else
		return (exec_last_piped_cmd(tree->right, ctx, pipes[OUT_PIPE]), 0);
}

int	exec_sub(t_btree *tree, t_context *ctx)
{
	t_list	*redir_list;
	pid_t	pid;

	redir_list = tree->data;
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 1);
	if (pid == 0) // CHILD
	{
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		// if (ctx->save_all)
		// 	append_file_content_to_alldomains_file(redir_list, ctx);
		exit(__exec(tree->left, ctx));
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			ft_printf("subcmd status: %d\n", (WEXITSTATUS(status)));
			return WEXITSTATUS(status);
		}
			
		else if (WIFSIGNALED(status))
		{
			ft_printf("subcmd status signaled: %d\n", (128 + WTERMSIG(status)));
			return (128 + WTERMSIG(status));
		}
		return (1);
	}
}


pid_t	exec_piped_cmd(t_btree *tree, t_context *ctx, int pipes[2][2])
{
	pid_t		pid;
	t_cmd		*cmd;
	t_list		*redir_list;
	t_prexec	pexec;
	int			status;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		close(pipes[OUT_PIPE][READ]);
		if (dup2(pipes[IN_PIPE][READ], STDIN_FILENO) < 0)
			return (close(pipes[OUT_PIPE][WRITE]), exit(1), 0);
		dup2(pipes[OUT_PIPE][WRITE], STDOUT_FILENO);
		close(pipes[OUT_PIPE][WRITE]);
		if (pipes[IN_PIPE][READ] != STDIN_FILENO)
			close(pipes[IN_PIPE][READ]);
		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		if (tree->type == nt_subcmd)
		{
			if (redirect(redir_list, ctx))
				return (exit(1), 0);
			reset_redir(redir_list, 0);
			// if (ctx->save_all)
				// append_file_content_to_alldomains_file(redir_list, ctx);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		if (!cmd->cmd_args)
			return (exit(0), 0);
		pexec.args = get_expanded_args(cmd, ctx);
		if (!pexec.args)
			return (perror("minishell"), exit(1), 0);
		status = select_buildin_commands(pexec.args, cmd->redir_list, ctx);
		if (status != -1)
			return (exit(status), 0);
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		reset_redir(redir_list, 0);
		// if (ctx->save_all)
			// append_file_content_to_alldomains_file(redir_list, ctx);
		if (init_command(&pexec, ctx, pexec.args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
			return (perror("minishell"), exit(1), 0);
	}
	else
	{
		if (pipes[IN_PIPE][READ] != STDIN_FILENO)
			close(pipes[IN_PIPE][READ]);
		return (close(pipes[OUT_PIPE][WRITE]), pid);
	}
	return (-1);
}

pid_t	exec_last_piped_cmd(t_btree *tree, t_context *ctx, int fd[2])
{
	pid_t		pid;
	t_cmd		*cmd;
	t_list		*redir_list;
	t_prexec	pexec;
	int			status;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		if (dup2(fd[READ], STDIN_FILENO))
			return (close(fd[READ]), exit(1), 0);
		close(fd[READ]);
		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		if (tree->type == nt_subcmd)
		{
			if (redirect(redir_list, ctx))
				return (exit(1), 0);
			// if (ctx->save_all)
				// append_file_content_to_alldomains_file(redir_list, ctx);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		if (!cmd->cmd_args)
			return (exit(0), 0);
		pexec.args = get_expanded_args(cmd, ctx);
		if (!pexec.args)
			return (perror("minishell"), exit(1), 0);
		status = select_buildin_commands(pexec.args, cmd->redir_list, ctx);
		if (status != -1)
			return (exit(status), 0);
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		reset_redir(redir_list, 0);
		// if (ctx->save_all)
			// append_file_content_to_alldomains_file(redir_list, ctx);
		if (init_command(&pexec, ctx, pexec.args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
		{
			return (perror("minishell"), exit(1), 0);
		}
	}
	else
		return (close(fd[READ]), pid);
	return (-1);
}