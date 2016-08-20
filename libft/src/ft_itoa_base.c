/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/15 16:52:59 by rojones           #+#    #+#             */
/*   Updated: 2016/08/20 16:27:52 by adippena         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_numlen(int num, int base)
{
	size_t	re;

	re = 0;
	if (num == 0)
		return(1);
	if (num < 0 && base == 10)
		re++;
	while (num)
	{
		re++;
		num /= base;
	}
	return (re);
}

char	*ft_itoa_base(int value, int base)
{
	char	*re;
	size_t	size;
	int		sign;

	size = ft_numlen(value, base);
	sign = (value < 0) ? -1 : 1;
	if (!(re = (char *)malloc(size + 1)))
		return (NULL);
	re[size--] = '\0';
	if (!value)
		re[0] = '0';
	while (value)
	{
		re[size--] = "0123456789ABCDEF"[sign * (value % base)];
		value /= base;
	}
	if (sign == -1 && base == 10)
		re[0] = '-';
	return (re);
}
