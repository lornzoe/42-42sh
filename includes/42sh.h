/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   42sh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/05 05:51:07 by lyanga            #+#    #+#             */
/*   Updated: 2026/08/10 06:28:04 by lyanga           ###   ########.fr       */
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

#include "log.h"
#include "token.h"


int	main(int argc, char **argv, char **envp);
t_token *tokeniser(char *line);

#endif
