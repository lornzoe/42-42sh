/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:13:29 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/10 06:38:19 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "log.h"
#include "libft.h"
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
	token->type = TOKEN_UNKNOWN;
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

t_token *split_token(t_token *token, const char c)
{
	char *str;
	char *delim_pos;
	char *before;
	char *delim;
	t_token *chain_start;
	t_token *chain_end;
	t_token *node;

	str = get_token_str(token);
	delim_pos = strchr(str, c);
	before = ft_substr(str, 0, delim_pos - str);
	delim = ft_substr(str, delim_pos - str, 1);
	if (!before || !delim)
	{
		free(before);
		free(delim);
		return NULL;
	}

	// build the tokens (before/after are omitted if empty)
	chain_start = add_token(before, NULL);
	free(before);
	chain_end = chain_start;
	node = add_token(delim, chain_end);
	free(delim);
	if (!chain_start)
		chain_start = node;
	chain_end = node;
	node = add_token(delim_pos + 1, chain_end);
	if (node)
		chain_end = node;

	// attach them to the current chain
	if (token->prev)
		token->prev->next = chain_start;
	chain_start->prev = token->prev;
	if (token->next)
		token->next->prev = chain_end;
	chain_end->next = token->next;

	// destroy token (unlink first so free_token doesn't touch the new chain)
	token->prev = NULL;
	token->next = NULL;
	free_token(token);

	return chain_end;
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

t_token *merge_tokens(t_token *first, t_token *second)
{
	char *new_str;

	if (!first || !second)
		return NULL;

	new_str = ft_strjoin(first->str, second->str);
	if (!new_str)
	{
		LOG_ERROR("merge_tokens(): failed malloc for merged token\n");
		return NULL;
	}
	free(first->str);
	first->str = new_str;

	first->next = second->next;
	if (second->next)
		second->next->prev = first;

	second->prev = NULL;
	second->next = NULL;
	free_token(second);

	return first;
}
