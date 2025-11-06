#include "minishell.h"

char *join2(const char *a, const char *b) {
    if (!a || !b) return NULL;
    size_t la = strlen(a), lb = strlen(b);
    char *s = (char*)malloc(la + lb + 1);
    if (!s) return NULL;
    memcpy(s, a, la);
    memcpy(s + la, b, lb);
    s[la + lb] = '\0';
    return s;
}
int wait_for_children(t_table *cmd_table)
{
    (void)cmd_table;
    int last_status = 0, status;

    for (;;) {
        pid_t pid = waitpid(-1, &status, 0);
        if (pid < 0) {
            if (errno == EINTR) continue;
            if (errno == ECHILD) break;
            break;                         
        }
        if (WIFEXITED(status)) {
            last_status = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            if (sig != SIGPIPE)
                last_status = 128 + sig;
        }
    }
    return last_status;
}
t_env *create_env_node(const char *prefix, char *exit_status_str, t_env **env_list)
{
    t_env *new_node = (t_env*)malloc(sizeof(t_env));
    if (!new_node) { free(exit_status_str); return NULL; }

    new_node->var = join2(prefix, exit_status_str);
    free(exit_status_str);
    if (!new_node->var) { free(new_node); return NULL; }

    new_node->next = *env_list;
    *env_list = new_node;
    return new_node;
}


char *int_to_str(int value)
{
    char buf[32];
    value &= 0xFF;

    int n = snprintf(buf, sizeof(buf), "%d", value);
    if (n < 0 || n >= (int)sizeof(buf)) return NULL;
    return strdup(buf);
}
void set_exit_status(t_env **env_list, int last_status)
{
    const char *prefix = "?=";
    char *exit_status_str = int_to_str(last_status);
    if (!exit_status_str) return;

    for (t_env *cur = *env_list; cur; cur = cur->next) {
        if (strncmp(cur->var, prefix, strlen(prefix)) == 0) {
            free(cur->var);
            cur->var = join2(prefix, exit_status_str);
            free(exit_status_str);
            return;
        }
    }
    create_env_node(prefix, exit_status_str, env_list);
}