#include "minishell.h"

void	init_table(t_table *cmd_table)
{
	if (!cmd_table)
	{
		cmd_table = malloc(sizeof(t_table));
		if (!cmd_table)
		{
			perror("malloc error:");
			return ;
		}
	}
	cmd_table->cmd = NULL;
	cmd_table->hdoc = NULL;
	cmd_table->heredoc_flag = 0;
	cmd_table->append_flag = 0;
	cmd_table->infile = NULL;
	cmd_table->outfile = NULL;
	cmd_table->array = NULL;
	cmd_table->next = NULL;
	cmd_table->errfile = NULL;
}

int	count_args(t_token *current, t_table *cmd_table)
{
	int	arg_count;

	arg_count = 0;
	while (current != NULL)
	{
		if (current->type < PIPE)
		{
			arg_count++;
			current = current->next;
		}
		else if (current->type == HEREDOC)
		{
			is_heredoc(current, cmd_table);
			current = current->next->next;
		}
		else if (current->type == IN_REDIR || current->type == OUT_REDIR
			|| current->type == APPEND)
		{
			is_redir(current, cmd_table);
			current = current->next->next;
		}
		else if (current->type == PIPE || current == NULL)
			break ;
	}
	return (arg_count);
}

void	empty_cmd_str(t_table *cmd_table)
{
	if (cmd_table->cmd == NULL)
	{
		if (cmd_table->array != NULL)
			cmd_table->cmd = strdup(cmd_table->array[0]);
	}
	return ;
}

void	free_args(char **args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (args == NULL)
		return ;
	while (args[i])
		i++;
	while (j < i)
	{
		free(args[j]);
		j++;
	}
	free(args);
}

void	free_cmd_table(t_table *cmd_table)
{
	t_table	*current_cmd;
	t_table	*next_cmd;

	current_cmd = cmd_table;
	while (current_cmd != NULL)
	{
		next_cmd = current_cmd->next;
		if (current_cmd->cmd)
			free(current_cmd->cmd);
		if (current_cmd->array)
			free_args(current_cmd->array);
		if (current_cmd->infile)
			free(current_cmd->infile);
		if (current_cmd->outfile)
			free(current_cmd->outfile);
		if (current_cmd->errfile)
			free(current_cmd->errfile);
		if (current_cmd->hdoc)
			free_hdoc(current_cmd->hdoc);
		free(current_cmd);
		current_cmd = next_cmd;
	}
}
