#include "minishell.h"

void	handle_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("getcwd");
}

void	free_all(t_token **token_list, t_table **cmd_table, t_env **env_list)
{
	if (*token_list)
		free_token_list(*token_list);
	if (*env_list)
		free_env_list(*env_list);
	if (cmd_table)
		free_cmd_table(*cmd_table);
}

void	exit_multiple_args(char **args, int *exit_num)
{
	int	i;

	i = -1;
	if (args[1] && !args[2])
	{
		while (args[1][++i])
		{
			if (!isdigit(args[1][i]))
			{
				printf("exit : argument has to be numeric\n");
				(*exit_num) = 1;
				return ;
			}
			else
				(*exit_num) = atoi(args[1]);
		}
	}
	return ;
}

void	handle_exit(char **args, t_env **env_list, t_table **cmd_table,
	t_token **token_list)
{
	int		exit_num;
	char	*exit_str;

	exit_num = 1;
	if (!args[1])
	{
		exit_str = env_path_finder("?",*env_list);
		if (!exit_str)
			exit_num = 0;
		else
		{
			exit_num = atoi(exit_str);
			free(exit_str);
		}
	}
	else if (args[1] && !args[2])
		exit_multiple_args(args, &exit_num);
	if (args[1] && args[2])
		printf("exit : too many arguments\n");
	set_exit_status(env_list, exit_num);
	free_all(token_list, cmd_table, env_list);
	exit(exit_num);
	return ;
}

void	handle_builtin(t_table *cmd_list, t_token *token_list, t_env **env_list)
{
	if (strncmp(cmd_list->cmd, "echo", 5) == 0)
		handle_echo(cmd_list, env_list);
	else if (strncmp(cmd_list->cmd, "cd", 3) == 0)
		handle_cd(token_list, *env_list);
	else if (strncmp(cmd_list->cmd, "exit", 5) == 0)
		handle_exit(cmd_list->array, env_list, &cmd_list, &token_list);
	else if (strncmp(cmd_list->cmd, "pwd", 4) == 0)
		handle_pwd();
	else if (strncmp(cmd_list->cmd, "env", 4) == 0)
		exec_env(*env_list);
	else if (strncmp(cmd_list->cmd, "export", 7) == 0)
		exec_export(*env_list, &cmd_list->array[1]);
	else if (strncmp(cmd_list->cmd, "unset", 6) == 0)
		exec_unset(*env_list, &cmd_list->array[1]);
}
