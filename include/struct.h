/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvan-pae <bryan.vanpaemel@gmail.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 10:40:50 by bvan-pae          #+#    #+#             */
/*   Updated: 2024/05/29 15:39:45 by bvan-pae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

#define DARK_RED "\x1b[48;5;1m\x1b[30m"
#define LIGHT_RED "\x1b[48;5;216m\x1b[30m"
#define LIGHT_BLUE "\x1b[48;5;39m\x1b[30m"
#define WHITE "\x1b[48;5;15m\x1b[30m"
#define DARK_BLUE "\x1b[48;5;18m\x1b[30m"
#define RESET_COLOR "\033[0m"

enum {
	DWORD = 1,
	DLETTER = 2,
	TWORD = 3,
	TLETTER = 4,
};

typedef struct word {
	char		*word;
	int			score;
	struct word	*next;
} word;

typedef struct grid {
	int			grid[15][15];
	int			modifier[15][15];
} grid;


#endif
