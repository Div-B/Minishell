#include "minishell.h"

static char	*get_path_env(t_env *env_list)
{
	char	*path;

	path = env_path_finder("PATH", env_list);
	if (path == NULL)
	{
		return NULL;
	}
	return (path);
}

static char *construct_full(const char *dir, const char *cmd_name)
{
    char *command_path;
    size_t len = strlen(dir) + 1 + strlen(cmd_name) + 1;

    command_path = malloc(len);
    if (!command_path) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    snprintf(command_path, len, "%s/%s", dir, cmd_name);
    return command_path;
}
static int is_regular_exec(const char *p) {
    struct stat st;
    if (stat(p, &st) != 0) return 0;
    return S_ISREG(st.st_mode) && access(p, X_OK) == 0;
}

static char *find_command(const char *path, const char *cmd)
{
    if (!cmd || !*cmd) return NULL;
    if (strchr(cmd, '/')) {
        return is_regular_exec(cmd) ? strdup(cmd) : NULL;
    }

    if (!path) return NULL;

    char *copy = strdup(path);
    if (!copy) return NULL;

    char *save = NULL;
    for (char *tok = strtok_r(copy, ":", &save); tok; tok = strtok_r(NULL, ":", &save)) {
        const char *dir = (*tok == '\0') ? "." : tok;
        char *full = construct_full(dir, cmd);
        if (!full) { free(copy); return NULL; }
        if (is_regular_exec(full)) { free(copy); return full; }
        free(full);
    }
    free(copy);
    return NULL;
}
char *construct_command_path(const char *cmd, t_env *env_list)
{
    if (!cmd || !*cmd) return NULL;

    if (strchr(cmd, '/')) {
        if (access(cmd, F_OK) != 0) return NULL;
        if (access(cmd, X_OK) != 0) {                      
            dprintf(2, "minishell: %s: Permission denied\n", cmd);
            exit(126);
        }
        return strdup(cmd);
    }

    char *path = get_path_env(env_list);
    char *found = find_command(path, cmd);
    free(path);
    return found; 
}
