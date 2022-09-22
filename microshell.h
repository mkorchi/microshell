/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/15 11:38:28 by mkorchi           #+#    #+#             */
/*   Updated: 2022/09/21 12:54:17 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H


# define MICROSHELL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

# define PIPE 4
# define SEMICOLON 5
# define START 0

typedef struct s_cmd {
	char	**args;
	int		sep_type;
}	t_cmd;


/*
command is a struct
array of commands seperated by either ";" or "|"
we shall call ";" and "|" seperators
number of array is number of seperators + 1

*	the first command will always read from 0
*	the last command will always write to 1

* each command have a seperator type that proceeds it, the first command is empty
* if the next command's sepearator is
							* pipe => we gonna write in fd[1]
								-> close fd[1] when done
							* semicolon => we gonna write in 1 no pipe needed

* if this command's seperator is
							* pipe => we gonna read from fd[0], the wait is not needed
								-> close fd[0] when done
							* semicolon => we gonna read from 0 and we shall wait for the previous command

*/


#endif