/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/07 20:39:41 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/08 00:56:01 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"
#include <stdio.h>
#include <string.h>

void tokeniser(char *line)
{
	printf("Tokenising line: [%s]\n", line);

	char *token;
	char *delimiter = " \n";
	token = strtok(line, delimiter);
	while (token)
    {
        printf("[%s]", token);
        token = strtok(NULL, delimiter);
    }
	printf("\n");

}