#include "minishell.h"

int	my_putchar(int c)
{
	return (write(1, &c, 1));
}

void	handle_interrupt(int sig)
{
	(void)sig;
	// if (!isatty(STDIN_FILENO)) // 	return ;
	// tcgetattr(STDIN_FILENO, &old_term);
	// ft_memcpy(&new_term, &old_term, sizeof(struct termios));
	// new_term.c_lflag &= ~(ICANON | ECHO);
	// tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	// write(1, "\033[6n", 4);
	// int x, y;
	// read_cursor_pos(&x, &y);
	// tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	// rl_replace_line("", 1);
	// ft_printf("\n");
    // rl_on_new_line();
	// rl_redisplay();
	//tputs(tgetstr("ce", NULL), 1, my_putchar);
	get_status(1, SET_STATUS);
	rl_replace_line("", 1);
	tputs(tgetstr("do", NULL), 1, my_putchar);
	if (get_state(0, 0) == ON_PROMPT)
	{
		rl_on_new_line();
		rl_replace_line("", 1);
		char *prompt = get_prompt_with_user_details(1);
		ft_printf("%s", prompt);
		rl_redisplay();
	}
	//tputs(tgetstr("cr", NULL), 1, my_putchar);
	// tputs(tgetstr("ce", NULL), 1, my_putchar);
}

void	handle_quit(int sig)
{
	(void)sig;
	if (get_state(0, 0) == ON_EXEC)
		ft_putendl_fd("Quit: 3", 1);
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
	//get_status(131, SET_STATUS);
}