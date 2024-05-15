#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void	remove_domain(const char *domain_to_delete)
{
	FILE	*domains;
	FILE	*new_file;
	char	line[256];
	char	command[256];

	if (domain_to_delete)
	{
		domains = fopen("domains.txt", "r");
		if (!domains)
		{
			perror("Error opening domains.txt");
			exit(EXIT_FAILURE);
		}
		new_file = fopen("domains_tmp.txt", "w");
		if (!new_file)
		{
			perror("Error opening temporary file");
			fclose(domains);
			exit(EXIT_FAILURE);
		}
		while (fgets(line, sizeof(line), domains))
		{
			// Remove newline character from line
			line[strcspn(line, "\n")] = 0;
			if (strcmp(line, domain_to_delete) == 0)
			{
				snprintf(command, sizeof(command), "rm -f ./output/%s.txt",
					line);
				system(command);
				printf("\n[-] %s was successfully removed from the monitored list.\n ", line);
			}
			else
				fprintf(new_file, "%s\n", line);
		}
		fclose(domains);
		fclose(new_file);
		if (rename("domains_tmp.txt", "domains.txt") != 0)
		{
			perror("Error renaming temporary file");
			exit(EXIT_FAILURE);
		}
		//exit(1);
	}
}
