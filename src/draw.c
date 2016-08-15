/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adippena <angusdippenaar@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/10 14:00:07 by adippena          #+#    #+#             */
/*   Updated: 2016/08/15 07:51:51 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw.h"

static uint32_t	find_colour(t_env *e)
{
	uint32_t	colour;
	t_colour	temp_c;

	if (!e->hit_type)
		return (0x7F7F7F);
	temp_c = (e->hit_type == FACE) ? face_diffuse(e) : prim_diffuse(e);
	colour = 0;
	colour |= (int)(temp_c.r * 255.0) << 16;
	colour |= (int)(temp_c.g * 255.0) << 8;
	colour |= (int)(temp_c.b * 255.0);
	return (colour);
}

static void		*draw_chunk(void *q)
{
	t_chunk		*c;

	c = (t_chunk *)q;
	c->stopx = c->d.x + c->d.w;
	c->stopy = c->d.y + c->d.h;
	setup_camera_plane(c->e, &c->cr);
	while (c->d.y < c->stopy && c->d.y < WIN_Y)
	{
		c->x = c->d.x;
		while (c->x < c->stopx && c->x < WIN_X)
		{
			get_ray_dir(c->e, &c->cr, (double)c->x, (double)c->d.y);
			intersect_scene(c->e);
			c->pixel = (c->d.y * c->e->px_pitch + c->x * 4);
			*(uint32_t *)(c->e->px + c->pixel) = find_colour(c->e);
			++c->x;
		}
		++c->d.y;
	}
	free(c->e);
	free(c);
	pthread_exit(0);
}

static t_env	*copy_env(t_env *e)
{
	t_env	*res;

	res = (t_env *)malloc(sizeof(t_env));
	res->win = e->win;
	res->rend = e->rend;
	res->img = e->img;
	res->px = e->px;
	res->px_pitch = e->px_pitch;
	res->ray = e->ray;
	res->camera = e->camera;
	res->p_hit = e->p_hit;
	res->prim = e->prim;
	res->prims = e->prims;
	res->o_hit = e->o_hit;
	res->object = e->object;
	res->objects = e->objects;
	res->light = e->light;
	res->lights = e->lights;
	res->material = e->material;
	res->materials = e->materials;
	res->t = e->t;
	return (res);
}

static void		make_chunks(t_env *e, SDL_Rect *d)
{
	t_make_chunks	m;

	m.tids = 1/*ceil((double)d->w / 64.0) * ceil((double)d->h / 64.0)*/;
	m.tid = (pthread_t *)malloc(sizeof(pthread_t) * m.tids);
	m.thread = 0;
	m.chunk_y = 0;
//	while (m.chunk_y * 64 < (size_t)d->h)
//	{
//		m.chunk_x = 0;
//		while (m.chunk_x * 64 < (size_t)d->w)
//		{
			m.c = (t_chunk *)malloc(sizeof(t_chunk));
			m.c->e = copy_env(e);
			m.c->d = (SDL_Rect){/*m.chunk_x * 64, m.chunk_y * 64, 64, 64*/0,0,d->w, d->h};
			pthread_create(&m.tid[m.thread++], NULL, draw_chunk, (void *)m.c);
//			++m.chunk_x;
//		}
//		++m.chunk_y;
//	}
	while (m.thread)
		pthread_join(m.tid[--m.thread], NULL);
	free(m.tid);
}

void			draw(t_env *e, SDL_Rect d)
{
	time_t	t;

	t = time(NULL);
	SDL_LockTexture(e->img, NULL, &e->px, &e->px_pitch);
	make_chunks(e, &d);
	SDL_UnlockTexture(e->img);
	SDL_RenderCopy(e->rend, e->img, NULL, NULL);
	SDL_RenderPresent(e->rend);
	t = time(NULL) - t;
	ft_printf("Frame drawn in %d seconds\n", t);
}
