#include "minishell.h"

void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current != NULL)
	{
		next = current->next;
		free(current->var);
		free(current);
		current = next;
	}
}

int	valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!isalpha(arg[i]) && arg[i] != '_')
		return (0);
	while (arg[i] != '=' && arg[i] != '\0')
	{
		if (!isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	error_msg(char *arg, char *msg, int code)
{
	if (code == 1 && arg)
	{
		dprintf(1,"%s",arg);
		dprintf(1,"%s",msg);
	}
}

int	check_unset(t_env *node, char *arg, t_env **list)
{
	size_t	arg_len;
	size_t	var_len;

	if (node == NULL || node->var == NULL)
		return (0);
	arg_len = strlen(arg);
	var_len = strlen(node->var);
	if (var_len >= arg_len && strncmp(node->var, arg, arg_len) \
		== 0 && node->var[arg_len] == '=')
	{
		unset_node(list, node);
		return (1);
	}
	return (0);
}

char	**env_list_to_envp(t_env *env_list)
{
	int		count;
	int		i;
	char	**envp;

	i = 0;
	count = count_env_vars(env_list);
	envp = malloc((count + 1) * sizeof(char *));
	if (envp == NULL)
		return (NULL);
	while (env_list != NULL)
	{
		envp[i] = strdup(env_list->var);
		if (envp[i] == NULL)
		{
			while (i > 0)
				free(envp[--i]);
			free(envp);
			return (NULL);
		}
		i++;
		env_list = env_list->next;
	}
	envp[i] = NULL;
	return (envp);
}
