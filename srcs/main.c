/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:50:58 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/15 05:23:52 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include "ast.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// POSIX reference:
// https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html

static void	run_line(char *line)
{
	t_token		*chain;
	t_astnode	*ast_root;

	// 2. The shell breaks the input into tokens: words and operators; see 2.3 Token Recognition.
	chain = tokeniser(line);
	// 3. The shell parses the input into simple commands (see 2.9.1 Simple Commands) and compound commands (see 2.9.4 Compound Commands).
	ast_root = parse_input(chain);

	// for printing out AST tree.
	LOG_DEBUG_CALL(print_ast(ast_root, 0));
	// LOG_DEBUG_CALL(print_ast_tree(ast_root));

	// 4. For each word within a command, the shell processes <backslash>-escape sequences inside dollar-single-quotes (see 2.2.4 Dollar-Single-Quotes) and then performs various word expansions (see 2.6 Word Expansions). In the case of a simple command, the results usually include a list of pathnames and fields to be treated as a command name and arguments; see 2.9 Shell Commands.
	// 5. The shell performs redirection (see 2.7 Redirection) and removes redirection operators and their operands from the parameter list.
	// 6. The shell executes a function (see 2.9.5 Function Definition Command), built-in (see 2.15 Special Built-In Utilities), executable file, or script, giving the names of the arguments as positional parameters numbered 1 to n, and the name of the command (or in the case of a function within a script, the name of the script) as special parameter 0 (see 2.9.1.4 Command Search and Execution).
	// 7. The shell optionally waits for the command to complete and collects the exit status (see 2.8.2 Exit Status for Commands).

	free_ast_node(ast_root);
	free_token_chain(chain);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)envp;

	 // -c option: run a single command and exit for testing
	if (argc >= 3 && strcmp(argv[1], "-c") == 0)
	{
		run_line(argv[2]);
		return (0);
	}
	
	while ((line = readline("42sh$ ")) != NULL)
	{
		if (*line)
		{
			add_history(line);
			run_line(line);
		}
		free(line);
	}
	return (0);
}