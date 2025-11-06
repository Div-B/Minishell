#include "minishell.h"

int	handle_hdocs(t_table *cmd_table, t_env **env_list)
{
	t_table	*current;
	int		fd;

	current = cmd_table;
	fd = dup(STDIN_FILENO);
	while (current)
	{
		if (exec_heredoc(current, env_list) == 1)
		{
			dup2(fd, STDIN_FILENO);
			set_exit_status(env_list, 1);
			return (1);
		}
		dup2(fd, STDIN_FILENO);
		set_exit_status(env_list, 0);
		current = current->next;
	}
	return (0);
}

int	init_com(const char *cmd, t_env **e_list, t_token **t_list, t_table **c_tab)
{
	*t_list = NULL;
	*c_tab = malloc(sizeof(t_table));
	if (!(*c_tab))
	{
		perror("malloc error");
		return (0);
	}
	init_table(*c_tab);
	if (tokenize(t_list, cmd, e_list) == 0)
		return (0);
	if (*t_list == NULL)
	{
		dprintf(2,"Empty token list\n");
		return (0);
	}
	if (fill_table(t_list, *c_tab) != 1)
	{
		dprintf(2,"Failed to fill command table\n");
		return (0);
	}
	if (handle_hdocs(*c_tab, e_list) == 1)
		return (0);
	sig_child();
	return (1);
}

int	init_check(const char *cmd, t_env **env, t_token **tok, t_table **cmd_tab)
{
	t_table	*cmd_list;

	if (init_com(cmd, env, tok, cmd_tab) == 0)
		return (0);
	cmd_list = *cmd_tab;
	if (cmd_list->cmd != NULL
		&& is_builtin(cmd_list->cmd) && cmd_list->next == NULL)
	{
		handle_builtin(cmd_list, *tok, env);
		return (0);
	}
	return (1);
}
