/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:20:39 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 21:02:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	panic(char *prog_name, int err, char c)
{
	if (prog_name)
	{
		ft_putstr_fd(prog_name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err == PERR_UNEXP_TOK)
		ft_putendl_fd("parse error: unexpected token", 2);
	else if (err == PERR_EXP_TOK)
		ft_putendl_fd("parse error: expected token not found", 2);
	else if (err == PERR_UNC_PAR)
		ft_putendl_fd("parse error: unclosed parenthesis", 2);
	else if (err == PERR_UNC_QUOT)
		ft_putendl_fd("parse error: unclosed quotes", 2);
	else if (err == PERR_NEAR)
	{
		ft_putstr_fd("parse error near ", 2);
		write(2, &c, 1);
		ft_putendl_fd("", 2);
	}
	return (0);
}

t_token	*next_token(t_list *token_list, int flags)
{
	static t_list	*current;
	static int		is_init;

	if (flags & RESET_TOK)
	{
		is_init = 0;
		current = NULL;
		return (NULL);
	}
	if (!is_init)
	{
		current = token_list;
		is_init = 1;
	}
	if (current && (flags & CONSUME_TOK))
		current = current->next;
	if (!current)
		return (NULL);
	return ((t_token *)current->content);
}

int	expect(t_token_type type, t_list *token_list)
{
	t_token	*token;

	token = next_token(token_list, 0);
	if (!token)
		return (0);
	if (token->type == type)
		next_token(token_list, CONSUME_TOK);
	else
		return (0);
	return (1);
}

int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == tok_pipe || token->type == tok_op_or || token->type == tok_op_and);
}

int	get_prec(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == tok_op_or || token->type == tok_op_and)
		return (1);
	if (token->type == tok_pipe)
		return (2);
	return (0);
}

int	parse_redir(t_list *tokens, t_list **redir_list)
{
	t_redir	*redir;
	t_token	*token;
	t_token	*file_token;
	t_list	*node;

	token = next_token(tokens, 0);
	// next_token(tokens, CONSUME_TOK);
	file_token = next_token(tokens, CONSUME_TOK);
	if (!file_token || file_token->type != tok_literal)
		return (ft_lstclear_libft(redir_list, free_redir), panic("minishell", PERR_EXP_TOK, 0), 1);
	redir = new_redir(token, file_token);
	if (!redir)
		return (ft_lstclear_libft(redir_list, free_redir), perror("minishell"), 1);
	node = ft_lstnew(redir);
	if(!node)
		return (ft_lstclear_libft(redir_list, free_redir), perror("minishell"), 1);
	ft_lstadd_back_libft(redir_list, node);
	next_token(tokens, CONSUME_TOK);
	return (0);
}

t_node_type	get_nt(t_token *token)
{
	if (!token)
		return (nt_undefined);
	if (token->type == tok_redir)
		return (nt_io_redir);
	if (token->type == tok_pipe)
		return (nt_pipe);
	if (token->type == tok_literal)
		return (nt_simplecmd);
	if (token->type == tok_op_or)
		return (nt_or_if);
	if (token->type == tok_op_and)
		return (nt_and_if);
	return (nt_undefined);
}

t_btree	*parse_simplecmd(t_list *tokens)
{
	t_btree	*simplecmd_root;
	t_list	*node;
	t_cmd	*cmd;
	t_token	*token;

	cmd = new_cmd(NULL, NULL);
	if (!cmd)
		return (perror("minishell"), NULL);
	token = next_token(tokens, 0);	
	while (token && (token->type == tok_redir || token->type == tok_literal))
	{
		if (token->type == tok_redir)
		{
			if (parse_redir(tokens, &cmd->redir_list) != 0)
				return (free_cmd(cmd), NULL);
		}
		else if (token->type == tok_literal)
		{
			next_token(tokens, CONSUME_TOK);
			node = ft_lstnew(token);
			if (!node)
				return (perror("minishell"), free_cmd(cmd), NULL);
			ft_lstadd_back_libft(&cmd->cmd_args, node);
		}
		token = next_token(tokens, 0);
	}
//	if (!cmd->cmd_args && !cmd->redir_list)
//		return (free_cmd(cmd), panic("minishell", PERR_EXP_TOK, 0), NULL);
	simplecmd_root = new_leaf(nt_simplecmd, cmd);
	if (!simplecmd_root)
		return(perror("minishell"), free_cmd(cmd), NULL);
	return (simplecmd_root);
}

