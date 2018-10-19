/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/22 04:43:16 by azulbukh          #+#    #+#             */
/*   Updated: 2018/03/22 04:53:22 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isalpha(int ch)
{
	if ((ch >= 97 && ch <= 122) || (ch >= 65 && ch <= 90))
		return (1);
	return (0);
}
