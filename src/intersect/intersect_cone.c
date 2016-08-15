/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cone.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adippena <angusdippenaar@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/16 09:02:46 by adippena          #+#    #+#             */
/*   Updated: 2016/08/15 11:00:51 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersect_cone.h"

static int	check_lim(t_ray *r, t_prim *o, double *t, double *t_test)
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
}

static int	find_t(t_quad *quad, double *t, t_prim *o, t_ray *r)
{
	double	sqrt_discr;
	double	t0;
	double	t1;
	double	tc;

	sqrt_discr = sqrt(quad->discr);
	t0 = (-quad->b + sqrt_discr) / (2.0 * quad->a);
	t1 = (-quad->b - sqrt_discr) / (2.0 * quad->a);
	tc = (t0 > t1) ? t1 : t0;
	if (o->limit == -1)
	{
		if (tc > EPSILON)
		{
			*t = tc;
			return (1);
		}
	}
	else 
	{
		if (tc > EPSILON)
		{
			if (check_lim(r, o, t, &tc))
				return (1);
		}
		else
		{
			tc = (tc == t1) ? t0 : t1;
			if (tc > EPSILON)
				return (check_lim(r, o, t, &tc));
			else
				return (0);
		}
	}
	return (0);
}

int			intersect_cone(t_ray *r, t_prim *o, double *t)
{
	t_intersect_cone	c;

	c.dist = vsub(r->loc, o->loc);
	c.v_va = vdot(r->dir, o->dir);
	c.dp_va = vdot(c.dist, o->dir);
	c.a_v = vsub(r->dir, vmult(o->dir, c.v_va));
	c.c_v = vsub(c.dist, vmult(o->dir, c.dp_va));
	c.cos2 = cos(o->angle) * cos(o->angle);
	c.sin2 = sin(o->angle) * sin(o->angle);
	c.quad.a = c.cos2 * vdot(c.a_v, c.a_v) -
		c.sin2 * c.v_va * c.v_va;
	c.quad.b = 2.0 * c.cos2 * vdot(c.a_v, c.c_v) -
		2.0 * c.sin2 * c.v_va * c.dp_va;
	c.quad.c = c.cos2 * vdot(c.c_v, c.c_v) - c.sin2 * c.dp_va * c.dp_va;
	c.quad.discr = c.quad.b * c.quad.b - 4.0 * c.quad.a * c.quad.c;
	if (c.quad.discr < EPSILON)
		return (0);
	return (find_t(&c.quad, t, o, r));
}
