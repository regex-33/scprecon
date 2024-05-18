/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:01:09 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/25 12:20:54 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define BASE_HERE_FILENAME "/tmp/.minshl-"
# define HERE_PROMPT "heredoc> "
# define HERE_MAX_RETRY 30
# define SUFF_LEN 30

char	*random_filename(void)
{
	unsigned char	filename[SUFF_LEN + 1];
	int				fd;
	int				i;
	int				r_len;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (NULL);
	r_len = read(fd, filename, SUFF_LEN);
	if (r_len <= 0)
		return (NULL);
	i = 0;
	filename[r_len] = 0;
	while (i < r_len)
	{
		filename[i] = (filename[i] % 25) + 65;
		i++;
	}
	close(fd);
	return (ft_strjoin(BASE_HERE_FILENAME, (char *)filename));
}

pid_t	fork_heredoc(char *limiter, size_t lim_len, int fd)
{
	pid_t	pid;
	char	*line;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_putstr_fd(HERE_PROMPT, STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		while (line)
		{
			if (lim_len == ft_strlen(line) - 1 && \
				!ft_strncmp(line, limiter, lim_len))
				return (free(line), get_next_line(INVALID_FD), close(fd), exit(0), 0);
			ft_putstr_fd(HERE_PROMPT, STDOUT_FILENO);
			if (write(fd, line, ft_strlen(line)) < 0)
				return (free(line), get_next_line(INVALID_FD), close(fd), exit(1), 0);
			free(line);
			line = get_next_line(STDIN_FILENO);
		}
		close(fd);
		ft_putendl_fd("\n", 2);
		ft_putendl_fd("minishell: warning: here document expected delimiter not found", 2);
		get_next_line(INVALID_FD);
		exit(0);
	}
	return (pid);
}

int	read_heredoc(char *limiter, int fd)
{
	size_t	limiter_len;
	int		status;
	pid_t	heredoc_pid;

	if (!limiter)
		return (ft_putendl_fd("minishell: invalid here document delimiter", 1), -1);
	limiter_len = ft_strlen(limiter);
	heredoc_pid = fork_heredoc(limiter, limiter_len, fd);
	if (heredoc_pid < 0)
		return (-1);
	waitpid(heredoc_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	new_heredoc(t_redir *redir)
{
	char	*filename;
	int		i;
	int		fd;

	filename = random_filename();
	if (!filename)
		return (-1);
	i = 0;
	while (i < HERE_MAX_RETRY && !access(filename, F_OK))
	{
		free(filename);
		filename = random_filename();
		if (!filename)
			return (-1);
	}
	fd = open(filename, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return (free(filename), -1);
	if (read_heredoc(redir->delimiter, fd) != 0)
		return (unlink(filename), close(fd), free(filename), -1);
	close(fd);
	redir->filename = filename;
	return (0);
}

int	prompt_heredoc(t_btree *tree)
{
	t_list	*redir_list;
	t_redir	*redir;

	if (tree->type != nt_subcmd && tree->type != nt_simplecmd)
	{
		if (tree->left && prompt_heredoc(tree->left) < 0)
			return (-1);
		if (tree->right && prompt_heredoc(tree->right) < 0)
			return (-1);
	}
	else
	{
		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		while(redir_list)
		{
			redir = redir_list->content;
			if (redir->type == REDIR_HERE && new_heredoc(redir) < 0)
				return (-1);
			redir_list = redir_list->next;
		}
	}
	return (0);
}
