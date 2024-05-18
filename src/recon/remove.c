#include "minishell.h"

int	remove_domain_in_list(const char *domain_to_delete)
{
	FILE	*domains;
	FILE	*new_file;
	char	line[256];
	char	command[256];

	if (domain_to_delete)
	{
		domains = fopen("domains.txt", "r");
		if (!domains)
			return (perror("Error opening domains.txt"), 1);
		new_file = fopen("domains_tmp.txt", "w");
		if (!new_file)
		{
			perror("Error opening temporary file");
			return (fclose(domains), 1);
		}
		while (fgets(line, sizeof(line), domains))
		{
			line[strcspn(line, "\n")] = 0;
			if (strcmp(line, domain_to_delete) == 0)
			{
				snprintf(command, sizeof(command), "rm -f ./output/%s.txt",
					line);
				system(command);
				printf(ANSI_COLOR_YELLOW"\n[-] %s was successfully removed from the monitored list.\n "ANSI_COLOR_RESET, line);
			}
			else
				fprintf(new_file, "%s\n", line);
		}
		fclose(domains);
		fclose(new_file);
		if (rename("domains_tmp.txt", "domains.txt") != 0)
			return (perror("Error renaming temporary file"), 1);
	}
	return 0;
}
