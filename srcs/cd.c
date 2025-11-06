
#include "minishell.h"

static char	*get_home_directory(t_env *env_list)
{
	char	*home_dir;

	home_dir = env_path_finder("HOME", env_list);
	if (home_dir == NULL)
		dprintf(2, "cd: HOME environment variable not set\n");
	return (home_dir);
}

static char	*get_cd_path(t_token *current, t_env *env_list)
{
	char	*path;

	path = env_path_finder(current->string, env_list);
	if (path == NULL)
	{
		path = strdup(current->string);
		if (path == NULL)
			perror("strdup");
	}
	return (path);
}

void	change_directory(const char *path, t_env *env_list)
{
	if (chdir(path) == 0)
		update_pwd(env_list);
	else
		perror("chdir");
}

void	handle_cd(t_token *head, t_env *env_list)
{
	t_token	*current;
	char	*path;

	current = head->next;
	if (current == NULL)
		path = get_home_directory(env_list);
	else
		path = get_cd_path(current, env_list);
	if (path != NULL)
	{
		change_directory(path, env_list);
		free(path);
	}
}

void	update_env(t_env *env_list, const char *name, const char *value)
{
	char	*env_str;
	char	*found_path;
	int		total_len;

	found_path = env_path_finder(name, env_list);
	if ((strncmp(name, "PWD", 3) == 0 && !found_path)
		|| (strncmp(name, "OLDPWD", 6) == 0 && !found_path))
	{
		free(found_path);
		return ;
	}
	free(found_path);
	total_len = strlen(name) + strlen(value) + 2;
	env_str = malloc(total_len);
	if (!env_str)
		return ;
	memcpy(env_str, name, strlen(name));
	env_str[strlen(name)] = '=';
	memcpy(env_str + strlen(name) + 1, value, strlen(value));
	env_str[total_len - 1] = '\0';
	replace_env_str(env_list, env_str);
	free(env_str);
}
