/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:16:30 by bchanaa           #+#    #+#             */
/*   Updated: 2024/03/14 21:41:29 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../lib/libft/includes/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <unistd.h>

# define PROG_NAME "pipex"
# define INVALID_FD -1
# define IN_PIPE 0
# define OUT_PIPE 1
# define READ 0
# define WRITE 1
# define INVALID_ARGS_STR "pipex: invalid arguments"
# define CMD_NOT_FOUND "minishell: command not found: "

typedef struct s_args
{
	int		ac;
	char	**av;
	char	**env;
	char	**path_dirs;
	char	*limiter;
	bool	is_heredoc;
}			t_args;

int			init_args(int ac, char **av, char **env, t_args *args);
char		*ft_getenv(char *str, char **env);
char		*ft_which(char *cmd, char **path_dirs);
//char		**get_cmd_args(char *cmd, char **path_dirs);
int			add_pipe(int pipes[2][2]);
int			handle_input(t_args *args, int fd[]);

#endif
