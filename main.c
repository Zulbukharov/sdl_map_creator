/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <azulbukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 15:35:47 by azulbukh          #+#    #+#             */
/*   Updated: 2018/09/10 19:23:22 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom.h"

void			list_remove(t_line **list, t_line *node)
{
	t_line *tmp;

	if (list == NULL || *list == NULL || node == NULL)
		return ;
	if (*list == node)
	{
		*list = (*list)->next;
		free(node);
		node = NULL;
	}
	else
	{
		tmp = *list;
		while (tmp->next && tmp->next != node)
			tmp = tmp->next;
		if (tmp->next)
		{
			tmp->next = node->next;
			free(node);
			node = NULL;
		}
	}
}

void			list_destroy(t_line **list)
{
	if (list == NULL)
		return ;
	while (*list != NULL)
	{
		list_remove(list, *list);
	}
}

void			itoa_isnegative(int *n, int *negative)
{
	if (*n < 0)
	{
		*n *= -1;
		*negative = 1;
	}
}

char			*ft_itoa(int n)
{
	int		tmpn;
	int		len;
	int		negative;
	char	*str;

	if (n == -2147483648)
		return (strdup("-2147483648"));
	tmpn = n;
	len = 2;
	negative = 0;
	itoa_isnegative(&n, &negative);
	while (tmpn /= 10)
		len++;
	len += negative;
	if ((str = (char*)malloc(sizeof(char) * len)) == NULL)
		return (NULL);
	str[--len] = '\0';
	while (len--)
	{
		str[len] = n % 10 + '0';
		n = n / 10;
	}
	if (negative)
		str[0] = '-';
	return (str);
}

char			*concat(char *s1, char *s2)
{
	char *result;

	result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

void			set_rect_for_texture(t_global *global)
{
	int	i;
	int	x;
	int	y;

	i = 0;
	x = 1375;
	y = 250;
	while (i < 16)
	{
		if (i == 4 || i == 8 || i == 12)
		{
			y += 100;
			x = 1375;
		}
		global->texture_rect[i].x = x;
		global->texture_rect[i].y = y;
		global->texture_rect[i].w = 75;
		global->texture_rect[i].h = 75;
		SDL_RenderCopy(global->renderer,
			global->texture, NULL, &global->texture_rect[i]);
		x += 100;
		i++;
	}
}

t_line			*create(t_global global)
{
	t_line *q;

	q = malloc(sizeof(t_line));
	q->x1 = global.x1;
	q->y1 = global.y1;
	q->x2 = global.x2;
	q->y2 = global.y2;
	q->next = NULL;
	return (q);
}

void			push(t_line **head, t_global global)
{
	t_line *new;

	new = create(global);
	new->next = *head;
	*head = new;
}

SDL_Rect		*create_rect(int x, int y, int w, int h)
{
	SDL_Rect *new;

	new = (SDL_Rect*)(malloc(sizeof(SDL_Rect)));
	new->x = x;
	new->y = y;
	new->w = w;
	new->h = h;
	return (new);
}

void			put_text(char *s, int ys, t_global global, int xs)
{
	int				x;
	int				y;
	SDL_Color		color;
	SDL_Surface		*surface;
	SDL_Rect		*link;

	x = 30;
	y = 30;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	surface = TTF_RenderText_Solid(global.font, s, color);
	global.texture = SDL_CreateTextureFromSurface(global.renderer, surface);
	SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
	link = create_rect(xs, ys, x, y);
	SDL_RenderCopy(global.renderer, global.texture, NULL, link);
	free(link);
	SDL_FreeSurface(surface);
}

int				in_save_to_file(int x, int y)
{
	if (x > 1375 && y > 775 && x <= 1750 && y <= 875)
		return (1);
	else
		return (0);
}

void			make_exit(t_line **lines)
{
	t_line	*cur;
	int		fd;
	char	*s;

	cur = *lines;
	if (!(fd = open("map.txt", O_WRONLY | O_CREAT)))
	{
		printf("error");
		return ;
	}
	while (cur)
	{
		s = ft_itoa(cur->texture_number);
		write(fd, s, strlen(s));
		free(s);
		s = NULL;
		write(fd, " ", 1);
		cur = cur->next;
	}
}

void			draw_lines_rect(t_global global)
{
	SDL_Rect *link;

	link = create_rect(100, 100, 1200, 800);
	SDL_RenderDrawRect(global.renderer, link);
	free(link);
	link = create_rect(1375, 125, 375, 100);
	SDL_RenderDrawRect(global.renderer, link);
	free(link);
	link = create_rect(1375, 775, 375, 100);
	SDL_RenderDrawRect(global.renderer, link);
	free(link);
	link = create_rect(1375, 650, 375, 100);
	SDL_RenderDrawRect(global.renderer, link);
	free(link);
	link = create_rect(1350, 100, 425, 800);
	SDL_RenderDrawRect(global.renderer, link);
	free(link);
}

void			draw_lines(t_global global)
{
	t_line *cur;

	cur = global.lines;
	while (cur)
	{
		SDL_RenderDrawLine(global.renderer, cur->x1, cur->y1, cur->x2, cur->y2);
		cur = cur->next;
	}
}

int				in_texture(int x, int y)
{
	int i;
	int x1;
	int y1;

	i = 0;
	x1 = 1375;
	y1 = 250;
	while (i < 16)
	{
		if (i == 4 || i == 8 || i == 12 || i == 16)
		{
			y1 += 100;
			x1 = 1375;
		}
		if (x > x1 && y > y1 && x <= x1 + 75 && y <= y1 + 75)
			return (i + 1);
		i++;
		x1 += 100;
	}
	return (0);
}

int				dot_in_map(int x1, int y1)
{
	if (x1 > MAP_REC_X && y1 > MAP_REC_Y
		&& x1 < MAP_REC_W && y1 < MAP_REC_H)
		return (0);
	else
		return (1);
}

void			init_sdl(t_global *global)
{
	global->window = SDL_CreateWindow("WOLF_SDL",
	SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	WIN_X, WIN_Y, SDL_WINDOW_SHOWN);
	global->renderer = SDL_CreateRenderer(global->window,
		-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	global->pos = 0;
	global->x1 = 0;
	global->y1 = 0;
	global->x2 = 110;
	global->y2 = 110;
	global->z = 0;
	global->drawing = 0;
	TTF_Init();
	global->font = TTF_OpenFont("font.ttf", 50);
	global->texture = IMG_LoadTexture(global->renderer, "texture0.jpg");
	if (global->texture == NULL)
	{
		fprintf(stderr, "Texture error");
		exit(1);
	}
	global->done = SDL_FALSE;
}

void			pos_0_pick_tex(t_global *global)
{
	if (global->event.type == SDL_MOUSEBUTTONDOWN)
	{
		SDL_GetMouseState(&global->x1, &global->y1);
		if ((global->current_texture = in_texture(global->x1, global->y1)) > 0)
		{
			global->pos = 1;
		}
		if (in_save_to_file(global->x1, global->y1) > 0)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_TEST,
				SDL_LOG_PRIORITY_DEBUG, "exit");
			make_exit(&global->lines);
		}
	}
}

void			pos_1_set_dot(t_global *global)
{
	if (global->event.type == SDL_MOUSEBUTTONDOWN &&
	(SDL_GetMouseState(&global->x1, &global->y1) & SDL_BUTTON(SDL_BUTTON_LEFT)))
	{
		if ((dot_in_map(global->x1, global->y1)) ||
		(dot_in_map(global->x2, global->y2)))
			return ;
		if (!global->drawing)
		{
			global->x2 = global->x1;
			global->y2 = global->y1;
			global->drawing = 1;
		}
		else
		{
			if (global->z)
				push(&global->lines, *global);
			else
			{
				global->lines = create(*global);
				global->z = 1;
			}
			global->pos = 2;
			global->drawing = 0;
		}
	}
}

void			global_pos_2_write(t_global *global)
{
	if (global->event.type == SDL_KEYUP)
		if (strcmp(SDL_GetKeyName(global->event.key.keysym.sym),
		"Backspace") == 0)
			global->text[strlen(global->text) - 1] = '\0';
	if (strcmp(SDL_GetKeyName(global->event.key.keysym.sym),
	"Return") == 0)
	{
		global->lines->height = atoi(global->text);
		global->lines->texture_number = global->current_texture - 1;
		bzero(global->text, strlen(global->text));
		global->pos = 0;
	}
}

void			event(t_global *global)
{
	if (global->event.type == SDL_QUIT)
	{
		list_destroy(&global->lines);
		TTF_Quit();
		global->done = SDL_TRUE;
	}
	if (global->pos == 0)
		pos_0_pick_tex(global);
	else if (global->pos == 1)
		pos_1_set_dot(global);
	else if (global->pos == 2 && strlen(global->text) <= 7)
		if (global->event.type == SDL_TEXTINPUT)
			strcat(global->text, global->event.text.text);
	if (global->pos == 2)
		global_pos_2_write(global);
}

void			loop(t_global global)
{
	while (!global.done)
	{
		while (SDL_PollEvent(&global.event))
			event(&global);
		SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 255);
		SDL_RenderClear(global.renderer);
		if (global.pos == 0)
			put_text("Pick Texture", 165, global, 1450);
		else if (global.pos == 1)
			put_text("Pick 2 Dots", 165, global, 1450);
		else if (global.pos == 2)
		{
			put_text("Set Height", 165, global, 1450);
			put_text(global.text, 685, global, 1400);
		}
		put_text("Save to file", 805, global, 1450);
		set_rect_for_texture(&global);
		SDL_SetRenderDrawColor(global.renderer, 255, 255, 255, 0);
		draw_lines(global);
		draw_lines_rect(global);
		SDL_RenderPresent(global.renderer);
	}
}

int				main(int argc, char *argv[])
{
	t_global global;

	if (SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		init_sdl(&global);
		loop(global);
		if (global.renderer)
		{
			SDL_DestroyRenderer(global.renderer);
		}
		if (global.window)
		{
			SDL_DestroyWindow(global.window);
		}
	}
	SDL_Quit();
	system("leaks a.out");
	return (0);
}
