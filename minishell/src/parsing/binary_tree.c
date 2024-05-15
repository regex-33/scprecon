/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:21:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 21:22:09 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parse -> PARSE_CMD(0)
// PARSE_CMD(x) -> PAIR { OPERATOR PARSE_CMD(q) }
// PARSE_PAIR -> SUBCMD | SIMPLE_COMMAND
// PARSE_SUBCMD -> LPAR CMD(0) RPAR {REDIR}
// SIMPLE_COMMAND -> [REDIR | LITERAL] {REDIR | LITERAL}
// REDIR -> IO_REDIR LITERAL

#include "minishell.h"

t_btree	*new_node(t_node_type type, void *data, t_btree *left, t_btree *right)
{
	t_btree	*tree;

	tree = malloc(sizeof(t_btree));
	if (!tree)
		return (NULL);
	tree->data = data;
	tree->type = type;
	tree->left = left;
	tree->right = right;
	return (tree);
}

t_btree	*new_leaf(t_node_type type, void *data)
{
	t_btree	*leaf;

	if (!data)
		return (NULL);
	leaf = malloc(sizeof(t_btree));
	if (!leaf)
		return (NULL);
	leaf->data = data;
	leaf->type = type;
	leaf->left = NULL;
	leaf->right = NULL;
	return (leaf);
}

void	clear_btree(t_btree *tree, void (*del)(void *))
{
	t_btree	*left;
	t_btree	*right;

	if (!tree)
		return ;
	if (del)
		del(tree->data);
	right = tree->right;
	left = tree->left;
	free(tree);
	if (left)
		clear_btree(left, del);
	if (right)
		clear_btree(right, del);
}


// typedef enum e_node_type
// {
// 	nt_undefined,
// 	nt_subcmd,
// 	nt_subredir,
// 	nt_simplecmd,
// 	nt_pipe,
// 	nt_and_if,
// 	nt_or_if,
// 	nt_io_redir,
// 	//nt_cmd_arg
// } 	t_node_type;

void	print_redir(t_redir *redir)
{
	char redir_strings[10][20] = 
	{
		"in",
		"here",
		"out",
		"append"
	};
	if (redir->type != REDIR_HERE)
		ft_printf("[[ %s %d->%s ]] ", redir_strings[redir->type], redir->fd, redir->filename);
	else
		ft_printf("[[ heredoc fd:%d; delim: %s file: %s]] ", redir->fd, redir->delimiter, redir->filename);
}

void	print_simplecmd(t_btree *node, int depth, t_node_type parentt)
{
	char nt_strings[10][20] = 
	{
		"UNDEFINED",
		"SUBCMD",
		"SUBREDIR",
		"SIMPLE_CMD",
		"PIPE",
		"AND",
		"OR",
		"IO_REDIR",
		"CMD_ARG"
	};
	if (!node)
		ft_printf("[ERROR: NULL NODE]\n");
	t_cmd *cmd = node->data;
	ft_printf("( %s [%d] {p: %d} : [", nt_strings[node->type], depth, parentt);
	t_list	*cmd_args = cmd->cmd_args;
	while (cmd_args)
	{
		t_token	*token = cmd_args->content;
		write(1, token->value, token->len);
		if (cmd_args->next)
			ft_printf(", ");
		cmd_args = cmd_args->next;
	}
	ft_printf("] ");
	t_list	*redir_list = cmd->redir_list;
	while (redir_list)
	{
		t_redir	*redir = redir_list->content;
		print_redir(redir);
		redir_list = redir_list->next;
	}
	ft_printf(" )\n");
}

void	print_node(t_btree *node, int depth, t_node_type parent_type)
{
	int	i;
	i = 0;
	char nt_strings[10][20] = 
	{
		"UNDEFINED",
		"SUBCMD",
		"SUBREDIR",
		"SIMPLE_CMD",
		"PIPE",
		"AND",
		"OR",
		"IO_REDIR",
		"CMD_ARG"
	};
	while (i < depth)
	{
		ft_printf("     ");
		i++;
	}
	if (node->type == nt_simplecmd)
	{
		print_simplecmd(node, depth, parent_type);
	}
	else if (node->type == nt_subcmd)
	{
		ft_printf("( %s [%d] {p: %s} ", nt_strings[node->type], depth, nt_strings[parent_type]);
		t_list	*redir_list = node->data;
		while (redir_list)
		{
			t_redir *redir = redir_list->content;
			print_redir(redir);
			redir_list = redir_list->next;
		}
		ft_printf(" )\n");
	}
	else
		ft_printf("( %s [%d] {p: %s} )\n", nt_strings[node->type], depth, nt_strings[parent_type]);

	
}

void	print_tree(t_btree *tree, int depth, t_node_type parent_type)
{
	if (!tree)
		return ;
	print_node(tree, depth, parent_type);
	if (tree->left)
		print_tree(tree->left, depth + 1, tree->type);
	if (tree->right)
		print_tree(tree->right, depth + 1, tree->type);
}
