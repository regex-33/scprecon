#include "minishell.h"

void initialize()
{
    print_banner();

    struct stat st = {0};
    if (stat("./output", &st) == -1)
        mkdir("./output", 0700);
}

void handle_listing() {
    if (list_domains())
        exit(1);
}

void handle_remove_domain(char *remove_domain) {
    if (remove_domain_in_list(remove_domain))
        exit(1);
}

void handle_discord_url_check(char *discord_url) {
    if (discord_url == NULL)
        return (ft_putstr_fd(ANSI_COLOR_RED "Discord WebHook Url is required\n" ANSI_COLOR_RESET, 2));
}

void handle_file_operations(char *discord_url, int save_all, int processes) {
    int num_commands = 0;
    int fd = 0;
    char *domain = NULL;

    num_commands = ft_count_coomands("commands.txt");
    fd = open("domains.txt", O_RDONLY);
    if (fd == -1)
    {
	system("echo nooooooooooooooo > ~/scprecon/youssef");
        return (perror("scprecon"), exit(1));
    }
    domain = get_next_line(fd);
    while (domain) {
        if (*domain != '\n' && *domain != '\0' && *domain != '#') {
            if (domain[strlen(domain) - 1] == '\n')
                domain[strlen(domain) - 1] = '\0';
            check_files_if_all_exist(num_commands);
            num_commands = fork_and_expand_and_exec_commands(save_all, &processes, domain);
            if (!num_commands)
                exit(1);
            if (add_content_of_files_to_alldomains(discord_url, domain, num_commands))
                exit(1);
        }
        free(domain);
        domain = get_next_line(fd);
    }
}
void handle_domain_processing(char *target, char *discord_url, int save_all, int processes) {
    int num_commands = 0;
    num_commands = ft_count_coomands("commands.txt");
    if (!validate_domain_name(target))
        return (ft_putstr_fd(ANSI_COLOR_RED "Invalid domain name\n" ANSI_COLOR_RESET, 2), exit(1));
    if (!check_domain(target))
        if (add_domain_to_list(target))
            exit(1);
    check_files_if_all_exist(num_commands);
    num_commands = fork_and_expand_and_exec_commands(save_all, &processes, target);
    if (!num_commands)
        exit(1);
    if (add_content_of_files_to_alldomains(discord_url, target, num_commands))
        exit(1);
}

void handle_domain_resolution(char *discord_url)
{
    if (resolve_domains(discord_url))
        exit(1);
}

int main(int argc, char *argv[]) {
    char *target = NULL;
    char *remove_domain = NULL;
    char *discord_url = NULL;
    int processes = 10;
    int resolve = 0;
    int save_all = 0;
    int listing = 0;
    int reset = 0;
    int remove_flag = 0;

    parse_arguments(argc, argv, &target, &remove_domain, &discord_url, &processes, &resolve, &save_all, &listing, &reset, &remove_flag);
    initialize();

    if (reset)
		reset_all(save_all);

    if (listing)
        handle_listing();

    if (remove_domain)
        handle_remove_domain(remove_domain);

    handle_discord_url_check(discord_url);

    if (target)
        handle_domain_processing(target, discord_url, save_all, processes);
    else
        handle_file_operations(discord_url, save_all, processes);

    if (remove_flag)
		system("rm -f ./output/file-*.txt");

    if (resolve)
        handle_domain_resolution(discord_url);

    return 0;
}





