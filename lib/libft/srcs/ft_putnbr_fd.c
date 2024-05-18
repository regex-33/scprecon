/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 18:38:09 by yachtata          #+#    #+#             */
/*   Updated: 2023/12/24 14:24:40 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putcharr(char c, int fd)
{
	write(fd, &c, 1);
}

static void	ft_putnbrr(int nb, int fd)
{
	if (nb == -2147483648)
	{
		write(fd, "-", 1);
		write(fd, "2", 1);
		write(fd, "147483648", 9);
	}
	else if (nb < 0)
	{
		ft_putcharr('-', fd);
		nb = -nb;
		ft_putnbrr(nb, fd);
	}
	else if (nb > 9)
	{
		ft_putnbrr(nb / 10, fd);
		ft_putnbrr(nb % 10, fd);
	}
	else
		ft_putcharr(nb + '0', fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (fd < 0)
		return ;
	ft_putnbrr(n, fd);
}
