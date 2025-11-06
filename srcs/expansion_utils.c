#include "minishell.h"
int expansion_required(t_token *token)
{
    if (!token || !token->string)
		return 0;
    if (token->type >= PIPE || token->type == SINGLE_Q)
		return 0;
    if (token->prev && token->prev->type == HEREDOC)
		return 0;
    const char *s = token->string;
    for (int i = 0; s[i]; ++i)
        if (s[i] == '$' && is_var_char(s[i + 1])) return 1;

    return 0;
}

int	is_var_char(char c)
{
	if (isalnum((unsigned char)c))
		return 1;
    return (c == '_' || c == '?');
}
char *env_path_finder(const char *name, t_env *list)
{
    size_t nlen = strlen(name);
    for (; list; list = list->next) {
        if (!strncmp(name, list->var, nlen) && list->var[nlen] == '=') {
            const char *val = list->var + nlen + 1; 
            return strdup(val);                     
        }
    }
    return NULL;
}
char *extract_var_name(char *s, int *idx)
{
    size_t cap = strlen(s) + 1;
    char *name = malloc(cap);
    int j = 0;
    if (!name) return NULL;

    (*idx)++;
    while (s[*idx] && is_var_char(s[*idx])) {
        name[j++] = s[*idx];
        (*idx)++;
    }
    name[j] = '\0';
    return name;
}
char	*expand_variable(char *name, t_env *env_list)
{
	char	*expansion;

	expansion = env_path_finder(name, env_list);
	if (expansion != NULL)
		return (expansion);
	else
		return (NULL);
}
