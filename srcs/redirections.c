#include "minishell.h"

void	setup_pipe(t_io_fds *io_fds)
{
	if (pipe(io_fds->pipe_fd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
}

static int	handle_input_redirection(t_table *cmd_table)
{
	int	in_fd;

	if (cmd_table->infile != NULL)
	{
		in_fd = open(cmd_table->infile, O_RDONLY);
		if (in_fd == -1)
		{
			perror("open input file");
			return (-1);
		}
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 input file");
			close(in_fd);
			return (-1);
		}
		close(in_fd);
	}
	return (0);
}

static int	handle_output_redirection(t_table *cmd_table)
{
	int	out_fd;

	if (cmd_table->outfile != NULL)
	{
		if (cmd_table->append_flag == 1)
			out_fd = open(cmd_table->outfile,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			out_fd = open(cmd_table->outfile,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_fd == -1)
		{
			perror("open output file");
			return (-1);
		}
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 output file");
			close(out_fd);
			return (-1);
		}
		close(out_fd);
	}
	return (0);
}

int	handle_redirections(t_table *cmd_table)
{
	if (handle_input_redirection(cmd_table) == -1)
		return (-1);
	if (handle_output_redirection(cmd_table) == -1)
		return (-1);
	return (0);
}
