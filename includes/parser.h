/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 22:38:53 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 03:38:55 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

t_astnode* parse_input(t_token *chain);

t_astnode* parse_program(t_astnode *parent);
t_astnode* parse_complete_commands(t_astnode *parent);
t_astnode* parse_complete_command(t_astnode *parent);
t_astnode* parse_list(t_astnode *parent);
t_astnode* parse_and_or(t_astnode *parent);
#endif