/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 15:48:36 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

void tokeniser(char *line)
{
	t_token *chain = NULL;
	char *str;
	char *delimiter = " ";
	str = strtok(line, delimiter);
	printf("Words: <%s>", str);
	chain = add_token(str, NULL);
	while (str)
    {
        str = strtok(NULL, delimiter);
		printf("<%s>", str);
		if (str != NULL)
			chain = add_token(str, chain);
    }
	printf("\n");
	print_token_chain(chain);
}

