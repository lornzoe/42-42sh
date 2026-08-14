/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 02:02:25 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 02:17:11 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// debug-only rendering of the ast; two styles are available:
//   print_ast()      - flat depth column + "└───" rails
//   print_ast_tree() - box-drawing branches (├── │ └──)

#include "ast.h"
#include "log.h"
#include <stdio.h>
#include <string.h>

#define TREE_PREFIX_MAX 512
#define TREE_STEM	"│   "
#define TREE_GAP	"    "
#define TREE_TEE	"├── "
#define TREE_ELBOW	"└── "

static const char *g_grammar_names[] = {
    "empty",
    "linebreak",
    "newline_list",
    "newline",
    "program",
    "complete_commands",
    "complete_command",
    "separator_op",
    "list",
    "and_or",
    "and_if",
    "or_if",
    "pipeline",
    "bang",
    "pipe_sequence",
    "pipe",
    "command",
    "simple_command",
    "not_simple_command",
    "end"
};

static const char *grammar_name(t_grammar type)
{
    if (type < 0 || type > GRAMMAR_END)
        return "?";
    return g_grammar_names[type];
}

// tokens no longer carry a type, so operators are recognised from their text;
// newlines live in that table too, which keeps them out of the blank filler
static int is_operator_token(t_token *token)
{
    if (!token)
        return 0;
    return operator_type(token->str) != TOKEN_UNKNOWN;
}

// [-] blank filler, [X] operator, [O] word: same at-a-glance markers as the
// per-word dump in minishell's print_ast_words()
static const char *token_mark(t_token *token)
{
    if (!token)
        return "[ ]";
    if (is_operator_token(token))
        return "[X]";
    if (is_blank_token(token))
        return "[-]";
    return "[O]";
}

static const char *token_color(t_token *token)
{
    if (!token)
        return LOG_COLOR_GREY;
    if (is_operator_token(token))
        return LOG_COLOR_YELLOW;
    if (is_blank_token(token))
        return LOG_COLOR_GREY;
    return LOG_COLOR_GREEN;
}

static int is_leaf(t_astnode *node)
{
    int i;

    if (!node || node->node_count == 0)
        return 1;
    i = 0;
    while (i < node->node_count)
        if (node->nodes[i++])
            return 0;
    return 1;
}

// the whole token span on one line: [ls][ ][-la]
static void print_token_span(t_astnode *node)
{
    t_token *current;

    if (!node->first)
    {
        printf(LOG_COLOR_GREY "(empty)" LOG_COLOR_RESET);
        return;
    }
    current = node->first;
    while (current)
    {
        printf("%s[" LOG_COLOR_RESET, token_color(current));
        print_escaped_str(current->str);
        printf("%s]" LOG_COLOR_RESET, token_color(current));
        if (current == node->last)
            break;
        current = current->next;
    }
}

static void print_node_label(t_astnode *node)
{
    if (!node)
    {
        printf(LOG_COLOR_RED "<null>" LOG_COLOR_RESET "\n");
        return;
    }
    printf(LOG_COLOR_CYAN "<%s>" LOG_COLOR_RESET, grammar_name(node->type));
    if (node->definition_type >= 0)
        printf(LOG_COLOR_GREY " #%d" LOG_COLOR_RESET, node->definition_type);
    printf(" ");
    print_token_span(node);
    printf("\n");
}

static void print_indent(int levels)
{
    while (levels-- > 0)
        printf("    ");
}

/* ========================================================================== */
/* style 1: depth column + rails                                              */
/* ========================================================================== */

// leaves get their span exploded one token per line
static void print_ast_words(t_astnode *node, int depth)
{
    t_token *current;

    current = node->first;
    while (current)
    {
        printf("      ");
        print_indent(depth + 1);
        printf("%s%s " LOG_COLOR_RESET, token_color(current),
            token_mark(current));
        print_escaped_str(current->str);
        printf("\n");
        if (current == node->last)
            break;
        current = current->next;
    }
}

void print_ast(t_astnode *node, int depth)
{
    int i;

    if (depth == 0)
        printf(LOG_COLOR_CYAN "<PRINTING AST TREE>" LOG_COLOR_RESET "\n");
    printf("%3d   ", depth);
    i = 1;
    while (i++ < depth)
        printf("    ");
    if (depth > 0)
        printf(LOG_COLOR_GREY "└───" LOG_COLOR_RESET);
    print_node_label(node);
    if (!node)
        return;
    if (is_leaf(node))
        print_ast_words(node, depth);
    i = 0;
    while (i < node->node_count)
        print_ast(node->nodes[i++], depth + 1);
}

/* ========================================================================== */
/* style 2: box-drawing branches                                              */
/* ========================================================================== */

// a leaf's tokens hang below it as dotted entries instead of branches, so the
// grammar shape stays readable at a glance
static void print_tree_words(t_astnode *node, const char *prefix)
{
    t_token *current;

    current = node->first;
    while (current)
    {
        printf(LOG_COLOR_GREY "%s  ╴ " LOG_COLOR_RESET, prefix);
        printf("%s%s " LOG_COLOR_RESET, token_color(current),
            token_mark(current));
        print_escaped_str(current->str);
        printf("\n");
        if (current == node->last)
            break;
        current = current->next;
    }
}

static void print_tree_children(t_astnode *node, char *prefix, int len);

// prefix holds the stems inherited from every ancestor; len is its length in
// bytes (the glyphs are multi-byte, so it is not the column count)
static void print_tree_node(t_astnode *node, char *prefix, int len, int is_last)
{
    const char  *stem;
    int         child_len;

    printf(LOG_COLOR_GREY "%s%s" LOG_COLOR_RESET, prefix,
        is_last ? TREE_ELBOW : TREE_TEE);
    print_node_label(node);
    if (!node)
        return;
    stem = is_last ? TREE_GAP : TREE_STEM;
    child_len = len;
    if (child_len + (int)strlen(stem) < TREE_PREFIX_MAX)
    {
        strcpy(prefix + child_len, stem);
        child_len += (int)strlen(stem);
    }
    print_tree_children(node, prefix, child_len);
    prefix[len] = '\0';
}

static void print_tree_children(t_astnode *node, char *prefix, int len)
{
    int i;

    if (is_leaf(node))
    {
        print_tree_words(node, prefix);
        return;
    }
    i = 0;
    while (i < node->node_count)
    {
        print_tree_node(node->nodes[i], prefix, len,
            i == node->node_count - 1);
        i++;
    }
}

void print_ast_tree(t_astnode *node)
{
    char    prefix[TREE_PREFIX_MAX];

    printf(LOG_COLOR_CYAN "<PRINTING AST TREE>" LOG_COLOR_RESET "\n");
    prefix[0] = '\0';
    print_node_label(node);
    if (!node)
        return;
    print_tree_children(node, prefix, 0);
}
