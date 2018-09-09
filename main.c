/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <azulbukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 15:35:47 by azulbukh          #+#    #+#             */
/*   Updated: 2018/09/09 22:06:11 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define WIN_X 1900
#define WIN_Y 1000

//3rd bar
#define BAR_X 1400
#define BAR_Y 100
#define BAR_W 400
#define BAR_H 800

#define MAP_REC_X 100
#define MAP_REC_Y 100
#define MAP_REC_W 1300
#define MAP_REC_H 900

typedef struct s_line
{
	int x1;
	int y1;
	int x2;
	int y2;
	int texture_number;
	int height;
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
	x = 1375;
	y = 250;
	while (i < 16)
	{
		if (i == 4 || i == 8 || i == 12)
		{
			y += 100;
			x = 1375;
		}
		global->texture_rect[i].x = x;  //the x coordinate
		global->texture_rect[i].y = y; // the y coordinate
		global->texture_rect[i].w = 75; //the width of the texture
		global->texture_rect[i].h = 75; //the height of the texture
		SDL_RenderCopy(global->renderer, texture, NULL, &global->texture_rect[i]);
		x += 100;
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
		printf("[x1][%d][y1][%d]\t[x2][%d][y2][%d]\t[texture][%d][height][%d]\n", cur->x1, cur->y1, cur->x2, cur->y2, cur->texture_number, cur->height);
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
	// printf("sometext\n");
 	SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
 	SDL_Rect rect = {xs, ys, x, y};
 	SDL_RenderCopy(global.renderer, global.texture, NULL, &rect);
	// free(s);
	// s = NULL;
	SDL_FreeSurface(surface);
}

int		in_save_to_file(int x, int y)
{
	if (x > 1375 && y > 775 && x <= 1750 && y <= 875)
		return (1);
	else
		return (0);
}

void	make_exit(t_line **lines)
{
	t_line *cur;
	int fd;
	char *s;
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

void	draw_lines_rect(t_line *lines, t_global global)
{
	//rec left
	SDL_Rect rec1;

	rec1.x = 100;
	rec1.y = 100;
	rec1.w = 1200;
	rec1.h = 800;
	SDL_RenderDrawRect(global.renderer, &rec1);


	//rec in right bar for info
	SDL_Rect bar2;
	bar2.x = 1375;
	bar2.y = 125;
	bar2.w = 375;
	bar2.h = 100;
	SDL_RenderDrawRect(global.renderer, &bar2);

	//rec in right bar for exit
	SDL_Rect bar;
	bar.x = 1375;
	bar.y = 775;
	bar.w = 375;
	bar.h = 100;
	SDL_RenderDrawRect(global.renderer, &bar);

	//rec in right bar for input
	SDL_Rect bar1;
	bar1.x = 1375;
	bar1.y = 650;
	bar1.w = 375;
	bar1.h = 100;
	SDL_RenderDrawRect(global.renderer, &bar1);

	//rec right
	SDL_Rect rec;
	rec.x = 1350;
	rec.y = 100;
	rec.w = 425;
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

int	in_texture(int x, int y)
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

int	dot_in_map(int x1, int y1)
{
	if (x1 > MAP_REC_X && y1 > MAP_REC_Y && x1 < MAP_REC_W && y1 < MAP_REC_H)
		return (0);
	else
		return (1);								
}

int main(int argc, char* argv[])
{
	int x1 = 0;
	int y1 = 0;
	int x2 = 110;
	int y2 = 110;
	int z = 0;
	int drawing = 0;
	char text[9];
	// linked list of walls with texture, height and cords.
	t_line *lines = NULL;
	SDL_Event e;
	// global position link to the lines and current mode
	// 0 pick texture
	// 1 set 2 lines 
	// 2 set height of wall
	t_global global; 
	global.pos = 0;
	//texture
	SDL_Texture *texture = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
	   	global.window = SDL_CreateWindow("WOLF_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, SDL_WINDOW_SHOWN);
		global.renderer = SDL_CreateRenderer(global.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_bool done = SDL_FALSE;
		TTF_Init();
		global.font = TTF_OpenFont("font.ttf", 50);
		// SDL_RenderClear(renderer);
		// SDL_Color color = { 255, 255, 255 };
 		// SDL_Surface * surface = TTF_RenderText_Solid(global.font,
 		// 		"Pick texture", color);
		// global.texture = SDL_CreateTextureFromSurface(global.renderer, surface);
 		// int x, y = 50;
 		// SDL_QueryTexture(global.texture, NULL, NULL, &x, &y);
 		// SDL_Rect rect = {100, 100, x, y};
		SDL_Texture *texture = IMG_LoadTexture(global.renderer, "texture0.jpg");
		if (texture == NULL)
		{
			fprintf(stderr, "Texture error");
			exit(1);
		}
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
					if (global.pos == 0)
					{
						if (event.type == SDL_MOUSEBUTTONDOWN)
						{
							SDL_GetMouseState(&x1, &y1);
							if ((global.current_texture = in_texture(x1, y1)) > 0)
							{
								// SDL_LogMessage(SDL_LOG_CATEGORY_TEST, SDL_LOG_PRIORITY_DEBUG, ft_itoa(global.current_texture));
								global.pos = 1;
							}
							if (in_save_to_file(x1, y1) > 0)
							{
								SDL_LogMessage(SDL_LOG_CATEGORY_TEST, SDL_LOG_PRIORITY_DEBUG, "exit");
								make_exit(&lines);
							}
						}
					}
					else if (global.pos == 1)
					{
						//wait for user height input
						// change pos to 3
						if (event.type == SDL_MOUSEBUTTONDOWN)
						{
							if (SDL_GetMouseState(&x1, &y1) & SDL_BUTTON(SDL_BUTTON_LEFT))
							{
								if ((dot_in_map(x1, y1)) || (dot_in_map(x2, y2)))
									break ;
								if (!drawing)
								{
									x2 = x1;
									y2 = y1;
									drawing = 1;
								}
								else
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
									global.pos = 2;									
									drawing = 0;
								}
							}
						}
					}
					else if (global.pos == 2 && strlen(text) <= 7)
					{
						//save
						if (event.type == SDL_TEXTINPUT)
						{
							printf("[%s][%s]\n", text, event.text.text);
							strcat(text, event.text.text);
						}
					}
					if (global.pos == 2)
					{
						if (event.type == SDL_KEYUP)
						{
							if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "Backspace") == 0)
							{
								printf("escape\n");
								text[strlen(text) - 1] = '\0';
							}
						}
						if (strcmp(SDL_GetKeyName(event.key.keysym.sym), "Return") == 0)
						{
							printf("Enter\n");
							lines->height = atoi(text);
							lines->texture_number = global.current_texture - 1;
							bzero(text, strlen(text));
							global.pos = 0;
						}
					}
		  		}
				SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 255);
				SDL_RenderClear(global.renderer);
				
				if (global.pos == 0)
				{
					put_text("Pick Texture", 165, global, 1450);

						//write pick texture
						// put_text("Pick texture", 300, global, 300);
						//wait when user picks texture from menu
						//change pos to 1
				}
				else if (global.pos == 1)
				{
					put_text("Pick 2 Dots", 165, global, 1450);
				
						//write pick texture
						// put_text("Pick texture", 300, global, 300);
						//wait when user picks texture from menu
						//change pos to 1
				}
				// // texture
				else if (global.pos == 2)
				{
					put_text("Set Height", 165, global, 1450);
					put_text(text, 685, global, 1400);
				}
				put_text("Save to file", 805, global, 1450);
				set_rect_for_texture(&global, texture);
 				// SDL_RenderCopy(global.renderer, global.texture, NULL, &rect);
				SDL_SetRenderDrawColor(global.renderer, 255, 255, 255, 0);
				// for loop for elemnts in struct;
				draw_lines(lines, global.renderer);
				// draw x1y1x2y2 to window unisng this function
				// draw_lines_to_menu(lines, global);
				// draw viewport to window
				draw_lines_rect(lines, global);

				SDL_RenderPresent(global.renderer);
			}
		// }
		
		// SDL_FreeSurface(surface);
		if (global.renderer) {
			SDL_DestroyRenderer(global.renderer);
		}
		if (global.window) {
			SDL_DestroyWindow(global.window);
		}
	}
	SDL_Quit();
	// while (1);	
	system("leaks a.out");
	return 0;
}