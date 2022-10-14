#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "drawline.h"
#include "triangle.h"
#include "teapot_data.h"

triangle_t exampletriangle1 = {
    .x1 = 60,
    .y1 = 80,
    .x2 = 50,
    .y2 = 70,
    .x3 = 70,
    .y3 = 60,
    .fillcolor = 0xffff0000,
    .scale = 1.0
};

triangle_t exampletriangle2 = {
    .x1 = 799,
    .y1 = 150,
    .x2 = 150,
    .y2 = 50,
    .x3 = 250,
    .y3 = 250,
    .fillcolor = 0xffffff00,
    .scale = 1.0
};

triangle_t exampletriangle3 = {
    .x1 = 350,
    .y1 = 350,
    .x2 = 460,
    .y2 = 300,
    .x3 = 500,
    .y3 = 400,
    .fillcolor = 0xff00ff00,
    .scale = 1.0
};

triangle_t exampletriangle4 = {
    .x1 = 350,
    .y1 = 100,
    .x2 = 450,
    .y2 = 50,
    .x3 = 500,
    .y3 = 200,
    .fillcolor = 0xff0000ff,
    .scale = 1.0
};


int main(void)
{
    const size_t bufsize = 100;

    /* Change the screen width and height to your own liking */
    const int screen_w = 1200;
    const int screen_h = 1200;

    char errmsg[bufsize];
    int done;
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Event event;

    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < -1) {
        snprintf(errmsg, bufsize, "Unable to initialize SDL.");
        goto error;
    }

    /* Create a 1600x900 window */
    window = SDL_CreateWindow("The Amazing Teapot",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              0);
    if(!window) {
        snprintf(errmsg, bufsize, "Unable to get video surface.");
        goto error;
    }

    /* Create window renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        snprintf(errmsg, bufsize, "Unable to create window renderer.");
        goto error;
    }

    /* Create the texture that contain the currentrly drawn pixels */
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC,
                                screen_w, screen_h);
    if(!texture) {
        snprintf(errmsg, bufsize, "Unable to create texture.");
        goto error;
    }

    /* Create the suface in RAM that we manipulate the pixels of */
    surface = SDL_CreateRGBSurface(0, screen_w, screen_h, 32,
                                   0x00FF0000,
                                   0x0000FF00,
                                   0x000000FF,
                                   0xFF000000);
    if(!surface) {
        snprintf(errmsg, bufsize, "Unable to create surface.");
        goto error;
    }

    /*
     * The teapot is represented as an array of triangle data structures.
     * To draw it on the screen you need to traverse the 'teapot_model' array
     * and call draw_triangle for each triangle (teapot_data.h contains the array).
     * The definition TEAPOT_NUMTRIANGLES specifies the number of triangles in the array.
     * The teapot model is contained within a 1000x1000 box (coordinates
     * from -500 to 500 on the x and y axis).  Remember to translate the
     * model to the middle of the screen before drawing it (initialize
     * triangle->tx and triangle->ty with the appropriate coordinates).
     */

    /*
     * Draw some example triangles on the screen.
     * Use these examples in the beginning.
     *
     * Remove these and draw the triangles that represent he teapot
     */

     for (int i = 0; i < TEAPOT_NUMTRIANGLES; i++) {
       //triangle_t teapotstuff = ;
       draw_triangle(surface, &teapot_model[i]);
     }/*
    draw_triangle(surface, &exampletriangle1);
    draw_triangle(surface, &exampletriangle2);
    draw_triangle(surface, &exampletriangle3);
    draw_triangle(surface, &exampletriangle4);*/
    /* Update the texture, it to the video buffer and present it */
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    /* Wait for the user to exit the application */
    done = 0;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                done = 1;
                break;
            }
        }
    }

    SDL_Quit();

    return 0;

    /* Upon an error, print message and quit properly */
error:
    fprintf(stderr, "%s Error returned: %s\n", errmsg, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
