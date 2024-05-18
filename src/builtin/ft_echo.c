#include "../../inc/minishell.h"

/* don't forget to free the memory if join is failed */

int	check_arguments(char c, int *no_newline, int *index, int i)
{
	if (c == 'n' && c == '\0')
		*no_newline = 1;
	else if (c != 'n')
	{
		*index = i;
		return 0;
	}
	return 1;
}

void	check_args(char **args, int *dash, int *no_newline, int *index)
{
	int		i;
	int		j;

	i = 1;
	j = 1;
	*index = 2;
	while (args[i])
	{
		if (args[i][0] == '-')
			*dash = 1;
		else
		{
			*index = i;
			return ;
		}
		j = 1;
		while (args[i][j])
		{
			*dash = check_arguments(args[i][j], no_newline, index, i);
			if (!*dash)
				return ;
			j++;
		}
		i++;
	}
}

int	ft_echo(char **args, int fd)
{
	char	*str = NULL;
	int		dash = 0;
	int		index;
	int		no_newline = 0;

 	if (!args[1])
		return (ft_putchar_fd('\n', fd), 0);
	check_args(args, &dash, &no_newline, &index);
	if (dash && no_newline)
		if (!args[index])
			return (0);
	str = join_strings(args, index);
	if (!str)
	{
		if (!args[index])
			return 0;
		return (free(str), perror("minishell"), 1);
	}
	ft_putstr_fd(str, fd);
	if (!dash && !no_newline)
		ft_putchar_fd('\n', fd);
	return (free(str), 0);
}
