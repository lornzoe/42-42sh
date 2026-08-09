/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:51:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/09 18:02:30 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORTYTWOSH_H
#define FORTYTWOSH_H

// required headers for 42sh
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <errno.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <fcntl.h>
// #include <readline/readline.h>
// #include <readline/history.h>

#define SHELL_SPECIAL_CHARS "|&;<>()$`\\\"' \t\n*?[]^-!#~=%{,}@"

typedef enum e_tokentype
{
	TOKEN_UNKNOWN = 0,
	// redirect < > >> << >& <&
	TOKEN_REDIRECT_IN,			// <
	TOKEN_REDIRECT_OUT_TRUNC,	// >
	TOKEN_REDIRECT_OUT_APPEND,	// >>
	TOKEN_REDIRECT_HEREDOC,		// <<
	TOKEN_REDIRECT_DUP_IN, 		// <&
	TOKEN_REDIRECT_DUP_OUT, 	// >&
	// pipe|
	TOKEN_PIPE,					// |
	// seperator ;
	TOKEN_SEPERATOR,			// ;
	// logical operators || &&
	TOKEN_LOGICAL_AND,			// &&
	TOKEN_LOGICAL_OR,			// ||
	// simple parameter expansion ${}
	TOKEN_PARAM_EXPAND_OPEN,	// ${
	TOKEN_PARAM_EXPAND_CLOSE,	// }

	TOKEN_WORD,					// any other word
}	t_tokentype;

int	main(int argc, char **argv, char **envp);
void tokeniser(char *line);

#endif
