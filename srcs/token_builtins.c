#include "minishell.h"

int	is_builtin(char *str)
{
	if (!strncmp(str, "echo", 5))
		return (1);
	if (!strncmp(str, "cd", 3))
		return (1);
	if (!strncmp(str, "exit", 5))
		return (1);
	if (!strncmp(str, "pwd", 4))
		return (1);
	if (!strncmp(str, "env", 4))
		return (1);
	if (!strncmp(str, "export", 7))
		return (1);
	if (!strncmp(str, "unset", 6))
		return (1);
	return (0);
}

int	assign_builtin(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current != NULL)
	{
		if (current->type == WORD && is_builtin(current->string))
			current->type = BUILTIN;
		current = current->next;
	}
	return (0);
}

