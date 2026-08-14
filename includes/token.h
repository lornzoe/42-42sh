/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 14:06:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 03:11:47 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
#define TOKEN_H

#define SHELL_SPECIAL_CHARS "|&;<>()$`\\\"' \t\n*?[]^-!#~=%{,}@"

typedef enum e_tokentype
{
	TOKEN_UNKNOWN = 0,
	TOKEN_WORD,

	TOKEN_OPERATOR, 				// all values after this are only
	// redirect < > >> >| <& >& << <<- <>
	TOKEN_REDIRECT_IN,				// <
	TOKEN_REDIRECT_OUT_TRUNC,		// >
	TOKEN_REDIRECT_OUT_APPEND,		// >>
	TOKEN_REDIRECT_OUT_CLOBBER,		// >|
	TOKEN_REDIRECT_DUP_IN, 			// <&
	TOKEN_REDIRECT_DUP_OUT, 		// >&
	TOKEN_REDIRECT_HEREDOC,			// <<
	TOKEN_REDIRECT_HEREDOC_STRIP,	// <<-
	TOKEN_REDIRECT_RDWR,			// <>
	// pipe|
	TOKEN_PIPE,						// |
	// seperator ;
	TOKEN_SEPARATOR,				// ;
	TOKEN_CASE_END,					// ;;
	// logical operators || &&
	TOKEN_LOGICAL_AND,				// &&
	TOKEN_LOGICAL_OR,				// ||
	// async/background &
	TOKEN_BACKGROUND,				// &
	// subshell grouping ( )
	TOKEN_SUBSHELL_OPEN,			// (
	TOKEN_SUBSHELL_CLOSE,			// )
	// structural
	TOKEN_NEWLINE,					// \n

}	t_tokentype;

typedef struct s_token
{
	char 	*str;

	struct  s_token *prev;
	struct	s_token *next;
}	t_token;

t_token *add_token(const char *str, t_token *end);

char	*get_token_str(t_token *token);
int		token_strcmp(t_token *token, const char *str);
int		token_matchstr(t_token *token, const char *str);
t_token	*split_token(t_token *token, const char delim);
t_token	*merge_tokens(t_token *first, t_token *second);
void	free_token(t_token *end);
void	free_token_chain(t_token *start);

t_tokentype	operator_type(const char *str);

void	print_escaped_str(const char *str);
void	print_token_chain(t_token *start);

t_token *get_first_token(t_token *chain);
t_token *get_last_token(t_token *chain);

int		is_space_token(t_token *token);
int		is_blank_token(t_token *token);
int		is_newline_token(t_token *token);

#endif
