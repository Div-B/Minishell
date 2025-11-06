#include "minishell.h"

void	init_hdoc(t_table *cmd_table)
{
	t_hdoc	*hdoc;

	hdoc = malloc(sizeof(t_hdoc));
	if (!hdoc)
	{
		dprintf(2,"error creating hdoc struct");
		return ;
	}
	hdoc->heredoc_flag = 0;
	hdoc->name = NULL;
	hdoc->delim = NULL;
	hdoc->prev = NULL;
	hdoc->next = NULL;
	cmd_table->hdoc = hdoc;
}

t_hdoc	*lst_last_hdoc(t_hdoc *hdoc)
{
	while (hdoc->next != NULL)
		hdoc = hdoc->next;
	return (hdoc);
}

t_hdoc	*create_next_hdoc(t_hdoc *hdoc)
{
	t_hdoc	*next;
	t_hdoc	*tmp;

	next = malloc(sizeof(t_hdoc));
	if (!next)
	{
		dprintf(2, "error creating hdoc struct");
		return (NULL);
	}
	next->heredoc_flag = 0;
	next->name = NULL;
	next->delim = NULL;
	next->next = NULL;
	tmp = lst_last_hdoc(hdoc);
	tmp->next = next;
	next->prev = tmp;
	return (next);
}

char    *get_hdoc_name(t_table *cmd_table)
{
    static int id = 0;
    char buf[64];

    (void) cmd_table;
    snprintf(buf, sizeof(buf), "/tmp/heredoc%d", id);
    id++;
    return strdup(buf);
}

void	free_hdoc(t_hdoc *list)
{
	t_hdoc	*current;
	t_hdoc	*next;

	current = list;
	while (current != NULL)
	{
		next = current->next;
		if (current->name)
			free(current->name);
		if (current->delim)
			free(current->delim);
		free(current);
		current = next;
	}
}
