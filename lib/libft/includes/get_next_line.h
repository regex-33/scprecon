/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/22 13:13:41 by yachtata          #+#    #+#             */
/*   Updated: 2023/11/22 13:13:43 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include "libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100

# endif

# if BUFFER_SIZE > 2147483645
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 0

# endif

# if BUFFER_SIZE < 0
#  undef BUFFER_SIZE
#  define BUFFER_SIZE 0

# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10

# endif

typedef struct s_get
{
	char			*content;
	struct s_get	*next;
}					t_get;

char				*get_next_line(int fd);
char				*ft_strdup(const char *src);
int					ft_lstadd_back(t_get **lst, char *content);
void				ft_lstclear(t_get **lst);
int					check_newline(t_get *list);
char				*ft_strchr(const char *s, int c);

#endif
