#include "minishell.h"

void	merge_tokens(t_token **token)
{
	t_token	*current;
	char	*merged_str;

	current = *token;
	merged_str = NULL;
	while (current)
	{
		while (current->merge_next == 1)
		{
			if (current->next == NULL)
				break ;
			merged_str = join2(current->string, current->next->string);
			free(current->string);
			current->string = strdup(merged_str);
			free(merged_str);
			if (current->next->merge_next == 1)
				current->merge_next = 1;
			else
				current->merge_next = 0;
			del_token_node(current->next);
		}
		current = current->next;
	}
}

void	set_merge_flag(t_token *token, const char *string, int start)
{
	int	type;

	type = token->type;
	if (type == WORD)
	{
		if (isspace(string[start - 1]) == 0
			&& get_token(string, start - 1) == -1)
			token->merge_next = 1;
	}
	if (type == SINGLE_Q || type == DOUBLE_Q)
	{
		if (isspace(string[start]) == 0
			&& get_token(string, start) == -1)
			token->merge_next = 1;
	}
	return ;
}

void	del_token_node(t_token *lst)
{
	if (!lst)
		return ;
	if (lst->prev)
		lst->prev->next = lst->next;
	if (lst->next)
		lst->next->prev = lst->prev;
	if (lst->string)
		free(lst->string);
	free(lst);
}
