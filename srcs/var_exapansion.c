#include "minishell.h"

int	assign_var(t_token **token, t_env *env_list)
{
	t_token	*current;
	t_token	*to_del;
	char	*expanded_str;

	current = *token;
	while (current != NULL)
	{
		to_del = NULL;
		if (expansion_required(current) == 1)
		{
			while (expansion_required(current) == 1)
			{
				expanded_str = expand_var(current->string, env_list);
				if (expanded_str == NULL || expanded_str[0] == '\0')
					to_del = current;
				free(current->string);
				current->string = expanded_str;
			}
		}
		current = current->next;
		if (to_del)
			del_token_node(to_del);
	}
	return (0);
}

char	*handle_remainder(char *string, int index, char *final)
{
	char	*remainder;
	char	*new_final;
	int		j;

	j = 0;
	remainder = malloc(strlen(string));
	if (!remainder)
	{
		free(final);
		return (NULL);
	}
	while (string[index] != '\0')
	{
		remainder[j] = string[index];
		j++;
		index++;
	}
	remainder[j] = '\0';
	new_final = join2(final, remainder);
	free(final);
	free(remainder);
	return (new_final);
}

char	*add_to_final(char *expansion, char *og)
{
	char	*final;

	if (og != NULL)
		final = join2(og, expansion);
	else
		final = strdup(expansion);
	free(expansion);
	return (final);
}

char	*get_initial_str(char *string, int *i)
{
	char	*og;

	og = malloc(strlen(string));
	if (!og)
		return (NULL);
	while (string[*i] != '\0' && !(string[*i] == '$'
			&& is_var_char(string[*i + 1]) == 1))
	{
		og[*i] = string[*i];
		(*i)++;
	}
	og[*i] = '\0';
	return (og);
}

char	*expand_var(char *string, t_env *env_list)
{
	char	*og;
	char	*expansion;
	char	*final;
	char	*name;
	int		i;

	i = 0;
	og = get_initial_str(string, &i);
	name = extract_var_name(string, &i);
	expansion = expand_variable(name, env_list);
	free(name);
	if (expansion != NULL)
		final = add_to_final(expansion, og);
	else
	{
		final = strdup(og);
		free(expansion);
	}
	if (string[i] != '\0')
		final = handle_remainder(string, i, final);
	free(og);
	return (final);
}
