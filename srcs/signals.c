#include "minishell.h"

int	g_exit_status;

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
	}
	if (sig == SIGQUIT)
	{
		g_exit_status = 131;
		printf("Quit (core dumped)\n");
	}
}

void	sig_parent(void)
{
	signal(SIGINT, restore_prompt);
	signal(SIGQUIT, SIG_IGN);
}

void	sig_child(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

void	restore_prompt(int sig)
{
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	heredoc_signal(int sig)
{
	(void)sig;
	g_exit_status = 130;
	ioctl(0, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}
