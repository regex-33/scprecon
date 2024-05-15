#include "minishell.h"

void print_banner() {
    printf("                   _____       __    __          __\n");
    printf("                  / ___/__  __/ /_  / /__  _____/ /_\n");
    printf("                  \\__ \\/ / / / __ \\/ / _ \\/ ___/ __/\n");
    printf("                 ___/ / /_/ / /_/ / /  __/ /  / /_\n");
    printf("                /____/\\__,_/_.___/_/\\___/_/   \\__/\n");
    printf("\n");
    printf("             Author: youssef achtatal \n");
    printf("                           Version: 1.4.7\n");
}

void print_usage(const char *prog_name) {
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -u, --url <target>          Domain to monitor. E.g: yahoo.com\n");
    printf("  -q, --question <true|false> Disable user input questions\n");
    printf("  -d, --delete <domain>       Domain to remove from the monitored list. E.g: yahoo.com\n");
    printf("  -t, --processes <number>      Number of concurrent processes to use. Default: 10\n");
    printf("  -r, --resolve               Perform DNS resolution.\n");
    printf("  -l, --logging               Enable Discord-based error logging.\n");
    printf("  -a, --list                  Listing all monitored domains.\n");
    printf("  -m, --reset                 Reset everything.\n");
    printf("  -h, --help                  Show this help message and exit.\n");
}

int wait_process(void)
{
    int status;
    pid_t pid;

    pid = waitpid(-1, &status, 0);
    if (pid == -1)
    {
        if (errno == ECHILD)
            return 1;
        else
            return (perror("philo"), 1);
    }
    if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
        return (1);
    return 0;

}

int fork_processes(int num_commands, t_list *commands)
{
    int i;
    pid_t pid;
    int status;
    char *command;
    t_list *tmp;

    i = 0;
    tmp = commands;
    while (i < num_commands && tmp)
    {
        pid = fork();
        if (pid < 0)
            return (perror("fork"), 1);
        else if (pid == 0)
        {
            command = tmp->content;
            /*  here i will execut my  minishell */
            exec_command(command);
            exit(0);
        }
        tmp = tmp->next;
        i++;
        if (i == num_commands)
        {
            if (!wait_process())
                return (1);
            i = 0;
        }
    }
    return (0);
}

int main(int argc, char *argv[])
{
    int opt;
    int option_index = 0;
    char *target = NULL;
    char *remove_domain = NULL;
    int processes = 10;
    int question = 1;
    int resolve = 0;
    int logging = 0;
    int listing = 0;
    int reset = 0;

    static struct option long_options[] = {
        {"url", required_argument, 0, 'u'},
        {"question", optional_argument, 0, 'q'},
        {"delete", required_argument, 0, 'd'},
        {"processes", required_argument, 0, 'p'},
        {"resolve", no_argument, 0, 'r'},
        {"logging", no_argument, 0, 'l'},
        {"list", no_argument, 0, 'a'},
        {"reset", no_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    print_banner();

    while ((opt = getopt_long(argc, argv, "u:q::d:t:rlamh", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'u':
                target = optarg;
                break;
            case 'q':
                if (optarg) {
                    question = atoi(optarg);
                } else {
                    question = 1;
                }
                break;
            case 'd':
                remove_domain = optarg;
                break;
            case 'p':
                processes = atoi(optarg);
                break;
            case 'r':
                resolve = 1;
                break;
            case 'l':
                logging = 1;
                break;
            case 'a':
                listing = 1;
                break;
            case 'm':
                reset = 1;
                break;
            case 'h':
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("Options set:\n");
    if (target) printf("  Target: %s\n", target);
    printf("  Disable questions: %d\n", question);
    if (remove_domain) printf("  Domain to remove: %s\n", remove_domain);
    printf("  processes: %d\n", processes);
    printf("  Resolve: %d\n", resolve);
    printf("  Logging: %d\n", logging);
    printf("  Listing: %d\n", listing);
    printf("  Reset: %d\n", reset);

    int fd;
    char *line = NULL;
    char *file_name = "commands.txt";
    t_list *commands = NULL;
    t_list *new_commands = NULL;

    int num_commands = 0;
    fd = open(file_name, O_RDONLY);
    if (fd == -1)
        return (perror("scprecon"), 1);
    line = get_next_line(fd);
    while (line)
    {
        if (*line != '\n' && *line != '\0' && *line != '#')
        {
            num_commands++;
            new_commands = ft_lstnew(strdup(line));
            if (!new_commands)
                return (ft_lstclear_libft(&commands, free), perror("scprecon"), 1);
            ft_lstadd_back_libft(&commands, new_commands);
        }
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    if (num_commands > processes)
        processes  = num_commands;
    fork_processes(processes, commands);
    ft_lstclear_libft(&commands, free);
    

    return 0;
}