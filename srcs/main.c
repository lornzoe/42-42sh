/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:50:58 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 16:39:13 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;
	while ((line = readline("42sh$ ")) != NULL)
	{
		if (*line)
		{
			add_history(line);
			tokeniser(line);
		}
		free(line);
	}
	return (0);
}
