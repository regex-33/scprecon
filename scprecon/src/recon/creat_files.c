#include "minishell.h"

int	open_files_and_add_content_to_alldomains(t_list *commands, char *target, char *discord_url)
{
	t_list	*tmp;
	char	*command;
	int		fd;
	char	*line;
	int		new_fd;
	char	path[100];
	char	*mv_command = NULL;
	char	*old_path = NULL;

	line = NULL;
	tmp = commands;
	sprintf(path, "./output/%s-domains.txt", target);
	old_path = ft_strjoin(path, ".old");
	if (!old_path)
		return (1);
	mv_command = create_mv_command(path, old_path);
	if (!mv_command)
		return (1);
	if (check_file_exist(path))
		exec_command(mv_command, 0);
	else
	{
		new_fd = open(old_path, O_CREAT | O_WRONLY, 0644);
		if (new_fd == -1)
			return (perror("scprecon"), 1);
		close(new_fd);
	}
	new_fd = open(path, O_CREAT | O_WRONLY, 0644);
	if (new_fd == -1)
		return (perror("scprecon"), 1);
	while (tmp)
	{
		command = tmp->content;
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = '\0';
		usleep(1000);
		fd = open(command, O_RDONLY);
		if (fd == -1)
			return (close(new_fd), perror("scprecon"), 1);
		line = get_next_line(fd);
		while (line)
		{
			if (*line != '\n' && *line != '\0' && *line != '#')
				ft_putstr_fd(line, new_fd);
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
		fd = 0;
		tmp = tmp->next;
	}
	close(new_fd);
	compare_files(old_path, path, discord_url);
	return (free(old_path), free(mv_command), 0);
}



int	add_content_of_files_to_alldomains(char *discord_url, char *target, int num_commands)
{
	char	*line;
	t_list	*file_names;
	t_list	*new_commands;
    char filename[120];
	int		i = 0;

	line = NULL;
	file_names = NULL;
	new_commands = NULL;
	while (i < num_commands)
	{
        sprintf(filename, "./output/file-%d.txt", i);
		new_commands = ft_lstnew(ft_strdup(filename));
		if (!new_commands)
			return (ft_lstclear_libft(&file_names, free), perror("scprecon"), 1);
		ft_lstadd_back_libft(&file_names, new_commands);
		i++;
    }
	return (open_files_and_add_content_to_alldomains(file_names, target, discord_url));
}