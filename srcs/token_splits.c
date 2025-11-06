#include "minishell.h"

int	token_split(const char *string, int i)
{
	if (string[i] == '|' || string[i] == '<' || string[i] == '>')
		return (-1);
	else if (string[i] == '\0')
		return (-2);
	else if (isspace(string[i]))
		return (-3);
	else if (string[i] == '\'')
		return (2);
	else if (string[i] == '\"')
		return (3);
	return (0);
}

int	get_token(const char *c, int i)
{
	if (c[i] == '>' && c[i + 1] == '>')
		return (APPEND);
	else if (c[i] == '<' && c[i + 1] == '<')
		return (HEREDOC);
	else if (c[i] == '>')
		return (OUT_REDIR);
	else if (c[i] == '<')
		return (IN_REDIR);
	else if (c[i] == '|')
		return (PIPE);
	else
		return (-1);
}

void	quote_split(t_token **token, const char *string, int *i, int *start)
{
	int	quote;
	int	split;

	split = token_split(string, *i);
	quote = split;
	if (quote > 0)
	{
		if (*start != *i)
		{
			assign_word(token, string, *i, start);
			(*start) = (*start) - 1;
		}
		assign_quote(token, string, i, start);
	}
	return ;
}

void	redir_split(t_token **token, const char *string, int *i, int *start)
{
	int	split;
	int	first_flag;
	int	j;

	first_flag = 1;
	j = (*start) - 1;
	while (++j < (*i))
	{
		if (!isspace(string[j]))
			first_flag = 0;
	}
	split = token_split(string, *i);
	if (isspace(string[*i]) || split == -2)
		assign_word(token, string, (*i), start);
	else
	{
		if ((*start) != (*i) && first_flag == 0)
		{
			assign_word(token, string, (*i), start);
			(*start) = (*start) - 1;
		}
		if (first_flag == 1)
			(*start) = (*i);
		assign_split(token, string, i, start);
	}
}
