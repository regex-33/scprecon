/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 15:48:45 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 15:35:22 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_token_length(char *line, t_token *token) {
	if (token->type == tok_redir)
	{
		token->len = match_pattern(line, O_DIGITS, "<>") + 1;
		if (line[token->len - 1] == line[token->len])
			token->len++;
	}
	else if (token->type == tok_op_or || token->type == tok_op_and)
		return (2); else if (token->type == tok_literal)
		token->len = wordlen(line);
	else
		return (1);
	return (token->len);
}

t_token	*gettoken(char *line, int *index)
{
	int		i;
	t_token	*token;

	i = 0;
	token = malloc(sizeof(t_token));
	token->len = 0;
	if (!token)
		return (NULL);
	if (line[i] == '(')
		token->type = tok_l_par;
	else if (line[i] == ')')
		token->type = tok_r_par;
	else if (line[i] == '|')
		token->type = tok_pipe + (line[i] == line[i + 1]);
	else if (line[i] == '&')
	{
		if (line[i] == line[i + 1])
			token->type = tok_op_and;
		else
		{
			token->type = tok_undefined;
			return (token);
		}
	}
	else if (match_pattern(line, O_DIGITS, "<>") > 0)
		token->type = tok_redir;
	else if (line[i] == '>' || line[i] == '<')
		token->type = tok_redir;
	else
		token->type = tok_literal;
	token->value = line;
	token->len = get_token_length(line, token);
//	if (token->len < 1)
//		return (free(token), NULL);
	*index += token->len;
	return (token);
}

t_list	*lexer(char *line)
{
	t_token	*token;
	t_list	*node;
	t_list	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	if (!line)
		return (NULL);
	while (line[i])
	{
		if (line[i] == 32 || line[i] == '\t')
		{
			i++;
			continue ;
		}
		token = gettoken(line + i, &i);
		if (!token)
			return (perror("minishell"), ft_lstclear_libft(&tokens, free), NULL); // malloc error
		if (token->type == tok_undefined || token->len < 1)
		{
			panic("minishell", PERR_NEAR, line[i]);
			return (free(token), ft_lstclear_libft(&tokens, free), NULL);
		}
		node = ft_lstnew(token);
		if (!node)
			return (perror("minishell"), free(token), ft_lstclear_libft(&tokens, free), NULL); // malloc error
		ft_lstadd_back_libft(&tokens, node);
	}
	return (tokens);
}
