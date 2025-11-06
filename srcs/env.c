#include "minishell.h"

void	exec_env(t_env *list)
{
	while (list != NULL)
	{
		if (list->var[0] != '?' || list->var[1] != '=')
		{
			write(1, list->var, strlen(list->var));
			write(1, "\n", 1);
		}
		list = list->next;
	}
}

t_env	*env_node(char *envp)
{
	t_env	*node;

	if (!envp)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
	{
		perror("malloc failed");
		return (NULL);
	}
	node->var = strdup(envp);
	node->value = NULL;
	node->next = NULL;
	node->envp = NULL;
	if (!node->var)
	{
		perror("strdup failed");
		free(node);
		return (NULL);
	}
	return (node);
}

int	env_init(t_env **env_list, char **envp)
{
	int		i;
	t_env	*current;
	t_env	*node;
	char	*exit_status_str;

	if (!env_list || !envp)
		return (0);
	current = NULL;
	i = 0;
	while (envp[i] != NULL)
	{
		node = env_node(envp[i]);
		if (!node)
			return (0);
		if (*env_list == NULL)
			*env_list = node;
		else
			current->next = node;
		current = node;
		i++;
	}
	exit_status_str = int_to_str(0);
	create_env_node("?=", exit_status_str, env_list);
	return (1);
}

int	env_exists(t_env *list, char *s)
{
	int		i;
	char	*name;

	i = 0;
	while (s[i] != '=' && s[i] != '\0')
		i++;
	name = malloc(i + 2);
	if (!name)
	{
		dprintf(1,"error assigning malloc");
		return (-1);
	}
	strlcpy(name, s, i + 2);
	while (list != NULL)
	{
		if (strncmp(list->var, name, i + 1) == 0)
		{
			list->var = s;
			free(name);
			return (1);
		}
		list = list->next;
	}
	free(name);
	return (0);
}

int	replace_env_str(t_env *list, char *str)
{
	t_env	*current;
	char	*equals_pos;

	equals_pos = strchr(str, '=');
	current = list;
	while (current != NULL)
	{
		if (strncmp(current->var, str, equals_pos - str) == 0
			&& current->var[equals_pos - str] == '=')
		{
			free(current->var);
			current->var = strdup(str);
			return (1);
		}
		current = current->next;
	}
	return (0);
}
