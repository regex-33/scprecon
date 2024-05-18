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
		perror("Failed to open file");
		exit(EXIT_FAILURE);
	}
	while (fgets(line, sizeof(line), file))
	{
		line[strcspn(line, "\n")] = 0; // Remove newline character
		insert(hashTable, line);
	}
	fclose(file);
}

int add_new_subdomains(char *file)
{
	int	fd;
	char *line;
	int  new_fd = 0;

	fd = open("./output/new_subdomains.txt", O_RDONLY);
	if (fd == -1)
		return (perror("Failed to open file"), 1);
	new_fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (new_fd == -1)
		return (perror("Failed to open file"),1);
	line = get_next_line(fd);
	while (line)
	{
		if (*line != '\n' && *line != '\0' && *line != '#')
			ft_putstr_fd(line, new_fd);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	close(new_fd);
	return (0);
}

char *create_message(const char *new_file, char *text)
{
	char *message = NULL;
	char *join = NULL;

	if (!new_file || strlen(new_file) < 21)
		return (perror("Invalid file name"), NULL); 
	message = ft_substr(new_file, 9, strlen(new_file) - 21);
	if (!message)
		return (perror("Failed to allocate memory"), NULL);
	join = ft_strjoin(text, message);
	if (!join)
		return (free(message), perror("Failed to allocate memory"), NULL);
	return (free(message), join);
}

int	compare_files(char *old_file, char *new_file, char *discord_webhook_url)
{
	HashTable	*oldTable;
	HashTable	*newTable;
	Node		*current;
	int			fd = 0;
	int			new = 0;
	char		*message = NULL;

	oldTable = create_table();
	newTable = create_table();
	read_file_to_table(old_file, oldTable);
	read_file_to_table(new_file, newTable);
	fd = open("./output/new_subdomains.txt", O_CREAT | O_WRONLY , 0644);
	if (fd == -1)
	{
		return (perror("Failed to open file"), free_table(oldTable), free_table(newTable), exit(EXIT_FAILURE), 1);
	}
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		current = newTable->table[i];
		while (current != NULL)
		{
			if (!exists(oldTable, current->subdomain))
			{
				new = 1;
				ft_putendl_fd(current->subdomain, fd);	
			}
			current = current->next;
		}
	}
	unlink(old_file);
	if (new)
	{
		add_new_subdomains(new_file);
		message = create_message(new_file, "New subdomains found in ");
		if (!message)
			return (perror("Failed to allocate memory"), free_table(oldTable), free_table(newTable), close(fd), 1);
		send_discord_file(discord_webhook_url, "./output/new_subdomains.txt", message); 
	}
	else
	{
		message = create_message(new_file, "No new subdomains found in ");
		if (!message)
			return (perror("Failed to allocate memory"), free_table(oldTable), free_table(newTable), close(fd), 1);
		send_discord_file(discord_webhook_url, "./output/new_subdomains.txt", message); 
	}
	return (system("rm ./output/new_subdomains.txt"), free(message), close(fd), free_table(oldTable), free_table(newTable), 0); 
}
