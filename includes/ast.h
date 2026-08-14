/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 19:17:50 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/14 23:38:30 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

#ifndef AST_H
#define AST_H

typedef enum e_grammar
{
    GRAMMAR_EMPTY,
    GRAMMAR_LINEBREAK,
    GRAMMAR_NEWLINE_LIST,
    GRAMMAR_NEWLINE,
    GRAMMAR_PROGRAM,
    GRAMMAR_COMPLETE_COMMANDS,
    GRAMMAR_COMPLETE_COMMAND,
    GRAMMAR_SEPARATOR_OP,
    GRAMMAR_LIST,
    GRAMMAR_AND_OR,
    GRAMMMAR_AND_IF,
    GRAMMAR_OR_IF,
    GRAMMAR_PIPELINE,
    GRAMMAR_BANG,
    GRAMMAR_PIPE_SEQUENCE,
    GRAMMAR_PIPE,
    GRAMMAR_COMMAND,
    GRAMMAR_SIMPLE_COMMAND,

    GRAMMAR_NOT_SIMPLE_COMMAND, //temp.
    GRAMMAR_END
}   t_grammar;

typedef struct s_astnode
{
    t_grammar type;
    int definition_type;
    struct s_astnode** nodes;
    int node_count;

    t_token *first;
    t_token *last;
}   t_astnode;

t_astnode* create_ast_node(t_grammar type, t_token *first, t_token *last);
int  alloc_ast_children(t_astnode *node, int count);
void free_ast_node(t_astnode* node);

// ast_print.c: two debug renderings of the same tree
void print_ast(t_astnode *node, int depth);
void print_ast_tree(t_astnode *node);


#endif