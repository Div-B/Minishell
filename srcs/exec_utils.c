#include "minishell.h"

int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list != NULL)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

void	setup_io_redirections(t_table *cmd_table, t_io_fds *io_fds)
{
	if (io_fds->in_fd != STDIN_FILENO)
	{
		dup2(io_fds->in_fd, STDIN_FILENO);
		close(io_fds->in_fd);
	}
	if (cmd_table->next != NULL)
	{
		dup2(io_fds->pipe_fd[1], STDOUT_FILENO);
		close(io_fds->pipe_fd[1]);
		close(io_fds->pipe_fd[0]);
	}
	if (handle_redirections(cmd_table) == -1)
		exit(EXIT_FAILURE);
}

void	exec_proc(t_table *c_tab, t_io_fds *io_fds, t_token *tok, t_env **env)
{
	char	*command_path;
	char	**args;
	char	**envp;

	setup_io_redirections(c_tab, io_fds);
	if (c_tab->cmd == NULL)
		exit(EXIT_SUCCESS);
	if (is_builtin(c_tab->cmd))
	{
		handle_builtin(c_tab, tok, env);
		exit(EXIT_SUCCESS);
	}
	args = c_tab->array;
	command_path = construct_command_path(c_tab->cmd,*env);
	if (command_path == NULL)
	{
		dprintf(2,"Command not found in PATH\n");
		exit(127);
	}
	envp = env_list_to_envp(*env);
	if (envp == NULL)
		exit(EXIT_FAILURE);
	execve(command_path, args, envp);
	perror("execve failed");
	exit(EXIT_FAILURE);
}

static void	cleanup_parent_process(int *in_fd, int *pipe_fd, t_table *cmd_table)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd_table->next != NULL)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

void	exec_child(t_table *c_tab, t_io_fds *io_fds, t_token *tok, t_env **env)
{
	pid_t	pid;

	if (c_tab->next != NULL)
		setup_pipe(io_fds);
	pid = fork();
	if (pid == -1)
	{
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
		exec_proc(c_tab, io_fds, tok, env);
	else
		cleanup_parent_process(&io_fds->in_fd, io_fds->pipe_fd, c_tab);
}
