/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersect_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adippena <angusdippenaar@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/14 22:44:25 by adippena          #+#    #+#             */
/*   Updated: 2016/08/15 07:15:21 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "intersect_cylinder.h"

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

static int	check_lim(t_ray *r, t_prim *o, double *t_test, double *t)
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

int			intersect_cylinder(t_ray *r, t_prim *o, double *t)
{
	double			t_test;
	t_int_cylinder	c;

	c.dist = vsub(r->loc, o->loc);
	c.a_v = vsub(r->dir, vmult(o->dir, vdot(r->dir, o->dir)));
	c.c_v = vsub(c.dist, (vmult(o->dir, vdot(c.dist, o->dir))));
	c.a = vdot(c.a_v, c.a_v);
	c.b = 2.0 * vdot(c.a_v, c.c_v);
	c.c = vdot(c.c_v, c.c_v) - (o->radius * o->radius);
	c.d = c.b * c.b - 4.0 * c.a * c.c;
	if (c.d < EPSILON)
		return (0);
	if (find_t(c.a, c.b, c.d, &t_test))
		return (check_lim(r, o, &t_test, t));
	return (0);
}
