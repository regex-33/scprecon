
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:15:04 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/26 18:31:12 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*expand_list(t_list *list, t_context *ctx)
{
	t_token	*token;
	char	*value;
	t_list	*expanding_list = NULL;

	if (!list)
		return (0);
	while (list)
	{
		token = list->content;
		value = ft_substr(token->value, 0, token->len);
		if (!value)
			return (ft_putstr_fd("Error: failed to copy token\n", 2), NULL);
		if (!expand_arg_list(&expanding_list, value, ctx))
			return (free(value), freeLinkedList(expanding_list) , NULL);
		free(value);
		list = list->next;
	}
	return (expanding_list);
}

char **get_expanded_args(t_cmd *cmd, t_context *ctx)
{
    t_list *list_args;
	t_list *expanding_list = NULL;
	char **args;

	if (!cmd)
		return NULL;
	list_args = cmd->cmd_args;
    expanding_list = expand_list(list_args, ctx);
    if (expanding_list)
    {
        args = ft_list_to_array(expanding_list);
        if (!args)
            return (freeLinkedList(expanding_list), NULL);
        return (freeLinkedList(expanding_list), args);
        //return (args);
    }
	return (NULL);
}

int exec_simple(t_btree *tree, t_context *ctx)
{
	char **args = NULL;
	int	status;

    if (!tree)
        return 1;
    t_cmd *cmd = tree->data;
	t_list	*redir = cmd->redir_list;
	if (!cmd->cmd_args)
	{
		if (redirect(cmd->redir_list, ctx))
			return (1);
		reset_redir(cmd->redir_list, 1);
		return (0);
	}
	args = get_expanded_args(cmd, ctx);
	if (!args)
		return 1;
	status = exec_cmd(redir, args, ctx);
	// append_file_content_to_alldomains_file(redir, ctx);
	return (free_array(args), status);
}



int exec_and_or(t_btree *tree, t_context *ctx)
{
	int first_res;

	first_res = 0;
	if (!tree)
		return (0);
	first_res = __exec(tree->left, ctx);
	if (tree->type == nt_or_if && first_res)
		return __exec(tree->right, ctx);
	if (tree->type == nt_and_if && !first_res)
		return __exec(tree->right, ctx);
	return (first_res);
}


int __exec(t_btree *tree, t_context *ctx)
{
	int pipes[2][2];

	pipes[OUT_PIPE][READ] = STDIN_FILENO;
	pipes[OUT_PIPE][WRITE] = STDOUT_FILENO;
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
	{
		pid_t	pid;
		int status;
		pid = exec_pipe(tree, ctx, pipes, 1);
		if (pid < 0)
			return (1);
		waitpid(pid, &status, 0);
		while (wait(NULL) > 0);
		if (WIFEXITED(status))
		{
			//ft_printf("Pipe status: %d\n", (WEXITSTATUS(status)));
			return WEXITSTATUS(status);
		}
			
		else if (WIFSIGNALED(status))
		{
			//ft_printf("Pipe status signaled: %d\n", (128 + WTERMSIG(status)));
			return (128 + WTERMSIG(status));
		}
	}
	else if (tree->type == nt_and_if)
		return exec_and_or(tree, ctx);
	else if (tree->type == nt_or_if)
		return exec_and_or(tree, ctx);
	else if (tree->type == nt_subcmd)
		return exec_sub(tree, ctx);
	else
		return exec_simple(tree, ctx);
	return (0);
}
