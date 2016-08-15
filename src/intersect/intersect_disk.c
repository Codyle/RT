/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_disk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 15:39:30 by rojones           #+#    #+#             */
/*   Updated: 2016/08/15 07:33:53 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int		intersect_disk(t_ray *r, t_prim *o, double *t)
{
	double		denominator;
	double		numerator;
	double		t0;
	t_vector	point;

	if ((denominator = vdot(r->dir, o->normal)) == 0)
		return (0);
	numerator = vdot(o->loc, o->normal) - vdot(r->loc, o->normal);
	t0 = numerator / denominator;
	if (t0 > EPSILON)
	{
		point = vadd(r->loc, vmult(r->dir, t0));
		t_vector	dist;
		dist = vsub(o->loc, point);
		double check;
		check = vnormalize(dist);
		if (check < o->radius)
		{
				*t = t0;
				return (1);
		}
		return (0);
	}
	return (0);
}
