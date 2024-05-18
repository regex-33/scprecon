#include "minishell.h"


char	*replace(char const *const original, char const *const pattern,
		char const *const replacement)
{
	size_t		patcnt;
	const char	*oriptr;
	const char	*patloc;
	char		*retptr;

	if (!original || !pattern || !replacement)
		return (NULL);

	size_t const replen = strlen(replacement);
	size_t const patlen = strlen(pattern);
	size_t const orilen = strlen(original);
	patcnt = 0;
	// find how many times the pattern occurs in the original string
	for ((oriptr = original); (patloc = strstr(oriptr,
				pattern)); (oriptr = patloc + patlen))
	{
		patcnt++;
	}
	{
		// allocate memory for the new string
		size_t const retlen = orilen + patcnt * (replen - patlen);
		char *const returned = (char *)malloc(sizeof(char) * (retlen + 1));
		if (returned != NULL)
		{
			// copy the original string,
			// replacing all the instances of the pattern
			retptr = returned;
			for ((oriptr = original); (patloc = strstr(oriptr,
						pattern)); (oriptr = patloc + patlen))
			{
				size_t const skplen = patloc - oriptr;
				// copy the section until the occurence of the pattern
				strncpy(retptr, oriptr, skplen);
				retptr += skplen;
				// copy the replacement
				strncpy(retptr, replacement, replen);
				retptr += replen;
			}
			// copy the rest of the string.
			strcpy(retptr, oriptr);
		}
		return (returned);
	}
}

int	validate_domain_name(const char *domain)
{
	regex_t		regex;
	int			reti;
	const char	*pattern = "^([A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)(\\.[A-Za-z0-9]([A-Za-z0-9-]{0,61}[A-Za-z0-9])?)*$";
	char		msgbuf[100];

	reti = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);
	if (reti)
		return (fprintf(stderr, "Could not compile regex\n"), 0);
	reti = regexec(&regex, domain, 0, NULL, 0);
	if (!reti)
		return (regfree(&regex), 1);
	else if (reti == REG_NOMATCH)
		return (regfree(&regex), 0);
	else
	{
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		return (regfree(&regex), 0);
	}
}

int check_files_if_all_exist(int num_commands)
{
	char	path[100];
	int		i = 0;

	while (i < num_commands)
	{
		sprintf(path, "./output/file-%d.txt", i);
		if (check_file_exist(path))
			unlink(path);
		i++;
	}
	return (1);
}


int check_domain(char *domain) 
{
    int fd;
    char *line = NULL;

    fd = open("domains.txt", O_RDONLY);
    if (fd == -1)
        return (perror("file doas not exist"), 1);
    while ((line = get_next_line(fd)))
    {
        line[strcspn(line, "\n")] = 0;
        if (!strcmp(line, domain))
        {
            // if (flag)
            //     printf(ANSI_COLOR_RED"\n[!] %s is already being monitored."ANSI_COLOR_RESET, domain);
            free(line);
            return 1;
        }
        free(line);
    }
    return 0;
}


int ft_count_coomands(char *file_name)
{
	int fd;
	char *line;
	int num_commands = 0;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (perror("scprecon"), 0);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
			num_commands++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (num_commands);
}

char *create_mv_command(char *path, char *old_path)
{
	char *mv_command = NULL;
	char *new_path = NULL;
	char *space = NULL;
	char *tmp = NULL;

	mv_command = ft_strjoin("mv ", path);
	if (!mv_command)
		return (NULL);
	tmp = mv_command;
	space = ft_strjoin(mv_command, " ");	
	if (!space)
		return (free(tmp), NULL);
	free(tmp);
	new_path = ft_strjoin(space, old_path);
	if (!new_path)
		return (free(space), NULL);
	return (free(space), new_path);
}
