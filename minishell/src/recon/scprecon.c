#include "minishell.h"
#include <regex.h>

int	validate_domain_name(const char *domain)
{
	regex_t		regex;
	int			reti;
	const char	*pattern = "^([A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)(\\.[A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)*$";
	char		msgbuf[100];

	reti = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
	if (reti)
	{
		fprintf(stderr, "Could not compile regex\n");
		return (0);
	}
	reti = regexec(&regex, domain, 0, NULL, 0);
	if (!reti)
	{
		regfree(&regex);
		return (1);
	}
	else if (reti == REG_NOMATCH)
	{
		regfree(&regex);
		return (0);
	}
	else
	{
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		regfree(&regex);
		return (0);
	}
}

char *ft_join_path(char *path, char *domain, char *ext)
{
	char *new_path;
	char *tmp;

	new_path = ft_strjoin(path, domain);
	tmp = new_path;
	new_path = ft_strjoin(new_path, ext);
	free(tmp);
	return (new_path);
}

int	resolve_domains(char *discord_url)
{
	t_list	*tmp;
	char	*command = NULL;
	int		fd;
	char	*line;
	int		new_fd;
	char	*path = NULL;
	char	*old_path = NULL;

	line = NULL;
	if (check_file_exist("./output/resolved.txt"))
		exec_command("mv ./output/resolved.txt ./output/resolved.txt.old", 0);
	new_fd = open("./output/resolved.txt", O_CREAT | O_WRONLY, 0644);
	if (new_fd == -1)
		return (perror("scprecon"), 1);
	if (!check_file_exist("$HOME/resolvers.txt"))
	{
		exec_command("git clone https://github.com/vortexau/dnsvalidator.git", 0);
		exec_command("cd dnsvalidator; sudo python3 setup.py install && cd ..", 0);
		exec_command("dnsvalidator -tL https://public-dns.info/nameservers.txt -threads 100 -o $HOME/resolvers.txt", 0);
	}
	if (!check_file_exist("$HOME/subdomains.txt"))
	{
		exec_command("wget https://wordlists-cdn.assetnote.io/data/manual/best-dns-wordlist.txt -O $HOME/subdomains.txt", 0);
	}
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
		{
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			path = ft_join_path("./output/", line, "-domains.txt");
			command = ft_strjoin("$HOME/go/bin/puredns bruteforce $HOME/subdomains.txt ", line);
			command = ft_strjoin_free(command, " -r $HOME/resolvers.txt -w ");
			command = ft_strjoin_free(command, path);
			if (check_file_exist(path))
			{
				path = ft_strjoin(path, ".old");
				old_path = ft_join_path("./output/", line, "-domains.txt.old");
				exec_command(, 0);
			}
			exec_command(command, 0);
			compare_files()
		}
		free(line);
		line = get_next_line(fd);
	}
	close(new_fd);
	compare_files("./output/resolved.txt.old", "./output/resolved.txt",
		discord_url);
	return (0);
}

void	print_banner(void)
{
	printf("                   _____       __    __          __\n");
	printf("                  / ___/__  __/ /_  / /__  _____/ /_\n");
	printf("                  \\__ \\/ / / / __ \\/ / _ \\/ ___/ __/\n")
	printf("                 ___/ / /_/ / /_/ / /  __/ /  / /_\n");
	printf("                /____/\\__,_/_.___/_/\\___/_/   \\__/\n");
	printf("\n");
	printf("             Author: youssef achtatal \n");
	printf("                           Version: 1.4.7\n");
}

