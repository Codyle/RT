/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_disk.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rojones <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/08/12 15:39:30 by rojones           #+#    #+#             */
/*   Updated: 2016/08/12 16:19:06 by rojones          ###   ########.fr       */
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
	{
		puts("returning fauls 1");
		return (0);
	}
	numerator = vdot(o->loc, o->normal) - vdot(r->loc, o->normal);
	t0 = numerator / denominator;
	if (t0 > EPSILON)
	{
		point = vadd(r->loc, vmult(r->dir, t0));
		t_vector	dist;
		dist = vsub(o->loc, point);
		double check;
		check = vnormalize(dist);
		printf("check %f\n",check);
		printf("o->radius %f\n",o->radius);
		if (check < o->radius)
		{
			puts("assining");
				*t = t0;
				puts("returning");
				return (1);
		}
		puts("returning fauls2");
		return (0);
	}
	puts("returning fauls3");
	return (0);
}
