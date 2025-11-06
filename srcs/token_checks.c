#include "minishell.h"

int	quotation_check(const char *string)
{
	int	i;
	int	start_quote;

	i = 0;
	start_quote = 0;
	while (string[i])
	{
		if (string[i] == '\"' && start_quote == 0)
			start_quote = 1;
		else if (string[i] == '\'' && start_quote == 0)
			start_quote = 2;
		else if (string[i] == '\"' && start_quote == 1)
			start_quote = 0;
		else if (string[i] == '\'' && start_quote == 2)
			start_quote = 0;
		i++;
	}
	if (start_quote != 0)
		return (1);
	return (0);
}

int	empty_string(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (isspace(str[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	parser_tree(t_token **token)
{
	t_token	*current;

	current = *token;
	if (current->type == PIPE)
	{
		printf("syntax error near unexpected token '%s'\n", current->string);
		return (1);
	}
	while (current != NULL)
	{
		if (current->next == NULL && current->type >= PIPE)
		{
			printf("syntax error near unexpected token 'newline'\n");
			return (1);
		}
		if (current->type > PIPE && current->next->type >= PIPE)
		{
			printf("syntax error near unexpected token '%s'\n",
				current->next->string);
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	del_empty_str(t_token **token)
{
	t_token	*current;

	current = *token;
	while (current)
	{
		if (current->string == NULL)
		{
			current = current->next;
			del_token_node(current->prev);
		}
		else
			current = current->next;
	}
}
