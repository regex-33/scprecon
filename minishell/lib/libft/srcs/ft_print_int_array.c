/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_int_array.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/24 14:33:29 by yachtata          #+#    #+#             */
/*   Updated: 2023/12/25 20:19:26 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_int_array(int *array, int arr_size)
{
	int	i;

	i = 0;
	if (arr_size > 0)
		ft_putchar('[');
	while (i < arr_size)
	{
		if (i + 1 == arr_size)
		{
			ft_putnbr(array[i]);
			ft_putstr("]\n");
		}
		else
		{
			ft_putnbr(array[i]);
			ft_putstr(", ");
		}
		i++;
	}
}
