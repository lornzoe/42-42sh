/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 21:28:31 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 02:25:32 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "log.h"
#include <stdlib.h>

t_astnode *parse_input(t_token *chain)
{
    t_astnode *root;

    if (!chain)
        return NULL;
    root = create_ast_node(GRAMMAR_PROGRAM,
        get_first_token(chain), get_last_token(chain));
    if (!root)
        return NULL;
    if (!parse_program(root))
    {
        free_ast_node(root);
        return NULL;
    }
    return root;
}

/*
linebreak
: newline_list
| // empty
;
*/
static t_astnode *make_linebreak(t_token *first, t_token *last)
{
    t_astnode   *node;
    t_token     *current;

    // only newlines make a newline_list; a run of blanks is an empty linebreak
    current = first;
    while (current && is_space_token(current))
    {
        if (current == last)
        {
            first = NULL;
            last = NULL;
            break;
        }
        current = current->next;
    }
    node = create_ast_node(GRAMMAR_LINEBREAK, first, last);
    if (!node)
        return NULL;
    if (node->first)
        node->definition_type = 0;
    else
        node->definition_type = 1;
    return node;
}

/*
program
: linebreak complete_commands linebreak
| linebreak
;
*/
t_astnode *parse_program(t_astnode *parent)
{

    if (parent == NULL)
        return NULL;

    t_token *first = parent->first;
    t_token *last = parent->last;

    while (is_blank_token(first) && first != last)
        first = first->next;
    if (is_blank_token(first))
    {
        LOG_TRACE("parse_program(): assignment 1 [linebreak]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = make_linebreak(parent->first, parent->last);
        if (!parent->nodes[0])
            return NULL;
        return parent;
    }

    LOG_TRACE("parse_program(): assignment 0 [linebreak complete_commands linebreak]\n");
    while (is_blank_token(last))
        last = last->prev;
    parent->definition_type = 0;
    if (!alloc_ast_children(parent, 3))
        return NULL;
    if (first == parent->first)
        parent->nodes[0] = make_linebreak(NULL, NULL);
    else
        parent->nodes[0] = make_linebreak(parent->first, first->prev);
    parent->nodes[1] = create_ast_node(GRAMMAR_COMPLETE_COMMANDS, first, last);
    if (last == parent->last)
        parent->nodes[2] = make_linebreak(NULL, NULL);
    else
        parent->nodes[2] = make_linebreak(last->next, parent->last);
    if (!parent->nodes[0] || !parent->nodes[1] || !parent->nodes[2])
        return NULL;

    // parse complete_commands before return
 
    return parent;
}


