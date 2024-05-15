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

void	compare_files(const char *old_file, const char *new_file)
{
	HashTable	*oldTable;
	HashTable	*newTable;
	Node		*current;

	oldTable = create_table();
	newTable = create_table();
	read_file_to_table(old_file, oldTable);
	read_file_to_table(new_file, newTable);
	printf("Subdomains in %s not present in %s:\n", new_file, old_file);
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
	{
		current = newTable->table[i];
		while (current != NULL)
		{
			if (!exists(oldTable, current->subdomain))
			{
				printf("%s\n", current->subdomain);
			}
			current = current->next;
		}
	}
	free_table(oldTable);
	free_table(newTable);
}
