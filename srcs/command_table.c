#include "minishell.h"

t_token	*fill_array(int args_count, t_table *cmd_table, t_token *start)
{
	char	**args;
	int		i;

	args = NULL;
	i = 0;
	args = malloc(sizeof(char *) * (args_count + 1));
	if (!args)
		return (NULL);
	while (i < args_count && start)
	{
		if (start->type < PIPE)
		{
			args[i] = strdup(start->string);
			start = start->next;
			i++;
		}
		if (!start || start->type == PIPE)
			break ;
		while (start != NULL && start->type > PIPE)
			start = start->next->next;
	}
	args[i] = NULL;
	cmd_table->array = args;
	return (start);
}

t_token	*fill_cmd(t_table *cmd_table, t_token *current)
{
	int	args_count;

	args_count = count_args(current, cmd_table);
	if (args_count == 0)
	{
		while (current)
		{
			if (current->type == PIPE)
				return (current);
			current = current->next;
		}
		return (NULL);
	}
	if (current->type < PIPE)
		cmd_table->cmd = strdup(current->string);
	current = fill_array(args_count, cmd_table, current);
	return (current);
}

void	is_redir(t_token *current, t_table *cmd_table)
{
	int	fd;

	if (current->type == IN_REDIR)
	{
		current = current->next;
		if (cmd_table->infile)
			free(cmd_table->infile);
		cmd_table->infile = strdup(current->string);
	}
	else if (current->type == OUT_REDIR || current->type == APPEND)
	{
		if (current->type == APPEND)
			cmd_table->append_flag = 1;
		else
			cmd_table->append_flag = 0;
		current = current->next;
		if (cmd_table->outfile)
		{
			fd = open(cmd_table->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
				perror("open output file");
			free(cmd_table->outfile);
		}
		cmd_table->outfile = strdup(current->string);
	}
}

void	is_heredoc(t_token *current, t_table *cmd_table)
{
	t_hdoc	*hdoc_list;

	if (cmd_table->hdoc == NULL)
		init_hdoc(cmd_table);
	else
		hdoc_list = create_next_hdoc(cmd_table->hdoc);
	cmd_table->heredoc_flag = 1;
	current = current->next;
	hdoc_list = lst_last_hdoc(cmd_table->hdoc);
	if (current->type == SINGLE_Q || current->type == DOUBLE_Q)
		hdoc_list->heredoc_flag = 2;
	else
		hdoc_list->heredoc_flag = 1;
	if (cmd_table->infile != NULL)
		free(cmd_table->infile);
	hdoc_list->name = get_hdoc_name(cmd_table);
	hdoc_list->delim = strdup(current->string);
	cmd_table->infile = strdup(hdoc_list->name);
}

int	fill_table(t_token **token, t_table *cmd_table)
{
	t_token	*current;
	t_table	*next;

	current = *token;
	init_table(cmd_table);
	while (current)
	{
		if (current->type == PIPE)
		{
			next = malloc(sizeof(t_table));
			if (!next)
				return (-1);
			init_table(next);
			cmd_table->next = next;
			next->prev = cmd_table;
			cmd_table = cmd_table->next;
			current = current->next;
		}
		else if (current->type != PIPE)
			current = fill_cmd(cmd_table, current);
		empty_cmd_str(cmd_table);
	}
	return (1);
}
