/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 21:28:31 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 03:57:49 by lyanga           ###   ########.fr       */
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

    parse_complete_commands(parent->nodes[1]);
 
    return parent;
}

/*
complete_commands
: complete_commands newline_list complete_command
|                                complete_command
;
*/
t_astnode *parse_complete_commands(t_astnode *parent)
{
    if (parent == NULL)
        return NULL;

    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *current = last;
    
    while (current && current != first)
    {
        if (is_newline_token(current))
            break;
        current = current->prev;
    }
    if (current == first && !is_newline_token(current))
    {
        LOG_TRACE("parse_complete_commands(): assignment 1 [complete_command]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_COMPLETE_COMMAND, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse complete_command
        parse_complete_command(parent->nodes[0]);
    }
    else
    {
        LOG_TRACE("parse_complete_commands(): assignment 0 [complete_commands newline_list complete_command]\n");
        parent->definition_type = 0;
        if (!alloc_ast_children(parent, 3))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_COMPLETE_COMMANDS, first, current->prev);
        parent->nodes[1] = make_linebreak(current, current);
        parent->nodes[2] = create_ast_node(GRAMMAR_COMPLETE_COMMAND, current->next, last);
        if (!parent->nodes[0] || !parent->nodes[1] || !parent->nodes[2])
            return NULL;
        // parse complete_commands and complete_command
        parse_complete_commands(parent->nodes[0]);
        parse_complete_command(parent->nodes[2]);
    }
    return parent;
}

/*
complete_command
: list separator_op
| list
;*/
t_astnode *parse_complete_command(t_astnode *parent)
{
    if (parent == NULL)
        return NULL;

    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *current = last;
    
    while (current && current != first)
    {
        if (token_matchstr(current, ";") || token_matchstr(current, "&"))
            break;
        current = current->prev;
    }
    if (current == first && !token_matchstr(current, ";") && !token_matchstr(current, "&"))
    {
        LOG_TRACE("parse_complete_command(): assignment 1 [list]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse list
        parse_list(parent->nodes[0]);
    }
    else
    {
        LOG_TRACE("parse_complete_command(): assignment 0 [list separator_op]\n");
        parent->definition_type = 0;
        if (!alloc_ast_children(parent, 2))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, current->prev);
        parent->nodes[1] = create_ast_node(GRAMMAR_SEPARATOR_OP, current, last);
        if (!parent->nodes[0] || !parent->nodes[1])
            return NULL;
        // parse list and separator_op (do not parse separator_op, as it is a leaf node)
        parse_list(parent->nodes[0]);
        
    }

    return parent;
}

/*
list
: list separator_op and_or
|                   and_or
;
*/
t_astnode *parse_list(t_astnode *parent)
{
    if (parent == NULL)
        return NULL;
    
    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *current = first;

    while (current && current != last)
    {
        if (token_matchstr(current, ";") || token_matchstr(current, "&"))
            break;
        current = current->next;
    }
    if (current == last && !token_matchstr(current, ";") && !token_matchstr(current, "&"))
    {
        LOG_TRACE("parse_list(): assignment 1 [and_or]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_AND_OR, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse and_or
        // parse_and_or(parent->nodes[0]);
    }
    else
    {
        LOG_TRACE("parse_list(): assignment 0 [list separator_op and_or]\n");
        parent->definition_type = 0;
        if (!alloc_ast_children(parent, 3))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, current->prev);
        parent->nodes[1] = create_ast_node(GRAMMAR_SEPARATOR_OP, current, current);
        parent->nodes[2] = create_ast_node(GRAMMAR_AND_OR, current->next, last);
        if (!parent->nodes[0] || !parent->nodes[1] || !parent->nodes[2])
            return NULL;
        // parse list and and_or 
        parse_list(parent->nodes[0]);
        // parse_and_or(parent->nodes[2]);
    }
    return parent;
}

/*
and_or
:                         pipeline
| and_or OR_IF  linebreak pipeline
| and_or AND_IF linebreak pipeline
*/
t_astnode *parse_and_or(t_astnode *parent)
{
    if (parent == NULL)
        return NULL;

    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *current = first;

    while (current && current != last)
    {
        if (token_matchstr(current, "&&") || token_matchstr(current, "||"))
            break;
        current = current->next;
    }
    if (current == last && !token_matchstr(current, "&&") && !token_matchstr(current, "||"))
    {
        LOG_TRACE("parse_and_or(): assignment 0 [pipeline]\n");
        parent->definition_type = 0;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_PIPELINE, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse pipeline
        // parse_pipeline(parent->nodes[0]);
    }
    else
    {
        LOG_TRACE("parse_and_or(): assignment 1/2 [and_or (OR_IF/AND_IF) linebreak pipeline]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 3))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_AND_OR, first, current->prev);
        parent->nodes[1] = create_ast_node(token_matchstr(current, "&&") ? GRAMMMAR_AND_IF : GRAMMAR_OR_IF, current, current);
        parent->nodes[2] = create_ast_node(GRAMMAR_PIPELINE, current->next, last);
        if (!parent->nodes[0] || !parent->nodes[1] || !parent->nodes[2])
            return NULL;
        // parse and_or and pipeline
        parse_and_or(parent->nodes[0]);
        // parse_pipeline(parent->nodes[2]);
    }
    return parent;
}
