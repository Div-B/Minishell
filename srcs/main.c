#include "minishell.h"

int	setup(const char *cmd, t_env **env, t_token **tok, t_table **cmd_table)
{
	if (init_check(cmd, env, tok, cmd_table) == 0)
		return (1);
	return (0);
}

void	exec_clean(t_table *cmd_table, t_token *token_list, t_env **env_list)
{
	t_io_fds	io_fds;
	t_table		*current_cmd;
	int			last_status;

	io_fds.in_fd = STDIN_FILENO;
	io_fds.pipe_fd[0] = -1;
	io_fds.pipe_fd[1] = -1;
	current_cmd = cmd_table;
	while (current_cmd != NULL)
	{
		exec_child(current_cmd, &io_fds, token_list, env_list);
		current_cmd = current_cmd->next;
	}
	last_status = wait_for_children(cmd_table);
	handle_heredoc_files(cmd_table);
	set_exit_status(env_list, last_status);
}

void	execute_command(const char *command, t_env **env_list)
{
	t_token	*token_list;
	t_table	*cmd_table;

	if (setup(command, env_list, &token_list, &cmd_table) == 1)
	{
		free_token_list(token_list);
		free_cmd_table(cmd_table);
		return ;
	}
	if (cmd_table == NULL)
		return ;
	exec_clean(cmd_table, token_list, env_list);
	free_token_list(token_list);
	free_cmd_table(cmd_table);
}

static void	command_loop(t_env **env_list)
{
	char	*command;

	while (1)
	{
		sig_parent();
		command = readline("minishell$ ");
		sig_child();
		if (command == NULL)
			break ;
		if (strlen(command) == 0)
		{
			free(command);
			continue ;
		}
		execute_command(command, env_list);
		add_history(command);
		free(command);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env_list;

	(void) argc;
	(void) argv;
	env_list = NULL;
	if (!env_init(&env_list, envp))
	{
		dprintf(2,"Failed to initialize environment variables\n");
		return (1);
	}
	command_loop(&env_list);
	free_env_list(env_list);
	return (0);
}
