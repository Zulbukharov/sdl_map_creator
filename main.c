/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <azulbukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 15:35:47 by azulbukh          #+#    #+#             */
/*   Updated: 2018/09/06 20:49:19 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WIN_X 1900
#define WIN_Y 1000

//3rd bar
#define BAR_X 1400
#define BAR_Y 100
#define BAR_W 400
#define BAR_H 800

typedef struct s_line
{
	int x1;
	int y1;
	int x2;
	int y2;
	struct s_line *next;
}				t_line;

typedef struct s_global
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	int pos;
	int current_texture;
	SDL_Texture *texture;
	SDL_Rect texture_rect[18];
	TTF_Font *font;
}               t_global;

void list_remove(t_line **list, t_line *node)
{
	t_line *tmp = NULL;
	if (list == NULL || *list == NULL || node == NULL) return;

	if (*list == node) {
		*list = (*list)->next;
		free(node);
		node = NULL;
	} else {
		tmp = *list;
		while (tmp->next && tmp->next != node) tmp = tmp->next;
		if (tmp->next) {
			tmp->next = node->next;
			free(node);
			node = NULL;
		}
	}
}

void list_destroy(t_line **list)
{
	if (list == NULL) return;
	while (*list != NULL) {
		list_remove(list, *list);
	}
}

void	itoa_isnegative(int *n, int *negative)
{
	if (*n < 0)
	{
		*n *= -1;
		*negative = 1;
	}
}

char	*ft_itoa(int n)
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

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void    set_rect_for_texture(t_global *global, SDL_Texture *texture)
{
	int i;
	int x;
	int y;

	i = 0;
	x = 975;
	y = 150;
	while (i < 18)
	{
		if (i == 3 || i == 6 || i == 9 || i == 12 || i == 15)
		{
			y += 125;
			x = 975;
		}
		global->texture_rect[i].x = x;  //the x coordinate
		global->texture_rect[i].y = y; // the y coordinate
		global->texture_rect[i].w = 100; //the width of the texture
		global->texture_rect[i].h = 100; //the height of the texture
		SDL_RenderCopy(global->renderer, texture, NULL, &global->texture_rect[i]);
		x += 125;
		i++;
	}
}

t_line *create(int x1, int y1, int x2, int y2)
{
	t_line *q;

	q = malloc(sizeof(t_line));
	q->x1 = x1;
	q->y1 = y1;
	q->x2 = x2;
	q->y2 = y2;
	q->next = NULL;
	return (q);
}

void	print(t_line *q)
{
	t_line *cur;

	cur = q;
	while (cur)
	{
		printf("[x1][%d][y1][%d]\t[x2][%d][y2][%d]\n", cur->x1, cur->y1, cur->x2, cur->y2);
		cur = cur->next;
	}
}

void	push(t_line **head, int x1, int y1, int x2, int y2)
{
	t_line *new;

	// if (!head || !x1 || !y1 || !y2 || x2)
	// 	return ;
	new = create(x1, y1, x2, y2);
	new->next = *head;
	*head = new;
}

void	put_text(char *s, int ys, t_global global, int xs)
{
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface * surface = TTF_RenderText_Solid(global.font, s, color);
 	global.texture = SDL_CreateTextureFromSurface(global.renderer, surface);
 	int x, y = 30;
 	SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
 	SDL_Rect rect = {xs, ys, x, y};
 	SDL_RenderCopy(global.renderer, global.texture, NULL, &rect);
	free(s);
	s = NULL;
	SDL_FreeSurface(surface);
}

void	draw_lines_to_menu(t_line *lines, t_global global)
{
	t_line *cur;
	int ys;

	ys = BAR_Y;
	cur = lines;
	while (cur)
	{
		put_text(ft_itoa(cur->x1), ys, global, BAR_X);
		put_text(ft_itoa(cur->y1), ys, global, BAR_X + 100);

		put_text(ft_itoa(cur->x2), ys, global, BAR_X + 200);
		put_text(ft_itoa(cur->y2), ys, global, BAR_X + 300);
		cur = cur->next;
		ys += 50;
	}
}

void	draw_lines_rect(t_line *lines, t_global global)
{
	//rec left
	SDL_Rect rec1;

	rec1.x = 100;
	rec1.y = 100;
	rec1.w = 800;
	rec1.h = 800;
	SDL_RenderDrawRect(global.renderer, &rec1);

	//rec middle
	SDL_Rect bar;
	bar.x = 950;
	bar.y = 100;
	bar.w = 400;
	bar.h = 800;
	SDL_RenderDrawRect(global.renderer, &bar);

	//rec right
	SDL_Rect rec;
	rec.x = 1400;
	rec.y = 100;
	rec.w = 400;
	rec.h = 800;
	SDL_RenderDrawRect(global.renderer, &rec);
}

void	draw_lines(t_line *lines, SDL_Renderer *renderer)
{
	t_line *cur;

	cur = lines;
	while (cur)
	{
		SDL_RenderDrawLine(renderer, cur->x1, cur->y1, cur->x2, cur->y2);
		cur = cur->next;
	}
}

int main(int argc, char* argv[])
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	int z = 0;
	int drawing = 0;
	// linked list of walls with texture, height and cords.
	t_line *lines = NULL;
	SDL_Event e;
	// global position link to the lines and current mode
	// 0 pick texture
	// 1 set 2 lines 
	// 2 set height of wall
	t_global global; 
	//texture
	SDL_Texture *texture = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
	   	global.window = SDL_CreateWindow("WOLF_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, SDL_WINDOW_SHOWN);
		global.renderer = SDL_CreateRenderer(global.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_bool done = SDL_FALSE;
		TTF_Init();
		global.font = TTF_OpenFont("font.ttf", 25);
		// SDL_RenderClear(renderer);
		SDL_Color color = { 255, 255, 255 };
 		SDL_Surface * surface = TTF_RenderText_Solid(global.font,
 				"Pick texture", color);
		global.texture = SDL_CreateTextureFromSurface(global.renderer, surface);
 		int x, y = 50;
 		SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
 		SDL_Rect rect = {100, 100, x, y};

		while (!done) 
		{
			SDL_Event event;
			// SDL_RenderClear(renderer);
			while (SDL_PollEvent(&event)) {
			// printf("[x1][%d]\t[y1][%d]\n", x2, y2);
			// printf("%hhu", event.button.button);
			   	// old_x = mouse_x;
					// old_y = mouse_y;
					if (event.type == SDL_QUIT) {
						// TTF_CLoseFont(font);
						list_destroy(&lines);
						TTF_Quit(); // destroy ttf
						done = SDL_TRUE;
					}
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						if (SDL_GetMouseState(&x1, &y1) & SDL_BUTTON(SDL_BUTTON_LEFT))
						{
							if (x1 >= 900 || x2 >= 900)
								break ;
							if (y1 >= 900 || y2 >= 900)
								break ;
							if (!drawing)
							{
								x2 = x1;
								y2 = y1;
								drawing = 1;
							}
							if (x1 != x2)
							{
								if (z)
								{
									push(&lines, x1, y1, x2, y2);
									printf("saved\n");
									print(lines);
								}
								else
								{
									lines = create(x1, y1, x2, y2);
									z = 1;
								}
								drawing = 0;
							}
						}
					}
		  		}
				SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 255);
				SDL_RenderClear(global.renderer);
				
				// // texture
				SDL_Texture *texture = IMG_LoadTexture(global.renderer, "texture0.jpg");
				if (texture == NULL)
				{
					fprintf(stderr, "Texture error");
					exit(1);
				}
				set_rect_for_texture(&global, texture);
 				SDL_RenderCopy(global.renderer, global.texture, NULL, &rect);

				SDL_SetRenderDrawColor(global.renderer, 255, 255, 255, 0);
				// //for loop for elemnts in struct;
				draw_lines(lines, global.renderer);
				
				// draw_lines_to_menu(lines, global);
				draw_lines_rect(lines, global);

				SDL_RenderPresent(global.renderer);
			}
		// }
		
		SDL_FreeSurface(surface);
		if (global.renderer) {
			SDL_DestroyRenderer(global.renderer);
		}
		if (global.window) {
			SDL_DestroyWindow(global.window);
		}
	}
	SDL_Quit();
	// while (1);	
	return 0;
}