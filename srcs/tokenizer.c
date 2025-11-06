#include "minishell.h"

int	assign_word(t_token **token, const char *string, int i, int *start)
{
	int		j;
	char	*str;

	j = 0;
	while (isspace(string[*start]))
		(*start)++;
	if (i - (*start) <= 0)
		return (0);
	str = malloc((i) - (*start) + 1);
	if (!str)
		return (0);
	while (i > *start)
	{
		str[j] = string[*start + j];
		j++;
		i--;
	}
	*start = *start + j + 1;
	str[j] = '\0';
	if (add_token(token, WORD, str) == 0)
		return (0);
	set_merge_flag(lst_last(*token), string, (*start));
	return (1);
}

int	assign_quote(t_token **token, const char *string, int *i, int *start)
{
	int		type;
	int		j;
	char	*str;

	j = 1;
	if (string[*i] == '\'')
		type = SINGLE_Q;
	else
		type = DOUBLE_Q;
	while (string[*start + j] != string[*i] && string[*start + j] != '\0')
		j++;
	str = malloc((j));
	if (!str)
		return (0);
	strlcpy(str, (string) + *start + 1, j);
	if (add_token(token, type, str) == 0)
		return (0);
	*i = *i + j;
	*start = *start + j + 1;
	set_merge_flag(lst_last(*token), string, (*start));
	return (1);
}

int	assign_split(t_token **token, const char *string, int *i, int *start)
{
	int		type;
	char	*str;
	int		j;
	int		token_length;

	j = 0;
	type = get_token(string, *i);
	token_length = 1;
	if (type == HEREDOC || type == APPEND)
		token_length = 2;
	str = malloc(token_length + 1);
	if (!str)
		return (0);
	while (j < token_length)
	{
		str[j] = string[*start + j];
		j++;
	}
	str[token_length] = '\0';
	if (add_token(token, type, str) == 0)
		return (0);
	*start += token_length;
	*i = *start - 1;
	return (1);
}

int	assign_token(t_token **token, const char *string)
{
	int	i;
	int	start;
	int	split;

	i = 0;
	start = 0;
	split = 0;
	while (isspace(string[i]))
		i++;
	while (string[i] || string[i] == '\0')
	{
		split = token_split(string, i);
		if (split > 0)
			quote_split(token, string, &i, &start);
		else if (split <= -1)
			redir_split(token, string, &i, &start);
		if (string[i] != '\0')
			i++;
		else
			break ;
	}
	return (1);
}

int	tokenize(t_token **token, const char *string, t_env **env_list)
{
	if (string == NULL || string[0] == '\0' || empty_string(string) == 0)
		return (0);
	if (quotation_check(string) != 0)
	{
		printf("syntax error: missing end quote\n");
		return (0);
	}
	assign_token(token, string);
	if (parser_tree(token) == 1)
		return (0);
	assign_var(token, *env_list);
	merge_tokens(token);
	del_empty_str(token);
	assign_builtin(token);
	return (1);
}
