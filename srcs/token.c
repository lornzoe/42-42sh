/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:13:29 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 16:49:46 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "libft_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_token *add_token(const char *str, t_token *end)
{
	if (!str || *str == '\0')
		return NULL;

	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->str = strdup(str);
	token->prev = end;
	token->next = NULL;
	if (end)
		end->next = token;
	return token;
}

char *get_token_str(t_token *token)
{
	if (token)
		return token->str;
	return NULL;
}

int split_token(t_token *token, const char c)
{
	char *str = get_token_str(token);

	// first find where the delim is at
	size_t delim_index = 0;
	while (str[delim_index] && str[delim_index] != c)
		delim_index++;

	// build string buffers before, for the delim, and after the delim
	t_strbuf *before = ft_sb_new();
	t_strbuf *delim = ft_sb_new();
	t_strbuf *after = ft_sb_new();
	if (!before || !delim || !after)
		return 0;

	if (delim_index > 0 
		&& !ft_sb_appendn(before, str, delim_index))
		return 0;
	if (!ft_sb_appendc(delim, c))
		return 0;
	if (!ft_sb_append(after, str + delim_index))
		return 0;

	// build the tokens
	t_token *chain;

	chain = add_token(ft_sb_finish(before), NULL);
	chain = add_token(ft_sb_finish(delim), chain);
	chain = add_token(ft_sb_finish(after), chain);

	// attach them to the current chain
	t_token *chain_end = chain;
	while (chain->prev)
		chain = chain->prev;
	t_token *chain_start = chain;

	if (token->prev)
		token->prev->next = chain_start;
	chain_start->prev = token->prev;
	if (token->next)
		token->next->prev = chain_end;
	chain_end->next = token->next;

	// destroy token
	free_token(token);

	return 1;
}

void free_token(t_token *token)
{
	if (!token)
		return;
	free(token->str);
	if (token->next && token->next->prev == token)
		token->next->prev = NULL;
	if (token->prev && token->prev->next == token)
		token->prev->next = NULL;
	free(token);
}

void free_token_chain(t_token *start)
{
	if (!start)
		return;
	t_token *current = start;
	while (current->prev)
		current = current->prev;
	while (current)
	{
		t_token *next = current->next;
		free_token(current);
		current = next;
	}
}

void print_escaped_str(const char *str)
{
    if (!str)
        return;
    while (*str)
    {
        if (*str == '\n')
            printf("\\n");
        else if (*str == '\t')
            printf("\\t");
        else if (*str == '\r')
            printf("\\r");
        else if (*str == '\\')
            printf("\\\\");
        else
            putchar(*str);
        str++;
    }
}

void print_token_chain(t_token *start)
{
	if (!start)
		return;
	printf("Token chain: ");
	t_token *current = start;
	while (current->prev)
		current = current->prev;
	while (current)
	{
		printf("[");
        print_escaped_str(current->str); // Replaces printf("[%s]", current->str);
        printf("]");		current = current->next;
	}
	printf("\n");
}
