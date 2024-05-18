#include "minishell.h"

void print_banner(void)
{
	printf(" ___                 ___                               \n");
	printf("(  _`\\              |  _`\\                             \n");
	printf("| (_(_)   ___ _ _   | (_) )   __     ___    _     ___  \n");
	printf("`\\__ \\  /'___| '_`\\ | ,  /  /'__`\\ /'___) /'_`\\ /' _ `\\\n");
	printf("( )_) |( (___| (_) )| |\\ \\ (  ___/( (___ ( (_) )| ( ) |\n");
	printf("`\\____)`\\____) ,__/'(_) (_)`\\____)`\\____)`\\___/'(_) (_)\n");
	printf("	     | |                                       \n");
	printf("	     (_)   Author: Youssef Achtatal \n");
	printf("            \n");
}

void	print_usage(const char *prog_name)
{
	printf("Usage: %s [options]\n", prog_name);
	printf("Options:\n");
	printf("  -u, --url <target>          Domain to monitor. E.g: yahoo.com\n");
	printf("  -d, --delete <domain>       Domain to remove from the monitored list. E.g: yahoo.com\n");
	printf("  -t, --processes <number>      Number of concurrent processes to use. Default: 10\n");
	printf("  -r, --resolve               Perform DNS resolution.\n");
	printf("  -l, --list                  Listing all monitored domains.\n");
	printf("  -m, --reset                 Reset everything.\n");
	printf("  -n, --discord               discord webhook url.\n");
	printf("  -o, --remove				 remove output files. E.g:file-1.txt and leave taget-domains.txt\n"); 
	printf("  -h, --help                  Show this help message and exit.\n");
}

void reset_all(int save_all)
{
    exec_command("cd ./output/ && rm -f *.txt && rm -f *.old && cd .. && rm -f domains.txt && touch domains.txt", save_all);
    ft_printf(ANSI_COLOR_RED "\n[!] Sublert was reset successfully. Please add new domains to monitor!\n" ANSI_COLOR_RESET);
}