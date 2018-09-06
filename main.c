/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azulbukh <azulbukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 15:35:47 by azulbukh          #+#    #+#             */
/*   Updated: 2018/09/06 16:40:13 by azulbukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

#define WIN_X 1280
#define WIN_Y 960

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
    int pos;
}               t_global;

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
    t_global gl; 

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
       	window = SDL_CreateWindow("WOLF_SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_X, WIN_Y, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                SDL_Color color = { 255, 255, 255 };
				SDL_Surface * surface = TTF_RenderText_Solid(font,
 				"Pick texture", color);
 				SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
 				int x, y = 50;
 				SDL_QueryTexture(texture, NULL, NULL, &x, &y);
 				SDL_Rect rect = {100, 100, x, y};
 				SDL_RenderCopy(renderer, texture, NULL, &rect);
				// SDL_RenderPresent(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
                //for loop for elemnts in struct;
                draw_lines(lines, renderer);
                SDL_RenderPresent(renderer);
            }
        // }
        

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}