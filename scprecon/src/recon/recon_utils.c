#include "minishell.h"



int list_domains(void)
{
    int fd;
    char *line = NULL;

    fd = open("domains.txt", O_RDONLY);
    if (fd == -1)
        return (perror("file doas not exist"), 1);
    while ((line = get_next_line(fd)))
    {
        printf(ANSI_COLOR_BLUE"%s"ANSI_COLOR_RESET, line);
        free(line);
    }
    return 0;
}

int add_domain_to_list(char *domain)
{
    FILE *domains;

    if (!domain)
        return 1;
    domains = fopen("domains.txt", "a");
    if (!domains)
    {
        perror("Error opening domains.txt");
        return 1;
    }
    fprintf(domains, "%s\n", domain);
    fclose(domains);
    printf(ANSI_COLOR_GREEN"\n[+] %s was successfully added to the monitored list."ANSI_COLOR_RESET, domain);
    return 0;
}

// Function to convert string to boolean
bool string_to_bool(const char *v) {
    if (strcmp(v, "yes") == 0 || strcmp(v, "true") == 0 || strcmp(v, "t") == 0 || strcmp(v, "y") == 0 || strcmp(v, "1") == 0) {
        return true;
    } else if (strcmp(v, "no") == 0 || strcmp(v, "false") == 0 || strcmp(v, "f") == 0 || strcmp(v, "n") == 0 || strcmp(v, "0") == 0) {
        return false;
    } else {
        // Throw an error if the input is not a valid boolean string
        fprintf(stderr, "Boolean value expected.\n");
        exit(EXIT_FAILURE);
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

int	check_file_exist(const char *file_name)
{
	struct stat	buffer;

	return (stat(file_name, &buffer) == 0);
}