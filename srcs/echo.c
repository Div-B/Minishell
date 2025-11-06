#include "minishell.h"

static void	check_skip_newline(char **current, int *skip_newline)
{
	int	i;
	int	j;

	i = 1;
	while (current[i] != NULL)
	{
		j = 1;
		if (current[i][0] == '-' && current[i][1] == 'n')
		{
			while (current[i][j] == 'n')
				j++;
			if (current[i][j] == '\0')
			{
				*skip_newline = 1;
				i++;
			}
			else
				break ;
		}
		else
			break ;
	}
}

static void	exec_echo2(t_table *cmd_table, int skip_newline)
{
	t_table	*current_cmd;
	char	**args;
	int		i;

	current_cmd = cmd_table;
	args = current_cmd->array;
	i = 0;
	if (handle_redirections(cmd_table) == -1)
		exit(EXIT_FAILURE);
	if (args != NULL)
	{
		i++;
		if (skip_newline == 1)
			i++;
		while (args[i] != NULL)
		{
			printf("%s", args[i]);
			if (args[i + 1] != NULL)
				printf(" ");
			i++;
		}
	}
	if (skip_newline == 0)
		printf("\n");
	exit(EXIT_SUCCESS);
}

static void	execute_echo(t_table *cmd_table, int skip_newline, t_env **env_list)
{
	pid_t	pid;
	int		last_status;

	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		exec_echo2(cmd_table, skip_newline);
	else
	{
		last_status = wait_for_children(cmd_table);
		set_exit_status(env_list, last_status);
	}
}

void	handle_echo(t_table *cmd_table, t_env **env_list)
{
	char	**current;
	int		skip_newline;

	if (cmd_table == NULL || cmd_table->array == NULL)
	{
		dprintf(2, "Error: Invalid command table or empty command array\n");
		return ;
	}
	current = cmd_table->array;
	skip_newline = 0;
	check_skip_newline(current, &skip_newline);
	execute_echo(cmd_table, skip_newline, env_list);
}
