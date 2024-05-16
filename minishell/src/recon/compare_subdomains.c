#include "minishell.h"

#define MAX_LINE_LENGTH 256
#define HASH_TABLE_SIZE 10000

typedef struct Node
{
	char		*subdomain;
	struct Node	*next;
}				Node;

typedef struct HashTable
{
	Node		**table;
}				HashTable;

// Hash function to map a string to an index
unsigned int	hash(const char *str)
{
	unsigned int	hash;

	hash = 0;
	while (*str)
		hash = (hash << 5) + *str++;
	return (hash % HASH_TABLE_SIZE);
}

// Create a new hash table
HashTable	*create_table(void)
{
	HashTable	*hashTable;

	hashTable = (HashTable *)malloc(sizeof(HashTable));
	hashTable->table = (Node **)malloc(sizeof(Node *) * HASH_TABLE_SIZE);
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		hashTable->table[i] = NULL;
	return (hashTable);
}

// Insert a subdomain into the hash table
void	insert(HashTable *hashTable, const char *subdomain)
{
	unsigned int	index;
	Node			*newNode;

	index = hash(subdomain);
	newNode = (Node *)malloc(sizeof(Node));
	newNode->subdomain = strdup(subdomain);
	newNode->next = hashTable->table[index];
	hashTable->table[index] = newNode;
}

// Check if a subdomain exists in the hash table
int	exists(HashTable *hashTable, const char *subdomain)
{
	unsigned int	index;
	Node			*current;

	index = hash(subdomain);
	current = hashTable->table[index];
	while (current != NULL)
	{
		if (strcmp(current->subdomain, subdomain) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

// Free the hash table
void	free_table(HashTable *hashTable)
{
	Node	*current;
	Node	*temp;

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		current = hashTable->table[i];
		while (current != NULL)
		{
			temp = current;
			current = current->next;
			free(temp->subdomain);
			free(temp);
		}
	}
	free(hashTable->table);
	free(hashTable);
}

void	read_file_to_table(const char *filename, HashTable *hashTable)
{
	FILE	*file;
	char	line[MAX_LINE_LENGTH];

	file = fopen(filename, "r");
	if (!file)
	{
		printf("filename: %s\n", filename);
		perror("Failed to open file 1");
		exit(EXIT_FAILURE);
	}
	while (fgets(line, sizeof(line), file))
	{
		line[strcspn(line, "\n")] = 0; // Remove newline character
		insert(hashTable, line);
	}
	fclose(file);
}

int add_new_subdomains(void)
{
	int	fd;
	char *line;
	int  new_fd = 0;

	fd = open("./output/new_subdomains.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open file 2");
		return (1);
	}
	new_fd = open("./output/alldomains.txt.old", O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (new_fd == -1)
	{
		perror("Failed to open file 3");
		return (1);
	}
	line = get_next_line(fd);
	while (line)
	{
		printf("line: %s\n", line);
		if (*line != '\n' && *line != '\0' && *line != '#')
			ft_putstr_fd(line, new_fd);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	close(new_fd);
	return (0);
}

void	compare_files(const char *old_file, const char *new_file, char *discord_webhook_url)
{
	HashTable	*oldTable;
	HashTable	*newTable;
	Node		*current;
	int			fd;
	int			new = 0;

	oldTable = create_table();
	newTable = create_table();
	read_file_to_table(old_file, oldTable);
	read_file_to_table(new_file, newTable);
	fd = open("./output/new_subdomains.txt", O_CREAT | O_WRONLY , 0644);
	if (fd == -1)
	{
		printf("i am here test\n");
		perror("Failed to open file 4");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		current = newTable->table[i];
		while (current != NULL)
		{
			if (!exists(oldTable, current->subdomain))
			{
				new = 1;
				printf("hello world %s\n", current->subdomain);
				ft_putendl_fd(current->subdomain, fd);	
			}
			current = current->next;
		}
	}
	close(fd);
	free_table(oldTable);
	free_table(newTable);
	if (new)
	{
		add_new_subdomains();
		exec_command("mv ./output/alldomains.txt.old ./output/alldomains.txt", 0);
		send_discord_file(discord_webhook_url, "./output/new_subdomains.txt", "New subdomains found!");
	}
	else
	{
		exec_command("rm ./output/alldomains.txt.old", 0);
		send_discord_file(discord_webhook_url, "./output/new_subdomains.txt", "No new subdomains found.");
	}
	exec_command("rm ./output/new_subdomains.txt", 0);
}
