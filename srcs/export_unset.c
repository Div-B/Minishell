#include "minishell.h"

void	add_or_replace(t_env *list, char *str)
{
	t_env	*new;
	char	*equals_pos;

	equals_pos = strchr(str, '=');
	if (equals_pos == NULL)
	{
		dprintf(2,"Invalid format\n");
		return ;
	}
	if (replace_env_str(list, str) == 1)
		return ;
	new = malloc(sizeof(t_env));
	if (!new)
	{
		perror("malloc error");
		return ;
	}
	new->var = strdup(str);
	new->next = NULL;
	while (list->next != NULL)
		list = list->next;
	list->next = new;
}

int	is_exit_status(char *arg)
{
	char	*str;

	str = "?=";
	if (strncmp(arg, str, strlen(str)) == 0)
		return (0);
	else
		return (1);
}

void	exec_export(t_env *list, char **arg)
{
	int	i;

	i = 0;
	if (!arg)
		exec_env(list);
	while (arg[i])
	{
		if (!valid_identifier(arg[i]) || !is_exit_status(arg[i]))
		{
			set_exit_status(&list, 1);
			error_msg(arg[i], ": not a valid identifier\n", 1);
		}
		else if (strchr(arg[i], '='))
			add_or_replace(list, arg[i]);
		i++;
	}
	return ;
}

void	unset_node(t_env **list, t_env *node)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *list;
	prev = NULL;
	if (tmp == node)
	{
		*list = tmp->next;
		free(tmp->var);
		free(tmp);
		return ;
	}
	while (tmp != NULL)
	{
		if (tmp == node)
		{
			prev->next = tmp->next;
			free(tmp->var);
			free(tmp);
			return ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

void	exec_unset(t_env *list, char **arg)
{
	int		i;
	t_env	*node;
	t_env	*tmp;

	i = 0;
	if (!arg)
		return ;
	while (arg[i])
	{
		if (!valid_identifier(arg[i]))
			error_msg(arg[i], ": not a valid identifier\n", 1);
		else
		{
			node = list;
			while (node != NULL)
			{
				tmp = node->next;
				check_unset(node, arg[i], &list);
				node = tmp;
			}
		}
		i++;
	}
}
