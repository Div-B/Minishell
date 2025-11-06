#include "minishell.h"

void	free_token_list(t_token *token_list)
{
	t_token	*current_token;
	t_token	*next_token;

	current_token = token_list;
	while (current_token != NULL)
	{
		next_token = current_token->next;
		if (current_token->string)
			free(current_token->string);
		free(current_token);
		current_token = next_token;
	}
}

t_token	*lst_new(int type, char *str)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node || !str)
	{
		if (node)
			free(node);
		return (NULL);
	}
	node->type = type;
	node->string = strdup(str);
	node->next = NULL;
	node->prev = NULL;
	node->merge_next = 0;
	return (node);
}

t_token	*lst_last(t_token *list)
{
	if (!list)
		return (NULL);
	while (list->next != NULL)
		list = list->next;
	return (list);
}

void	token_addback(t_token **list, t_token *node)
{
	t_token	*head;

	if (!list || !node)
		return ;
	head = *list;
	if (!head)
	{
		*list = node;
		return ;
	}
	while (head->next != NULL)
		head = head->next;
	head->next = node;
	node->prev = head;
	return ;
}

int	add_token(t_token **token_list, int type, char *string)
{
	t_token	*new;

	new = lst_new(type, string);
	if (!new)
	{
		free(string);
		return (0);
	}
	token_addback(token_list, new);
	free(string);
	return (1);
}
