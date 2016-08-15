/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adippena <angusdippenaar@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/16 09:02:46 by adippena          #+#    #+#             */
/*   Updated: 2016/08/15 07:58:01 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersect_cone.h"

static int	find_t(double a, double b, double discr, double *t)
{
	double	sqrt_discr;
	double	t0;
	double	t1;

	sqrt_discr = sqrt(discr);
	t0 = (-b + sqrt_discr) / (2.0 * a);
	t1 = (-b - sqrt_discr) / (2.0 * a);
	if (t0 > t1)
		t0 = t1;
	if (t0 > EPSILON)
	{
		*t = t0;
		return (1);
	}
	return (0);
}

/*static int	check_lim(t_ray *r, t_prim *o, double *t_test, double *t)
{
	t_vector	point;

	if (o->limit == -1)
	{
		*t = *t_test;
		return (1);
	}
	point = vadd(r->loc, vmult(r->dir, *t_test));
	point = vproject(point, o->dir);
	if (vnormalize(vsub(point, o->loc)) <= o->limit)
	{
		*t = *t_test;
		return (1);
	}
	return (0);
}*/

int			intersect_cone(t_ray *r, t_prim *o, double *t)
{
//	double				t_test;
	t_intersect_cone	c;

	c.dist = vsub(r->loc, o->loc);
	c.v_va = vdot(r->dir, o->dir);
	c.dp_va = vdot(c.dist, o->dir);
	c.a_v = vsub(r->dir, vmult(o->dir, c.v_va));
	c.c_v = vsub(c.dist, vmult(o->dir, c.dp_va));
	c.cos2 = cos(o->angle) * cos(o->angle);
	c.sin2 = sin(o->angle) * sin(o->angle);
	c.a = c.cos2 * vdot(c.a_v, c.a_v) -
		c.sin2 * c.v_va * c.v_va;
	c.b = 2.0 * c.cos2 * vdot(c.a_v, c.c_v) -
		2.0 * c.sin2 * c.v_va * c.dp_va;
	c.c = c.cos2 * vdot(c.c_v, c.c_v) - c.sin2 * c.dp_va * c.dp_va;
	c.d = c.b * c.b - 4.0 * c.a * c.c;
	if (c.d < EPSILON)
		return (0);
//	if (find_t(c.a, c.b, c.d, &t_test))
//		return (check_lim(r, o, &t_test, t));
	return (find_t(c.a, c.b, c.d, t));
}
