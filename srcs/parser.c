/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 21:28:31 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 04:36:19 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "log.h"
#include <stdlib.h>

static t_token *find_last_split(t_token *first, t_token *last,
    const char *a, const char *b)
{
    t_token *current;

    if (!first || !last)
        return NULL;
    current = last;
    while (current)
    {
        if (token_matchstr(current, a) || (b && token_matchstr(current, b)))
            return current;
        if (current == first)
            break;
        current = current->prev;
    }
    return NULL;
}

static t_astnode *syntax_error(t_token *token)
{
    LOG_ERROR("syntax error near unexpected token `%s'\n",
        token ? get_token_str(token) : "newline");
    return NULL;
}

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

    if (!parse_complete_commands(parent->nodes[1]))
        return NULL;

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
    if (!parent->first || !parent->last)
        return syntax_error(NULL);

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
        return parse_complete_command(parent->nodes[0]);
    }
    if (current == first || current == last)
        return syntax_error(current);

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
    if (!parse_complete_commands(parent->nodes[0]))
        return NULL;
    return parse_complete_command(parent->nodes[2]);
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
    if (!parent->first || !parent->last)
        return syntax_error(NULL);

    t_token *first = parent->first;
    t_token *last = parent->last;

    if (!token_matchstr(last, ";") && !token_matchstr(last, "&"))
    {
        LOG_TRACE("parse_complete_command(): assignment 1 [list]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse list
        return parse_list(parent->nodes[0]);
    }
    if (last == first)
        return syntax_error(last);

    LOG_TRACE("parse_complete_command(): assignment 0 [list separator_op]\n");
    parent->definition_type = 0;
    if (!alloc_ast_children(parent, 2))
        return NULL;
    parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, last->prev);
    parent->nodes[1] = create_ast_node(GRAMMAR_SEPARATOR_OP, last, last);
    if (!parent->nodes[0] || !parent->nodes[1])
        return NULL;
    // parse list and separator_op (do not parse separator_op, as it is a leaf node)
    return parse_list(parent->nodes[0]);
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
    if (!parent->first || !parent->last)
        return syntax_error(NULL);

    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *op = find_last_split(first, last, ";", "&");

    if (!op)
    {
        LOG_TRACE("parse_list(): assignment 1 [and_or]\n");
        parent->definition_type = 1;
        if (!alloc_ast_children(parent, 1))
            return NULL;
        parent->nodes[0] = create_ast_node(GRAMMAR_AND_OR, first, last);
        if (!parent->nodes[0])
            return NULL;
        // parse and_or
        return parse_and_or(parent->nodes[0]);
    }
    if (op == first || op == last)
        return syntax_error(op);

    LOG_TRACE("parse_list(): assignment 0 [list separator_op and_or]\n");
    parent->definition_type = 0;
    if (!alloc_ast_children(parent, 3))
        return NULL;
    parent->nodes[0] = create_ast_node(GRAMMAR_LIST, first, op->prev);
    parent->nodes[1] = create_ast_node(GRAMMAR_SEPARATOR_OP, op, op);
    parent->nodes[2] = create_ast_node(GRAMMAR_AND_OR, op->next, last);
    if (!parent->nodes[0] || !parent->nodes[1] || !parent->nodes[2])
        return NULL;
    // parse list and and_or
    if (!parse_list(parent->nodes[0]))
        return NULL;
    return parse_and_or(parent->nodes[2]);
}

/*
and_or
:                         pipeline
| and_or OR_IF  linebreak pipeline
| and_or AND_IF linebreak pipeline
*/
static t_astnode *build_and_or_split(t_astnode *parent, t_token *op,
    t_token *rhs)
{
    int is_and;

    is_and = token_matchstr(op, "&&");
    LOG_TRACE("parse_and_or(): assignment %d [and_or %s linebreak pipeline]\n",
        is_and ? 2 : 1, 
        is_and ? "AND_IF" : "OR_IF");
    parent->definition_type = is_and ? 2 : 1;
    if (!alloc_ast_children(parent, 4))
        return NULL;
    parent->nodes[0] = create_ast_node(GRAMMAR_AND_OR, parent->first, op->prev);
    parent->nodes[1] = create_ast_node(
        is_and ? GRAMMMAR_AND_IF : GRAMMAR_OR_IF, op, op);
    if (rhs == op->next)
        parent->nodes[2] = make_linebreak(NULL, NULL);
    else
        parent->nodes[2] = make_linebreak(op->next, rhs->prev);
    parent->nodes[3] = create_ast_node(GRAMMAR_PIPELINE, rhs, parent->last);
    if (!parent->nodes[0] || !parent->nodes[1]
        || !parent->nodes[2] || !parent->nodes[3])
        return NULL;
    // parse and_or (pipeline stays a leaf until parse_pipeline() exists)
    // parse_pipeline(parent->nodes[3]);
    return parse_and_or(parent->nodes[0]);
}

t_astnode *parse_and_or(t_astnode *parent)
{
    if (parent == NULL)
        return NULL;
    if (!parent->first || !parent->last)
        return syntax_error(NULL);

    t_token *first = parent->first;
    t_token *last = parent->last;
    t_token *op = find_last_split(first, last, "&&", "||");
    t_token *rhs;

    if (!op)
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
        return parent;
    }
    if (op == first || op == last)
        return syntax_error(op);

    rhs = op->next;
    while (rhs && rhs != last && is_blank_token(rhs))
        rhs = rhs->next;
    if (!rhs || is_blank_token(rhs))
        return syntax_error(op);
    return build_and_or_split(parent, op, rhs);
}
