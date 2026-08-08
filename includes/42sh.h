/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:51:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/09 01:58:09 by lyanga           ###   ########.fr       */
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

#define QUOTING "|&;<>()$`\\\"' \t\n*?[]^-!#~=%{,}"

typedef enum e_symbols
{
	SYMBOL_NONE = 0,
	// redirect < > >> << >& <&
	SYMBOL_REDIRECT_IN,			// <
	SYMBOL_REDIRECT_OUT_TRUNC,	// >
	SYMBOL_REDIRECT_OUT_APPEND,	// >>
	SYMBOL_REDIRECT_HEREDOC,	// <<
	SYMBOL_REDIRECT_DUP_IN, 	// <&
	SYMBOL_REDIRECT_DUP_OUT, 	// >&

	SYMBOL_UNKNOWN,
}	t_symbols;

int	main(int argc, char **argv, char **envp);
void tokeniser(char *line);

#endif
