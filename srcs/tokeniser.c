/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/14 21:54:55 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include "token.h"
#include "libft.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static t_token *decompose_chain(t_token *start, const char delim)
{
	t_token *current = start;
	t_token *last = start;

	if (!start)
		return NULL;
	while (current)
	{
		last = current;
		while (strchr(current->str, delim) && strlen(current->str) > 1)
		{
			LOG_TRACE("decompose_chain(): splitting [%s]\n", current->str);
			current = split_token(current, delim);
			if (!current) {
				LOG_ERROR("decompose_chain(): split_token() returned NULL.\n");
				return NULL;
			}
			last = current;
		}
		current = current->next;
	}
	while (last->prev)
		last = last->prev;
	return last;
}

static t_token *merge_operator_at(t_token *op)
{
	char *joined;

	while (op->next)
	{
		joined = ft_strjoin(op->str, op->next->str);
		if (!joined)
		{
			LOG_ERROR("merge_operator_at(): allocation failed.\n");
			return NULL;
		}
		if (operator_type(joined) == TOKEN_UNKNOWN)
		{
			free(joined);
			break;
		}
		free(joined);
		LOG_TRACE("merge_operator_at(): merging [%s] + [%s]\n", op->str, op->next->str);
		if (!merge_tokens(op, op->next))
			return NULL;
	}
	//op->type = operator_type(op->str); // add this in later
	return op;
}

// just putting everything that is defined in posix
static const struct {
	const char	*str;
	t_tokentype	type;
}	g_operators[] = {
	// control operators
	{"&", TOKEN_BACKGROUND},
	{"&&", TOKEN_LOGICAL_AND},
	{"(", TOKEN_SUBSHELL_OPEN},
	{")", TOKEN_SUBSHELL_CLOSE},
	{";", TOKEN_SEPARATOR},
	{";;", TOKEN_CASE_END},
	{"\n", TOKEN_NEWLINE},
	{"|", TOKEN_PIPE},
	{"||", TOKEN_LOGICAL_OR},
	// redirect operators
	{"<", TOKEN_REDIRECT_IN},
	{">", TOKEN_REDIRECT_OUT_TRUNC},
	{">>", TOKEN_REDIRECT_OUT_APPEND},
	{">|", TOKEN_REDIRECT_OUT_CLOBBER},
	{"<&", TOKEN_REDIRECT_DUP_IN},
	{">&", TOKEN_REDIRECT_DUP_OUT},
	{"<<", TOKEN_REDIRECT_HEREDOC},
	{"<<-", TOKEN_REDIRECT_HEREDOC_STRIP},
	{"<>", TOKEN_REDIRECT_RDWR},
	{NULL, TOKEN_UNKNOWN}
};

t_tokentype	operator_type(const char *str)
{
	size_t	i;

	if (!str)
		return (TOKEN_UNKNOWN);
	i = 0;
	while (g_operators[i].str)
	{
		if (strcmp(g_operators[i].str, str) == 0)
			return (g_operators[i].type);
		i++;
	}
	return (TOKEN_UNKNOWN);
}

// basically combine operators
static t_token *merge_operators(t_token *chain)
{
	t_token *current = chain;

	while (current)
	{
		if (operator_type(current->str) != TOKEN_UNKNOWN
			&& !merge_operator_at(current))
			return NULL;
		current = current->next;
	}
	return chain;
}

// combine $ and { (${) then keep absorbing tokens till } (include it ).
static t_token *merge_params(t_token *chain)
{
	t_token *current = chain;
	int		is_close;

	while (current)
	{
		if (current->next && strcmp(current->str, "$") == 0
			&& strcmp(current->next->str, "{") == 0)
		{
			LOG_TRACE("merge_params(): merging [$] + [{]\n");
			if (!merge_tokens(current, current->next))
				return NULL;
			while (current->next)
			{
				is_close = (strcmp(current->next->str, "}") == 0);
				LOG_TRACE("merge_params(): merging [%s] + [%s]\n",
					current->str, current->next->str);
				if (!merge_tokens(current, current->next))
					return NULL;
				if (is_close)
					break;
			}
		}
		current = current->next;
	}
	return chain;
}

static t_token *merge_io_location(t_token *chain)
{
	t_token *current = chain;
	t_token *close;
	int		is_close;

	while (current)
	{
		if (strcmp(current->str, "{") == 0)
		{
			close = current->next;
			while (close && strcmp(close->str, "}") != 0)
				close = close->next;
			if (close && close->next
				&& (close->next->str[0] == '<' || close->next->str[0] == '>'))
			{
				LOG_TRACE("merge_io_location(): found valid end node.");
				LOG_TRACE("merge_io_location(): merging starting at [%s]\n", current->str);
				while (current->next)
				{
					is_close = (current->next == close);
					LOG_TRACE("merge_io_location(): merging [%s] + [%s]\n",
						current->str, current->next->str);
					if (!merge_tokens(current, current->next))
						return NULL;
					if (is_close)
						break;
				}
			}
		}
		current = current->next;
	}
	return chain;
}

static t_token *merge_quotes(t_token *chain)
{
	t_token *current = chain;
	char	quote[2];
	int		is_close;

	quote[1] = '\0';
	while (current)
	{
		if (strcmp(current->str, "\"") == 0 || strcmp(current->str, "\'") == 0)
		{
			quote[0] = current->str[0];
			LOG_TRACE("merge_quotes(): starting at %s\n", current->str);
			while (current->next)
			{
				is_close = (strcmp(current->next->str, quote) == 0);
				LOG_TRACE("merge_quotes(): merging [%s] + [%s]\n",
					current->str, current->next->str);
				if (!merge_tokens(current, current->next))
					return NULL;
				if (is_close)
					break;
				if (!is_close && current->next == NULL)
				{
					LOG_WARN("merge_quotes(): end of chain reached, quotation of [%s] was not closed.\n", quote);
					return NULL;
				}
			}
		}
		current = current->next;
	}
	return chain;
}

t_token *tokeniser(char *line)
{
	t_token		*chain;
	const char	*special;

	LOG_INFO("tokeniser()\n");
	chain = add_token(line, NULL);
	special = SHELL_SPECIAL_CHARS;
	while (*special)
	{
		chain = decompose_chain(chain, *special);
		special++;
	}
	LOG_DEBUG_CALL(print_token_chain(chain));

	// combine symbols together where possible so that they are intrepretable later as operators/special words/etc.
	if (!merge_operators(chain))
		return NULL;
	if (!merge_params(chain))
		return NULL;
	if (!merge_io_location(chain))
		return NULL;
	if (!merge_quotes(chain))
		return NULL;
	
	LOG_DEBUG_CALL(print_token_chain(chain));
	return chain;
}

