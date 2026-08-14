/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 22:03:50 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 02:13:34 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_astnode *create_ast_node(t_grammar type, t_token *first, t_token *last)
{
    t_astnode *node;

    if (first && last)
    {
        while (is_space_token(first) && first != last)
            first = first->next;
        while (is_space_token(last) && last != first)
            last = last->prev;
    }
    else
    {
        first = NULL;
        last = NULL;
    }

    node = (t_astnode *)malloc(sizeof(t_astnode));
    if (!node)
    {
        LOG_ERROR("create_ast_node(): failed malloc for ast node\n");
        return NULL;
    }

    node->type = type;
    node->definition_type = -1;
    node->nodes = NULL;
    node->node_count = 0;
    node->first = first;
    node->last = last;
    return node;
}

// every grammar definition has a known arity, so children are allocated once
int alloc_ast_children(t_astnode *node, int count)
{
    int i;

    if (!node || count <= 0)
        return 0;
    node->nodes = (t_astnode **)malloc(sizeof(t_astnode *) * count);
    if (!node->nodes)
    {
        LOG_ERROR("alloc_ast_children(): failed malloc for %d children\n", count);
        return 0;
    }
    i = 0;
    while (i < count)
        node->nodes[i++] = NULL;
    node->node_count = count;
    return 1;
}

void free_ast_node(t_astnode *node)
{
    int i;

    if (!node)
        return;
    i = 0;
    while (i < node->node_count)
        free_ast_node(node->nodes[i++]);
    free(node->nodes);
    free(node);
}
