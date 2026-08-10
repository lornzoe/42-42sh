/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:50:58 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/10 08:09:24 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// POSIX reference:
// https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;

	/* ============================================================
	 * STEP 1: INPUT SOURCE
	 * ------------------------------------------------------------
	 * The shell reads its input from a file (via the sh utility),
	 * from the -c option, or from the system() and popen()
	 * functions. Here, readline() reads interactively from stdin,
	 * one line at a time, in place of the sh utility's file input.
	 *
	 * NOTE: If a script file's first line starts with "#!", and
	 * that file is passed explicitly to sh, results are unspecified.
	 * ============================================================ */
	
	while ((line = readline("42sh$ ")) != NULL)
	{
		if (*line)
		{
			add_history(line);
			// following posix's 2.1 general overview of operations:
			/* ============================================================
			* STEP 2: TOKENIZATION
			* ------------------------------------------------------------
			* The shell breaks raw input into tokens: words and operators.
			*
			* Example input:
			*     grep "error" log.txt > results.txt
			*
			* Tokens produced:
			*     WORD:grep  WORD:"error"  WORD:log.txt  OP:>  WORD:results.txt
			* ============================================================ */
			t_token *chain = tokeniser(line);

			/* ============================================================
			 * STEP 3: PARSING (SIMPLE vs COMPOUND COMMANDS)
			 * ------------------------------------------------------------
			 * Tokens are parsed into a command structure:
			 *   - Simple command:   grep "error" log.txt > results.txt
			 *   - Compound command: if [ -f x ]; then echo yes; fi
			 *                        for i in 1 2 3; do echo $i; done
			 *                        cmd1 | cmd2   (pipeline)
			 * ============================================================ */
			// build ast tree

			/* ============================================================
			 * STEP 4: WORD EXPANSIONS
			 * ------------------------------------------------------------
			 * Each word undergoes (in order): backslash processing inside
			 * $'...', tilde expansion, parameter expansion ($var), command
			 * substitution ($(cmd)), arithmetic expansion ($((expr))),
			 * field splitting, pathname expansion (globbing), and quote
			 * removal.
			 *
			 * Example:
			 *     "error"  ->  error        (quotes removed, no globbing)
			 *     *.txt    ->  log.txt a.txt b.txt   (glob expanded)
			 * ============================================================ */

			/* ============================================================
			 * STEP 5: REDIRECTION
			 * ------------------------------------------------------------
			 * Redirection operators/operands are processed and then
			 * stripped from the argument list before the command runs.
			 *
			 * Example:
			 *     grep error log.txt > results.txt
			 *         -> stdout redirected to results.txt
			 *         -> argv passed to grep: { "grep", "error", "log.txt" }
			 *            (">" and "results.txt" never reach grep itself)
			 * ============================================================ */

			/* ============================================================
			 * STEP 6: EXECUTION
			 * ------------------------------------------------------------
			 * The shell executes a function, special built-in, executable
			 * file, or script. Arguments become positional parameters
			 * ($1..$n); the command name becomes $0.
			 *
			 * Example:
			 *     argv[0] = "grep"      // $0
			 *     argv[1] = "error"     // $1
			 *     argv[2] = "log.txt"   // $2
			 * ============================================================ */
			// execute ast tree

			/* ============================================================
			 * STEP 7: EXIT STATUS COLLECTION
			 * ------------------------------------------------------------
			 * The shell optionally waits for the command to finish and
			 * collects its exit status into $?.
			 *
			 * Example:
			 *     grep error log.txt > results.txt
			 *     echo $?        // 0 = match found, 1 = no match, 2 = error
			 *
			 * If run in background ("cmd &"), the shell does NOT wait here;
			 * status can be collected later via wait.
			 * ============================================================ */
			// end loop (set return number, idk what else?)
			free_token_chain(chain);
		}
		free(line);
	}
	return (0);
}