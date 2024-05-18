#ifndef EXPANDING_H
#define EXPANDING_H

#include "types.h"

typedef struct s_expanding
{
    int    in_quotes;
    int    in_single_quotes;
    int     have_asterisk;
    int     more_strings;
    char    quote;
    char    *join;
    t_context *ctx;
}   	t_expanding;

char	*get_cmd_path(char	**cmd_args, char **path_dirs);

/*      expanding utils     */

void	printLinkedList(t_list *head);
void	freeLinkedList(t_list *head);

/*  asterisk functions */

int	move_temp_list_to_list(t_list **list, t_list **temp);
int	match_wildcard(const char *pattern, const char *filename);
int	expand_wildcard_add_node(t_list **temp, const char *pattern, const char *entry_name);
int expand_wildcard(const char *pattern, t_list **matches);
void	*expand_asterisk(char *command, t_list **list);

/*  single double dollar */

int	is_valid_chars(char c);
char	*get_value(char *name, char **env);
int	is_spcial_chars(char c);
int	split_and_add_to_list(t_list **list, t_expanding *expanding, char *value);
char	*extract_dollar(const char *str);
bool	hasTrailingSpaces(const char *str);

//char	*get_value(char *name);


/*  merge sort  */

void	merge(char **arr, int left, int mid, int right);
void	merge_sort(char **arr, int left, int right);

void	merge_sort_linkedlist(t_list **head);

/*  expanding */

t_list	**expand_arg_list(t_list **list, char *temp,  t_context *ctx);
int handle_regular_char(char *temp, char **join, int *i);
char	*handle_dollar_sign(t_list **list, int *i, char *temp, t_expanding *expanding);
int	init_expanding(t_expanding *expanding, char *temp, t_context *ctx);
int	handle_quotes_asterisk(t_expanding *expanding, char c, int *i);

#endif