void	print_usage(const char *prog_name)
{
	printf("Usage: %s [options]\n", prog_name);
	printf("Options:\n");
	printf("  -u, --url <target>          Domain to monitor. E.g: yahoo.com\n");
	printf("  -q, --question <true|false> Disable user input questions\n");
	printf("  -d,
		--delete <domain>       Domain to remove from the monitored list. E.g: yahoo.com\n");
	printf("  -t,
		--processes <number>      Number of concurrent processes to use. Default: 10\n");
	printf("  -r, --resolve               Perform DNS resolution.\n");
	printf("  -s,
		--save                  save all result in alldomains.txt file\n");
	printf("  -l, --list                  Listing all monitored domains.\n");
	printf("  -m, --reset                 Reset everything.\n");
	printf("  -n, --discord               discord webhook url.\n");
	printf("  -o,
		--output               file name have all file name of your commands commadns.\n");
	printf("  -h, --help                  Show this help message and exit.\n");
}

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
			return (perror("philo"), 1);
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

int	check_file_exist(const char *file_name)
{
	struct stat	buffer;

	return (stat(file_name, &buffer) == 0);
}

int	open_files_and_add_content_to_alldomains(t_list *commands,
		char *discord_url)
{
	t_list	*tmp;
	char	*command;
	int		fd;
	char	*line;
	int		new_fd;

	line = NULL;
	tmp = commands;
	if (check_file_exist("./output/alldomains.txt"))
		exec_command("mv ./output/alldomains.txt ./output/alldomains.txt.old",
			0);
	new_fd = open("./output/alldomains.txt", O_CREAT | O_WRONLY, 0644);
	if (new_fd == -1)
		return (perror("scprecon"), 1);
	while (tmp)
	{
		command = tmp->content;
		fd = open(command, O_RDONLY);
		if (fd == -1)
		{
			return (perror("scprecon"), 1);
		}
		line = get_next_line(fd);
		while (line)
		{
			if (*line != '\n' && *line != '\0' && *line != '#')
				ft_putstr_fd(line, new_fd);
			free(line);
			line = get_next_line(fd);
		}
		close(fd);
		tmp = tmp->next;
	}
	close(new_fd);
	compare_files("./output/alldomains.txt.old", "./output/alldomains.txt",
		discord_url);
	return (0);
}

int	add_content_of_files_to_alldomains(char *file_name, char *discord_url)
{
	int		fd;
	char	*line;
	t_list	*commands;
	t_list	*new_commands;
	int		num_commands;

	line = NULL;
	commands = NULL;
	new_commands = NULL;
	num_commands = 0;
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (perror("scprecon"), 1);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
		{
			num_commands++;
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			new_commands = ft_lstnew(strdup(line));
			if (!new_commands)
				return (ft_lstclear_libft(&commands, free), perror("scprecon"),
					1);
			ft_lstadd_back_libft(&commands, new_commands);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (open_files_and_add_content_to_alldomains(commands, discord_url));
}

int	main(int argc, char *argv[])
{
	int opt;
	int option_index = 0;
	char *target = NULL;
	char *remove_domain = NULL;
	char *discord_url = NULL;
	int processes = 10;
	int question = 1;
	int resolve = 0;
	int save_all = 0;
	int listing = 0;
	int reset = 0;
	char *output_files = NULL;

	static struct option long_options[] = {{"url", required_argument, 0, 'u'},
		{"question", optional_argument, 0, 'q'}, {"delete", required_argument,
		0, 'd'}, {"processes", required_argument, 0, 'p'}, {"resolve",
		no_argument, 0, 'r'}, {"save", no_argument, 0, 's'}, {"list",
		no_argument, 0, 'l'}, {"reset", no_argument, 0, 'm'}, {"help",
		no_argument, 0, 'h'}, {"discord", required_argument, 0, 'n'}, {"output",
		required_argument, 0, 'o'}, {0, 0, 0, 0}};

	print_banner();

	while ((opt = getopt_long(argc, argv, "u:n:o:q::d:p:rlamh", long_options,
				&option_index)) != -1)
	{
		switch (opt)
		{
		case 'u':
			target = optarg;
			break ;
		case 'n':
			discord_url = optarg;
			break ;
		case 'o':
			output_files = optarg;
			break ;
		case 'q':
			if (optarg)
				question = atoi(optarg);
			else
				question = 1;
			break ;
		case 'd':
			remove_domain = optarg;
			break ;
		case 'p':
			processes = atoi(optarg);
			break ;
		case 'r':
			resolve = 1;
			break ;
		case 's':
			save_all = 1;
			break ;
		case 'l':
			listing = 1;
			break ;
		case 'm':
			reset = 1;
			break ;
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
		default:
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	printf("Options set:\n");
	if (target)
		printf("  Target: %s\n", target);
	printf("  Disable questions: %d\n", question);
	if (remove_domain)
		printf("  Domain to remove: %s\n", remove_domain);
	printf("  Processes: %d\n", processes);
	printf("  Resolve: %d\n", resolve);
	printf("  Save All result: %d\n", save_all);
	printf("  Listing: %d\n", listing);
	printf("  Reset: %d\n", reset);
	printf("  Discord: %s\n", discord_url);
	printf("  Output: %s\n", output_files);

	/*   creat output directory if not exist */

	if (reset)
	{
		reset_all(save_all);
		return (0);
	}

	if (listing)
		if (list_domains())
			return (1);
	if (remove_domain)
		if (remove_domain_in_list(remove_domain))
			return (1);
	if (target)
	{
		if (!validate_domain_name(target))
			return (ft_putstr_fd(ANSI_COLOR_RED "Invalid domain name\n" ANSI_COLOR_RESET,
					2), 1);
		if (add_domain_to_list(target))
			return (1);
	}

	if (discord_url == NULL)
		return (ft_putstr_fd(ANSI_COLOR_RED "Discord WebHook Url is required\n" ANSI_COLOR_RESET,
				2), 1);

	struct stat st = {0};

	if (stat("./output", &st) == -1)
		mkdir("./output", 0700);

	int fd;
	char *line = NULL;
	char *file_name = "commands.txt";
	t_list *commands = NULL;
	t_list *new_commands = NULL;

	int num_commands = 0;
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (perror("scprecon"), 1);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
		{
			num_commands++;
			if (line[strlen(line) - 1] == '\n')
				line[strlen(line) - 1] = '\0';
			new_commands = ft_lstnew(strdup(line));
			if (!new_commands)
				return (ft_lstclear_libft(&commands, free), perror("scprecon"),
					1);
			ft_lstadd_back_libft(&commands, new_commands);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	if (num_commands > processes)
		processes = num_commands;
	fork_processes(processes, commands, save_all);
	if (output_files)
	{
		if (add_content_of_files_to_alldomains(output_files, discord_url))
			return (1);
	}
	if (resolve)
	{
		if (resolve_domains(discord_url))
			return (1);
	}
	ft_lstclear_libft(&commands, free);

	return (0);
}