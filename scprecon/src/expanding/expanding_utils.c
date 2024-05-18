#include "minishell.h"

void	printLinkedList(t_list *head)
{
	t_list	*current;

	current = head;
	ft_printf("\nLinked List: \n");
	while (current != NULL)
	{
		ft_printf("str: %s\n", (char *)(current->content));
		current = current->next;
	}
}

void	freeLinkedList(t_list *head)
{
	t_list	*current;
	t_list	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	head = NULL;
}