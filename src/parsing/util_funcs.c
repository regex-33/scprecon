/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:36:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 15:36:57 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isquote(char c)
{
	return (c == '"' || c == '\'');
}

int	match_pattern(char *str, int flags, char *sentinel)
{
	int	i;
	int	found;
	int	has_sentinel;

	i = 0;
	has_sentinel = 0;
	while (str[i])
	{
		if (sentinel && ft_strchr(sentinel, str[i]))
		{
			has_sentinel = 1;
			break ;
		}
		found = ((flags & O_DIGITS) && ft_isdigit(str[i])) || 
				((flags & O_LETTERS) && ft_isalpha(str[i]));
		if (!found)
			return (-1);
		i++;
	}
	if (!sentinel || has_sentinel)
		return (i);
	return (-1);
}

size_t	wordlen(char *str)
{
	int	i;
	int	quote;

	while (*str && ft_strchr(TOKEN_D, *str))
			str++;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == str[i])
			quote = 0;
		else if (!quote && isquote(str[i]))
			quote = str[i];
		else if (!quote && ft_strchr(TOKEN_D, str[i]))
			break;
		i++;
	}
	if (quote) // invalid quoted literal
		return (0);
	return (i);
}

void	print_token(t_token *token)
{
	char	tokens[10][20] = {
		"undefined",
		"l_par",
		"r_par",
		"literal",
		"pipe",
		"op_or",
		"op_and",
		"redir"
	};
	ft_printf("[type: %s; val: ", tokens[token->type]);
	write(1, token->value, token->len);
	ft_printf(" ] ");
}

void	print_token_list(t_list *token_list)
{
	t_list	*curr;
	t_token	*token;

	curr = token_list;
	int	size = ft_lstsize(token_list);
	ft_printf("Tokens: %d\n", size);
	while (curr)
	{
		token = curr->content;	
		print_token(token);
		ft_printf(" -> ");
		curr = curr->next;
	}
	ft_printf(" END\n");
}

/*
char	*get_word(char *str)
{
	char	*word;
	int		i;
	int		quoted;

	i = 0;
	while (issep(str[i]))
			i++;
	quoted = isquote(str[i]); 
	word = str + i;
	while (str[i] && ((!quoted && !issep(str[i])) || (quoted && isquote(str[i]))))
		i++;
	if (issep(str[i]))
		str[i] = 0;
	return (word);
}
*/
