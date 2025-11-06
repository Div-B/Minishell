#include "minishell.h"

int	exec_heredoc(t_table *cmd_table, t_env **env_list)
{
	t_table	*current;
	t_hdoc	*current_hdoc;

	current = cmd_table;
	if (current->hdoc != NULL)
	{
		current_hdoc = current->hdoc;
		while (current_hdoc != NULL)
		{
			if (heredoc_handler(current_hdoc, env_list) == 1)
				return (1);
			current_hdoc = current_hdoc->next;
		}
	}
	return (0);
}

int	line_expansion_required(char *string)
{
	int	i;

	i = 0;
	if (strchr(string, '$') != NULL)
	{
		while (string[i] != '\0')
		{
			if (string[i] == '$' && is_var_char(string[i + 1]) == 1)
				return (1);
			i++;
		}
		return (0);
	}
	return (0);
}

char	*heredoc_var_expansion(char *line, t_env **env_list)
{
	char	*expanded;
	char	*tmp;

	if (!line || !strchr(line, '$'))
		return (NULL);
	tmp = strdup(line);
	while (1)
	{
		if (line_expansion_required (tmp))
		{
			expanded = tmp;
			tmp = expand_var(expanded, *env_list);
			free(expanded);
		}
		else
			break ;
	}
	return (tmp);
}

int	init_heredoc(t_hdoc *hdoc, int *fd, int *delim_flag, char **line)
{
	setup_heredoc_sig();
	*fd = open_heredocfd(hdoc);
	if (*fd == -1)
		return (1);
	g_exit_status = 0;
	*delim_flag = 0;
	*line = NULL;
	return (0);
}

int	heredoc_handler(t_hdoc *hdoc, t_env **env_list)
{
	int		fd;
	int		delim_flag;
	char	*line;

	if (init_heredoc(hdoc, &fd, &delim_flag, &line) == 1)
		return (1);
	if (heredoc_loop(hdoc, fd, env_list) == 1)
		return (1);
	close(fd);
	return (0);
}
