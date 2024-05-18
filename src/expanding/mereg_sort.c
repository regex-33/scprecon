#include "minishell.h"

void	merge(char **arr, int left, int mid, int right)
{
	int		n1;
	int		n2;
	char	**left_arr;
	char	**right_arr;

	int i, j, k;
	n1 = mid - left + 1;
	n2 = right - mid;
	// Create temporary arrays
	left_arr = (char **)malloc(n1 * sizeof(char *));
	right_arr = (char **)malloc(n2 * sizeof(char *));
	// Copy data to temporary arrays left_arr[] and right_arr[]
	for (i = 0; i < n1; i++)
		left_arr[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		right_arr[j] = arr[mid + 1 + j];
	// Merge the temporary arrays back into arr[left..right]
	i = 0;    // Initial index of first subarray
	j = 0;    // Initial index of second subarray
	k = left; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (strcmp(left_arr[i], right_arr[j]) <= 0)
		{
			arr[k++] = left_arr[i++];
		}
		else
		{
			arr[k++] = right_arr[j++];
		}
	}
	// Copy the remaining elements of left_arr[], if any
	while (i < n1)
	{
		arr[k++] = left_arr[i++];
	}
	// Copy the remaining elements of right_arr[], if any
	while (j < n2)
	{
		arr[k++] = right_arr[j++];
	}
	// Free the temporary arrays
	free(left_arr);
	free(right_arr);
}

void	merge_sort(char **arr, int left, int right)
{
	int	mid;

	if (left < right)
	{
		mid = left + (right - left) / 2;
		// Sort first and second halves
		merge_sort(arr, left, mid);
		merge_sort(arr, mid + 1, right);
		// Merge the sorted halves
		merge(arr, left, mid, right);
	}
}
/*													*/
/*				merge sort liked list 				*/
/*													*/

void	merge_linked_list(t_list **head, t_list *left, t_list *right)
{
	t_list	*result;

	result = NULL;
	if (left == NULL)
	{
		*head = right;
		return ;
	}
	else if (right == NULL)
	{
		*head = left;
		return ;
	}
	// Compare nodes and merge
	if (strcmp(left->content, right->content) <= 0)
	{
		*head = left;
		left = left->next;
	}
	else
	{
		*head = right;
		right = right->next;
	}
	result = *head;
	while (left != NULL && right != NULL)
	{
		if (strcmp(left->content, right->content) <= 0)
		{
			result->next = left;
			left = left->next;
		}
		else
		{
			result->next = right;
			right = right->next;
		}
		result = result->next;
	}
	// Append remaining nodes
	if (left != NULL)
	{
		result->next = left;
	}
	else if (right != NULL)
	{
		result->next = right;
	}
}

// Merge sort for a linked list

// Helper function to split the linked list into two halves
void	split_list(t_list *head, t_list **left, t_list **right)
{
	t_list	*slow;
	t_list	*fast;

	slow = head;
	fast = head->next;
	// Use fast and slow pointer technique to find the midpoint
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	// 'slow' is the midpoint, split the list into two halves
	*left = head;
	*right = slow->next;
	slow->next = NULL; // Terminate the left half
}

void	merge_sort_linkedlist(t_list **head)
{
	t_list	*current;
	t_list	*left;
	t_list	*right;

	current = *head;
	// Base case: if list is empty or has only one node
	if (current == NULL || current->next == NULL)
	{
		return ;
	}
	// Split the list into two halves
	split_list(current, &left, &right);
	// Recursively sort the two halves
	merge_sort_linkedlist(&left);
	merge_sort_linkedlist(&right);
	// Merge the sorted halves
	merge_linked_list(head, left, right);
}
