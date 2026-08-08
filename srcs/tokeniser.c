/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 16:21:59 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

static t_token *decompose_line(char *line, const char delim)
{
	t_token *chain;
	char 	*str;
	char	delim_str[2] = {delim, '\0'};
	
	str = strtok(line, delim_str);
	chain = add_token(str, NULL);
	while (str)
	{
		str = strtok(NULL, delim_str);
		if (str != NULL)
			chain = add_token(str, chain);
	}
	while (chain->prev)
		chain = chain->prev;
	return chain;
}

static t_token *decompose_chain(t_token *start, const char delim)
{
	t_token *current = start;

	while (current)
	{
		if (strchr(current->str, delim))
		{
			if (!split_token(current, delim))
				return NULL;
		}
		current = current->next;
	}
	while (start->prev)
		start = start->prev;
	return start;
}

void tokeniser(char *line)
{
	t_token *chain;
	chain = decompose_line(line, ' ');
	chain = decompose_chain(chain, '\n');
	print_token_chain(chain);
}

