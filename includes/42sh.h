/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:51:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 18:20:20 by lyanga           ###   ########.fr       */
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

typedef enum e_symbols
{
	SYMBOL_NONE = 0,
	SYMBOL_END
}	t_grammar;

int	main(int argc, char **argv, char **envp);
void tokeniser(char *line);

#endif
