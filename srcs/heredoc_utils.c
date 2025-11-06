#include "minishell.h"

void	handle_heredoc_files(t_table *cmd_table)
{
	t_table	*current_cmd;
	t_hdoc	*hdoc_list;

	current_cmd = cmd_table;
	while (current_cmd != NULL)
	{
		if (current_cmd->hdoc != NULL)
		{
			hdoc_list = current_cmd->hdoc;
			while (hdoc_list)
			{
				if (access(hdoc_list->name, F_OK) == 0)
				{
					if (unlink(hdoc_list->name) == -1)
						perror("unlink heredoc file");
				}
				hdoc_list = hdoc_list->next;
			}
		}
		current_cmd = current_cmd->next;
	}
}

void	setup_heredoc_sig(void)
{
	g_exit_status = 0;
	signal(SIGINT, heredoc_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	need_expansion(t_hdoc *hdoc, char *line, int fd, t_env **env_list)
{
	char	*tmp;

	if (hdoc->heredoc_flag == 1)
	{
		tmp = heredoc_var_expansion(line, env_list);
		if (tmp)
		{
			dprintf(fd,"%s\n",tmp);
			free (tmp);
		}
		else
			dprintf(fd,"%s\n",line);
	}
	else
		dprintf(fd,"%s\n",line);
	return ;
}

int	open_heredocfd(t_hdoc *hdoc)
{
	int	fd;

	fd = open(hdoc->name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		perror("heredoc file error:");
	return (fd);
}

int	heredoc_loop(t_hdoc *hdoc, int fd, t_env **env_list)
{
	char	*line;
	int		delim_flag;

	delim_flag = 0;
	while (delim_flag == 0)
	{
		line = readline("heredoc> ");
		if (g_exit_status == 130)
		{
			close(fd);
			return (1);
		}
		if (line == NULL)
			break ;
		if (strlen(line) == 0)
			continue ;
		if (strncmp(line, hdoc->delim, strlen(hdoc->delim) + 1) == 0)
			delim_flag = 1;
		else
			need_expansion(hdoc, line, fd, env_list);
		free(line);
	}
	return (0);
}
