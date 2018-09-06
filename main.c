/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <azulbukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 15:35:47 by azulbukh          #+#    #+#             */
/*   Updated: 2018/09/06 18:15:47 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <unistd.h>

#define WIN_X 1900
#define WIN_Y 1000

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
	SDL_Rect texture_rect[9];
}               t_global;

void    set_rect_for_texture(t_global *global, SDL_Texture *texture)
{
	int i;
	int x;
	int y;

	i = 0;
	x = 1000;
	y = 100;
	while (i < 9)
	{
		if (i == 3 || i == 6 || i == 9)
		{
			y += 300;
			x = 1000;
		}
		global->texture_rect[i].x = x;  //the x coordinate
		global->texture_rect[i].y = y; // the y coordinate
		global->texture_rect[i].w = 200; //the width of the texture
		global->texture_rect[i].h = 200; //the height of the texture
		SDL_RenderCopy(global->renderer, texture, NULL, &global->texture_rect[i]);
		x += 300;
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

void	draw_lines_to_menu(t_line *lines, t_global global)
{
	SDL_Rect rec;

	rec.x = 100;
	rec.y = 300;
	rec.w = 800;
	rec.h = 600;
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
		TTF_Font *font = TTF_OpenFont("04B_30__.TTF", 25);
		// SDL_RenderClear(renderer);
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
						TTF_Quit(); // destroy ttf
						done = SDL_TRUE;
					}
					if (event.type == SDL_MOUSEBUTTONDOWN)
					{
						if (SDL_GetMouseState(&x1, &y1) & SDL_BUTTON(SDL_BUTTON_LEFT))
						{
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
				
				// texture
				SDL_Texture *texture = IMG_LoadTexture(global.renderer, "texture0.jpg");
				if (texture == NULL)
				{
					fprintf(stderr, "Texture error");
					exit(1);
				}
				set_rect_for_texture(&global, texture);
				
				SDL_Color color = { 255, 255, 255 };
				SDL_Surface * surface = TTF_RenderText_Solid(font,
 				"Pick texture", color);
 				global.texture = SDL_CreateTextureFromSurface(global.renderer, surface);
 				int x, y = 50;
 				SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
 				SDL_Rect rect = {100, 100, x, y};
 				SDL_RenderCopy(global.renderer, global.texture, NULL, &rect);
				// SDL_RenderPresent(renderer);

				SDL_SetRenderDrawColor(global.renderer, 255, 255, 255, 0);
				//for loop for elemnts in struct;
				draw_lines(lines, global.renderer);
				

				draw_lines_to_menu(lines, global);

				SDL_RenderPresent(global.renderer);
			}
		// }
		

		if (global.renderer) {
			SDL_DestroyRenderer(global.renderer);
		}
		if (global.window) {
			SDL_DestroyWindow(global.window);
		}
	}
	SDL_Quit();
	return 0;
}