#include "minishell.h"

void	update_pwd(t_env *env_list)
{
	char	cwd[1024];
	char	*oldpwd;
	t_env	*current;

	oldpwd = NULL;
	current = env_list;
	while (current != NULL)
	{
		if (strncmp(current->var, "PWD=", 4) == 0)
		{
			oldpwd = strdup(current->var + 4);
			break ;
		}
		current = current->next;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (oldpwd != NULL)
		{
			update_env(env_list, "OLDPWD", oldpwd);
			free(oldpwd);
		}
		update_env(env_list, "PWD", cwd);
	}
}
