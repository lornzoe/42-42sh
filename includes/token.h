/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:06:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 15:31:06 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
#define TOKEN_H

typedef struct s_token
{
	char 	*str;
	struct  s_token *prev;
	struct	s_token *next;
}	t_token;

t_token *add_token(const char *str, t_token *end);

char	*get_token_str(t_token *token);
int		split_token(t_token *token, const char delim);
void	free_token(t_token *end);
void	free_token_chain(t_token *start);

void	print_token_chain(t_token *start);

#endif
