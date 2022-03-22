#include "../inc/minishell.h"

extern t_minishell	g_msh;

size_t	next_exp(char *token, bool print);
int		valid_exp_char(int c, bool first_char);
char	*get_varname(char *token);
char	*expand_varname(char *varname);

size_t	skip_quotes(char *expanded)
{
	size_t	i;
	size_t	count;
	int		s_quotes;
	int		d_quotes;

	s_quotes = -1;
	d_quotes = -1;
	while (expanded != NULL && expanded[i] != '\0')
	{
		if (expanded[i] == '\'')
			s_quotes *= -1;
		if (expanded[i] == '\"')
			d_quotes *= -1;
		i++;
	}
	return (count);
}

char	*expander(char *token, bool unquote)
{
	char	*expanded;
	size_t	i;

	expanded = NULL;
	i = 0;
	while (token != NULL && token[i] != '\0')
	{
		expanded = ft_strjoin_free(expanded,
				ft_substr(token, 0, next_exp(token, true)));
		token = ft_substr_free(token,
				next_exp(token, false), ft_strlen(token));
		expanded = ft_strjoin_free(expanded,
				expand_varname(get_varname(token)));
		token = ft_substr_free(token,
				ft_strlen_free(get_varname(token)) + 1, ft_strlen(token));
	}
	printf("expanded: %s|\n", expanded);
	if (unquote)
		return (quote_remover(expanded));
	return (expanded);
}

size_t	next_exp(char *token, bool print)
{
	size_t	pos;

	pos = 0;
	if (print)
		printf("next_exp: %s|\n", token);
	while (token != NULL && token[pos] != '\0')
	{
		if (token[pos] == '\'')
		{
			if (print)
				printf("next_exp, s_quote: token[%zu]: %s|\n", pos, &token[pos]);
			pos += ft_strchr_int(&token[pos + 1], '\'') + 1;
		}
		else if (token[pos] == '"')
		{
			while (token[++pos] != '\0' && token[pos] != '"')
			{
				if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
				{
					if (print)
						printf("next_exp, d_qoute: token[%zu]: %s|\n", pos, &token[pos]);
					return (pos);
				}
			}
		}
		else if (token[pos] == '$' && valid_exp_char(token[pos + 1], true))
		{
			if (print)
				printf("next_exp, $_sign: token[%zu]: %s|\n", pos, &token[pos]);
			return (pos);
		}
		if (token[pos] != '\0')
			pos++;
	}
	return (pos);
}

int	valid_exp_char(int c, bool first_char)
{
	if (ft_isalnum(c) || c == '_'
		|| (first_char == true && (c == '?' || c == '\'' || c == '"')))
		return (1);
	return (0);
}

char	*get_varname(char *token)
{
	size_t	i;
	size_t	j;

	i = 1;
	if (token == NULL || token[0] == '\0')
		return (NULL);
	while (token[i] != '\0' && token[i] != '$')
		i++;
	if (token[i + 1] == '?')
		return (ft_strdup("?"));
	else if (token[i + 1] == '\'' || token[i + 1] == '"')
		return (ft_substr(token, i + 1, ft_strchr_int(token + i + 2, token[i + 1]) + 2));
	j = 0;
	while (valid_exp_char(token[i], false))
	{
		i++;
		j++;
	}
	return (ft_substr(token, i - j, i - 1));
}

char	*expand_varname(char *varname)
{
	char	*value;

	if (varname == NULL)
		return (NULL);
	else if (ft_strcmp(varname, "?") == 0)
		value = ft_itoa(g_msh.exit_code);
	else if (varname[0] == '\'' || varname[0] == '\"')
		value = ft_strdup(varname);
	else
		value = get_var((const char *)varname);
	value = ft_strjoin_free(ft_strdup("~"), value);
	value = ft_strjoin_free(value, ft_strdup("~"));
	ft_free((void **)&varname);
	return (value);
}
