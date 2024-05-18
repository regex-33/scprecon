/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_to_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <yachtata@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 04:05:47 by yachtata          #+#    #+#             */
/*   Updated: 2024/01/30 04:06:14 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	int_to_str(int num, char *str)
{
	int		i;
	int		start;
	int		end;
	char	temp;

	i = 0;
	while (num != 0)
	{
		str[i++] = '0' + num % 10;
		num /= 10;
	}
	str[i] = '\0';
	start = 0;
	end = i - 1;
	while (start < end)
	{
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}
