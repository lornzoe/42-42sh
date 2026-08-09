/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/09 18:04:56 by lyanga           ###   ########.fr       */
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
		if (strchr(current->str, delim))
		{
			current = split_token(current, delim);
			if (!current)
				return NULL;
			last = current;
		}
		current = current->next;
	}
	while (last->prev)
		last = last->prev;
	return last;
}

void tokeniser(char *line)
{
	t_token		*chain;
	const char	*special;

	chain = add_token(line, NULL);
	special = SHELL_SPECIAL_CHARS;
	while (*special)
	{
		chain = decompose_chain(chain, *special);
		special++;
	}
	print_token_chain(chain);
}

