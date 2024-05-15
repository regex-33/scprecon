/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 19:38:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 17:27:25 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	getredir_fd(char *str, int len)
{
	int		fd;
	char	tmp;

	if (str[0] == '<' || str[0] == '>')
		return (str[0] == '>');
	tmp = str[len];
	if (len - 1 > 10 || 
		(len - 1 == 10 && ft_strncmp(str, "2147483647", 10) > 0))
		return (-1);
	str[len] = 0;
	fd = ft_atoi(str);
	str[len] = tmp;
	return (fd);
}

t_redir_type	getredir_type(char *str, int len)
{
	int				i;
	t_redir_type	type;

	i = 0;
	while (ft_isdigit(str[i]))
		i++;
	if (!str[i])
		return (REDIR_APPEND);
	if (str[i] == '>')
		type = REDIR_OUT;
	else
		type = REDIR_IN;
	if (len - i > 1)
		type += str[i] == str[i + 1];
	return (type);
}

t_redir	*new_redir(t_token *redir_token, t_token *file_token)
{
	t_redir	*redir;
	char	*str;

	redir = malloc(sizeof(t_redir));
	if (!redir || !redir_token || !file_token)
		return (free(redir), NULL);
	redir->filename = NULL;
	redir->delimiter = NULL;
	redir->bak_fd = -1;
	redir->fd = getredir_fd(redir_token->value, redir_token->len);
	redir->type = getredir_type(redir_token->value, redir_token->len);
	str = ft_substr(file_token->value, 0, file_token->len);
	if (!str)
		return (free(redir), NULL);
	if (redir->type == REDIR_HERE)
		redir->delimiter = str;
	else
		redir->filename = str;
	return (redir);
}

void	free_redir(void  *p_redir)
{
	t_redir	*redir;

	redir = (t_redir *)p_redir;
	if (!redir)
		return ;
	free(redir->filename);
	free(redir->delimiter);
	free(redir);
}

t_cmd	*new_cmd(t_list *cmd_args, t_list *redir_list)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_args = cmd_args;
	cmd->redir_list = redir_list;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd_args)
		ft_lstclear_libft(&cmd->cmd_args, free);
	if (cmd->redir_list)
		ft_lstclear_libft(&cmd->redir_list, free_redir);
	free(cmd);
}

// int	set_redir_attrs(t_redir *redir, t_token *token)
// {
// 	redir->fd = getredir_fd(token->value, token->len);
// 	redir->type = getredir_type(token->value, token->len);
// 	if (redir->type == REDIR_HERE)
// 
// }
