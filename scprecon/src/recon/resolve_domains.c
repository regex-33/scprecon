#include "minishell.h"

int	resolve_domains(char *discord_url)
{
	char	*command = NULL;
	extern char **environ;
	int		fd;
	char	*line;
	char	*path = NULL;
	char	*old_path = NULL;
	char 	*rename_file = NULL;
	char	*answer = NULL;
	char	*resolvers = NULL;
	char	*value = NULL;

	line = NULL;
	fd = open("domains.txt", O_RDONLY);
	if (fd == -1)
		return (perror("scprecon"), 1);
	
	value = get_value("HOME", environ);
	if (!value)
		return (perror("scprecon"), 1);
	
	resolvers = ft_strjoin(value, "/resolvers.txt");
	if (!check_file_exist(resolvers))
	{
		ft_putstr(ANSI_COLOR_BOLD_BLUE"\nDo you want to install dnsvalidator? and download resolvers.txt file? [y/n]: "ANSI_COLOR_RESET);
		answer = get_next_line(0);	
		if (answer[strlen(answer) - 1] == '\n')
			answer[strlen(answer) - 1] = '\0';
		if (!string_to_bool(answer))
			return (ft_putstr(ANSI_COLOR_YELLOW"should install dnsvalidator and download resolvers.txt file to do the resolution\n"ANSI_COLOR_RESET), free(answer), 0);
		free(answer);
		exec_command("git clone https://github.com/vortexau/dnsvalidator.git", 0);
		exec_command("cd dnsvalidator; sudo python3 setup.py install && cd ..", 0);
		exec_command("dnsvalidator -tL https://public-dns.info/nameservers.txt -threads 100 -o $HOME/resolvers.txt", 0);
	}
	free(resolvers);
	resolvers = ft_strjoin(value, "/subdomains.txt");
	if (!resolvers)
		return (perror("scprecon"), 1);
	if (!check_file_exist(resolvers))
	{
		ft_putstr(ANSI_COLOR_YELLOW"Do you want to download subdomains.txt file? [y/n]: "ANSI_COLOR_RESET);
		answer = get_next_line(0);
		if (!string_to_bool(answer))
			return (ft_putstr(ANSI_COLOR_YELLOW"should download subdomains.txt file to do the resolution\n"ANSI_COLOR_RESET), free(answer), 0);
		free(answer);
		exec_command("wget https://wordlists-cdn.assetnote.io/data/manual/best-dns-wordlist.txt -O $HOME/subdomains.txt", 0);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
		{
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			path = ft_join_path("./output/", line, "-resolve.txt");
			command = ft_strjoin("$HOME/go/bin/puredns bruteforce $HOME/subdomains.txt ", line);
			command = ft_strjoin_free(command, " -r $HOME/resolvers.txt -w ");
			command = ft_strjoin_free(command, path);
			if (check_file_exist(path))
			{
				old_path = ft_strjoin(path, ".old");
				rename_file = ft_join_path("mv ", path, " ");
				rename_file = ft_strjoin_free(rename_file, old_path);
				exec_command(rename_file, 0);
			}
			exec_command(command, 0);
			compare_files(old_path, path, discord_url);
			free(command);
			free(path);
			free(old_path);
			free(rename_file);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (free(resolvers), 0);
}