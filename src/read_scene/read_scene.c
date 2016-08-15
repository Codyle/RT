/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adippena <angusdippenaar@gmail.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/07/08 20:00:42 by adippena          #+#    #+#             */
/*   Updated: 2016/08/15 17:12:55 by rojones          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	scene_attributes(t_env *e, char *line)
{
	t_split_string	split;

	split = ft_nstrsplit(line, '\t');
	if (split.strings[0][0] != '#' && split.words != 2)
		err(FILE_FORMAT_ERROR, "scene_arrtibutes", e);
	if (!ft_strcmp(split.strings[0], "MAXDEPTH"))
		e->maxdepth = MAX(ft_atoi(split.strings[1]), 1);
	ft_free_split(&split);
}

static void	call_type(t_env *e, int fd, char **line)
{
	char	*temp_line;

	temp_line = ft_strtrim(*line);
	ft_strdel(line);
	if (!ft_strcmp(temp_line, "PRIMITIVE"))
		get_primitive_attributes(e, fd);
	if (!ft_strcmp(temp_line, "OBJECT"))
		get_object_attributes(e, fd);
	else if (!ft_strcmp(temp_line, "CAMERA"))
		get_camera_attributes(e, fd);
	else if (!ft_strcmp(temp_line, "LIGHT"))
		get_light_attributes(e, fd);
	else if (!ft_strcmp(temp_line, "MATERIAL"))
		get_material_attributes(e, fd);
	ft_strdel(&temp_line);
}

static void	default_material(t_env *e)
{
	e->material[0] = (t_material *)malloc(sizeof(t_material));
	init_material(e->material[0]);
	ft_strdel(&e->material[0]->name);
	e->material[e->materials]->name = ft_strdup("DEFAULT");
	++e->materials;
}

static void	get_quantities(t_env *e, int fd)
{
	char	*temp;
	char	*line;

	while (ft_gnl(fd, &temp) || temp != NULL)
	{
		line = ft_strtrim(temp);
		ft_strdel(&temp);
		if (!ft_strcmp(line, "LIGHT"))
			++e->lights;
		else if (!ft_strcmp(line, "MATERIAL"))
			++e->materials;
		else if (!ft_strcmp(line, "PRIMITIVE"))
			++e->prims;
		else if (!ft_strcmp(line, "OBJECT"))
			++e->objects;
		ft_strdel(&line);
	}
	ft_printf("%d:\tLIGHTS\n%d:\tMATERIALS\n%d:\tPRIMITIVES\n%d:\tOBJECTS\n",\
		(int)e->lights, (int)e->materials, (int)e->prims, (int)e->objects);
	e->light = (t_light **)malloc(sizeof(t_light *) * e->lights);
	e->material = (t_material **)malloc(sizeof(t_material *) * ++e->materials);
	e->prim = (t_prim **)malloc(sizeof(t_prim *) * e->prims);
	e->object = (t_object **)malloc(sizeof(t_object *) * e->objects);
	close(fd);
}

/*
**  33 lines :'(
*/

void		read_scene(char *file, t_env *e)
{
	int			fd;
	char		*line;
	char		*temp_line;

	if ((fd = open(file, O_RDONLY)) == -1)
		err(FILE_OPEN_ERROR, "read_scene", e);
	if (!(ft_gnl(fd, &line)))
		if (ft_strcmp(line, "# SCENE RT"))
			err(FILE_FORMAT_ERROR, "read_scene", e);
	ft_strdel(&line);
	get_quantities(e, fd);
	e->lights = 0;
	e->materials = 0;
	e->prims = 0;
	e->objects = 0;
	default_material(e);
	if ((fd = open(file, O_RDONLY)) == -1)
		err(FILE_OPEN_ERROR, "read_scene", e);
	while (ft_gnl(fd, &temp_line))
	{
		if (temp_line[0] == '\0')
		{
			ft_strdel(&line);
			break ;
		}
		line = ft_strtrim(temp_line);
		ft_strdel(&temp_line);
		scene_attributes(e, line);
		ft_strdel(&line);
	}
	while (ft_gnl(fd, &temp_line))
		call_type(e, fd, &temp_line);
	close(fd);
}