t_btree	*parse_subcmd(t_list *tokens)
{
	t_btree	*subcmd_root;
	t_token	*token;
	t_list	*redir_list;

	redir_list = NULL;
	next_token(tokens, CONSUME_TOK); // CONSUME LEFT PAREN
	subcmd_root = new_node(nt_subcmd, NULL, NULL, NULL);
	if (!subcmd_root)
		return (perror("minishell"), NULL);
	subcmd_root->left = parse_cmd(tokens, 0);
	if (!subcmd_root->left)
		return (clear_btree(subcmd_root, free), NULL);
	if (!expect(tok_r_par, tokens)) // EXPECT RIGHT PAREN
		return (clear_btree(subcmd_root, free),
			panic("minishell", PERR_EXP_TOK, 0), NULL);
	// get redirs
	token = next_token(tokens, 0);
	while (token && token->type == tok_redir)
	{
		if (parse_redir(tokens, &redir_list) != 0)
			return (clear_btree(subcmd_root, free), NULL);
		token = next_token(tokens, 0);
	}
	subcmd_root->data = redir_list;
	return (subcmd_root);
}

t_btree	*parse_pair(t_list *tokens)
{
	t_btree	*pair_root;
	t_token	*token;

	token = next_token(tokens, 0);
	pair_root = NULL;
	if (!token)
		return (panic("minishell", PERR_EXP_TOK, 0), NULL);
	if (token->type == tok_l_par)
	{
		pair_root = parse_subcmd(tokens);
		return (pair_root);
	}
	else if (token->type == tok_redir || token->type == tok_literal)
	{
		pair_root = parse_simplecmd(tokens);
		return (pair_root);
	}
	else
		return (clear_btree(pair_root, free), panic("minishell", PERR_UNEXP_TOK, 0), NULL);
	return (pair_root);
}

t_btree	*parse_cmd(t_list *tokens, int prec)
{
	t_btree	*cmd_root;
	t_btree	*tmp;
	t_token	*token;
	t_btree	*child;
	cmd_root = parse_pair(tokens);
	if (!cmd_root)
		return (NULL);
	token = next_token(tokens, 0);
	while (is_operator(token) && get_prec(token) >= prec)
	{
		next_token(tokens, CONSUME_TOK);
		tmp = cmd_root;
		child = parse_cmd(tokens, get_prec(token) + 1);
		if (!child)
			return (clear_btree(cmd_root, free), NULL);
		cmd_root = new_node(get_nt(token), token, tmp, child);
		if (!cmd_root)
			return (clear_btree(child, free), perror("minishell"), NULL);
		token = next_token(tokens, 0);
	}
	return (cmd_root);
}

t_btree	*parse(t_list *tokens)
{
	t_btree	*parse_tree;

	parse_tree = parse_cmd(tokens, 0);
	if (!parse_tree)
		return (NULL);
	else if (next_token(tokens, 0))
	{
		clear_btree(parse_tree, free);
		return (panic("minishell", PERR_UNEXP_TOK, 0), NULL);
	}
	return (parse_tree);
}

// int main(void)
// {
// 	t_list	*tokens; 
// 	t_btree	*parse_tree;
// 	char *line = get_next_line(0);
// 	while (line)
// 	{
// 		line[ft_strlen(line) - 1] = 0;
// 		tokens = lexer(line);
// 		print_token_list(tokens);
// 		parse_tree = parse(tokens);
// 		ft_printf("--------- TREE -------\n");
// 		print_tree(parse_tree, 0, nt_undefined);
// 		ft_printf("--------- COMMAND ----------\n");
// 		ft_printf("%s\n", line);
// 		ft_printf("----------- EXECUTION ---------\n");
// 		__exec(parse_tree);
// 		ft_printf("\n");
// 		ft_lstclear_libft(&tokens, free);
// 		next_token(tokens, RESET_TOK);
// 		free(line);
// 		line = get_next_line(0);
// 	}
// 	return (0);
// }
