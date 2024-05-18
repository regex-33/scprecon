#ifndef RECON_H
# define RECON_H

#include "minishell.h"
#include "types.h"


/*  compare subdomains  */

int	compare_files(char *old_file, char *new_file, char *discord_webhook_url);
char *create_message(const char *new_file, char *text);
int add_new_subdomains(char *file);

/*  create files  */

int	open_files_and_add_content_to_alldomains(t_list *commands, char *target, char *discord_url);
int	add_content_of_files_to_alldomains(char *discord_url, char *target, int num_commands);

/*  create processes  */

int	fork_processes(int num_commands, t_list *commands, int save_all);
int	wait_process(void);
int fork_and_expand_and_exec_commands(int save_all, int *processes, char *target);
int expand_command(char *line, int num_commands, t_list **commands, const char *target);

/*  help.c  */

void	print_usage(const char *prog_name);
void    print_banner(void);
void reset_all(int save_all);

/*  parse domainname    */

char *create_mv_command(char *path, char *old_path);
int ft_count_coomands(char *file_name);
int check_files_if_all_exist(int num_commands);
int	validate_domain_name(const char *domain);
char	*replace(char const *const original, char const *const pattern, char const *const replacement);
int check_domain(char *domain);
void parse_arguments(int argc, char *argv[], char **target, char **remove_domain, char **discord_url, int *processes, int *resolve, int *save_all, int *listing, int *reset, int *remove_flag);

/*  post to discord */

void send_discord_file(const char *webhook_url, const char *file_path, const char *message);

/*  recon   utils   */

int list_domains(void);
int add_domain_to_list(char *domain);
bool string_to_bool(const char *v);
char *ft_join_path(char *path, char *domain, char *ext);
int	check_file_exist(const char *file_name);

/*  remove  */

int remove_domain_in_list(const char *domain_to_delete);

/*  resolve domains */

int	resolve_domains(char *discord_url);

/*  scprecon.c  */

void parse_arguments(int argc, char *argv[], char **target, char **remove_domain, char **discord_url, int *processes, int *resolve, int *save_all, int *listing, int *reset, int *remove_flag);
void initialize();
void handle_reset(int save_all);
void handle_listing();
void handle_remove_domain(char *remove_domain);
void handle_discord_url_check(char *discord_url);
void handle_domain_processing(char *target, char *discord_url, int save_all, int processes);
void handle_file_operations(char *discord_url, int save_all, int processes);
void handle_remove_files();
void handle_domain_resolution(char *discord_url);

#endif