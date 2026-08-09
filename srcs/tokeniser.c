/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/10 04:53:25 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include "token.h"

#include <stdio.h>
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
				LOG_WARN("decompose_chain(): split_token() returned NULL.\n");
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
	return chain;
}

