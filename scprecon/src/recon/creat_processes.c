#include "minishell.h"


int	wait_process(void)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	if (pid == -1)
	{
		if (errno == ECHILD)
			return (1);
		else
			return (perror("scprecon"), 1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
		return (1);
	return (0);
}

int	fork_processes(int num_commands, t_list *commands, int save_all)
{
	int		i;
	pid_t	pid;
	char	*command;
	t_list	*tmp;

	i = 0;
	tmp = commands;
	while (i < num_commands && tmp)
	{
		pid = fork();
		if (pid < 0)
			return (perror("fork"), 1);
		else if (pid == 0)
		{
			command = tmp->content;
			exec_command(command, save_all);
			exit(0);
		}
		tmp = tmp->next;
		i++;
		if (i == num_commands)
		{
			if (!wait_process())
				return (1);
			i = 0;
		}
	}
	return (0);
}

int expand_command(char *line, int num_commands, t_list **commands, const char *target)
{
	char file[50];
	char *expand_scp = NULL;
	char *expand_target = NULL;
	t_list *new_commands = NULL;

	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
	sprintf(file, "./output/file-%d.txt", num_commands);
	expand_scp = replace(line, "$scp", file);
	if (!expand_scp)
		return (free(line), ft_lstclear_libft(commands, free), perror("scprecon"), 1);
	expand_target = replace(expand_scp, "$target", target);
	if (!expand_target)
		return (free(line), free(expand_scp), ft_lstclear_libft(commands, free), perror("scprecon"), 1);
	free(expand_scp);
	new_commands = ft_lstnew(expand_target);
	if (!new_commands)
		return (free(line), ft_lstclear_libft(commands, free), perror("scprecon"), 1);
	ft_lstadd_back_libft(commands, new_commands);
	return (0);
}

int fork_and_expand_and_exec_commands(int save_all, int *processes, char *target)
{
	int fd;
	char *line = NULL;
	char *file_name = "commands.txt";
	t_list *commands = NULL;

	int num_commands = 0;
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (perror("scprecon"), 0);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
		{
			if (expand_command(line, num_commands, &commands, target))
				return (0);
			num_commands++;
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (num_commands > *processes)
		*processes = num_commands;
	if (fork_processes(*processes, commands, save_all))
		return (ft_lstclear_libft(&commands, free), 0);
	// printLinkedList(commands);
	return (ft_lstclear_libft(&commands, free), num_commands);
}