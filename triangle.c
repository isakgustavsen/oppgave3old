#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "triangle.h"
#include "drawline.h"

#define TRIANGLE_PENCOLOR   0xBBBB0000

/*
 * Print triangle coordinates along with a message
 */
void print_triangle(triangle_t *triangle, char *msg)
{
    printf("%s: %d,%d - %d,%d - %d,%d\n",
        msg,
        triangle->x1, triangle->y1,
        triangle->x2, triangle->y2,
        triangle->x3, triangle->y3);
}

/*
 * Return 0 if triangle coordinates are outside the surface boundary. 1 otherwise.
 */
int sanity_check_triangle(SDL_Surface *surface, triangle_t *triangle)
{
    if (triangle->sx1 < 0 || triangle->sx1 >= surface->w ||
        triangle->sx2 < 0 || triangle->sx2 >= surface->w ||
        triangle->sx3 < 0 || triangle->sx3 >= surface->w ||
        triangle->sy1 < 0 || triangle->sy1 >= surface->h ||
        triangle->sy2 < 0 || triangle->sy2 >= surface->h ||
        triangle->sy3 < 0 || triangle->sy3 >= surface->h) {
        return 0;
    } else {
        return 1;
    }
}

/*
 * Scale triangle, altering the on-screen coordinates(e.g. triangle->sx1)
 */
void scale_triangle(triangle_t *triangle)
{


    triangle->sx1 = triangle->x1 * triangle->scale;
    triangle->sy1 = triangle->y1 * triangle->scale;

    triangle->sx2 = triangle->x2 * triangle->scale;
    triangle->sy2 = triangle->y2 * triangle->scale;

    triangle->sx3 = triangle->x3 * triangle->scale;
    triangle->sy3 = triangle->y3 * triangle->scale;
}

/*
 * Move the triangle to the center of the surface,
 * altering the on-screen coordinates(e.g. triangle->sx1)
 */
void translate_triangle(triangle_t *triangle)
{

    //Henter ut skjermstørrelsen fra main.c og flytter tekannen til midten
    //ved å legge til halvedelen på x og y
    triangle->tx = 600;
    triangle->ty = 600;
    triangle->sx1 += triangle->tx;
    triangle->sy1 += triangle->ty;

    triangle->sx2 += triangle->tx;
    triangle->sy2 += triangle->ty;

    triangle->sx3 += triangle->tx;
    triangle->sy3 += triangle->ty;

}

/*
 * Calculate the triangle bounding box,
 * altering fields of the triangle's rect(e.g. triangle->rect.x)
 */
void calculate_triangle_bounding_box(triangle_t *triangle)
{

    //Setter startkordinat for boundingbozen som de andre kan sammenlignes med
    triangle->rect.x = triangle->sx1;
    triangle->rect.w = triangle->sx1;
    triangle->rect.y = triangle->sy1;
    triangle->rect.h = triangle->sy1;

    //Sammenligner de satte kordinatene med de andre mulighetene og setter
    //x og y til det laveste den finner og w og h til det høyeste
    if (triangle->sx2 < triangle->rect.x){triangle->rect.x = triangle->sx2;}
    if (triangle->sx3 < triangle->rect.x){triangle->rect.x = triangle->sx3;}

    if (triangle->sy2 < triangle->rect.y){triangle->rect.y = triangle->sy2;}
    if (triangle->sy3 < triangle->rect.y){triangle->rect.y = triangle->sy3;}

    if (triangle->sx2 > triangle->rect.w){triangle->rect.w = triangle->sx2;}
    if (triangle->sx3 > triangle->rect.w){triangle->rect.w = triangle->sx3;}

    if (triangle->sy2 > triangle->rect.h){triangle->rect.h = triangle->sy2;}
    if (triangle->sy3 > triangle->rect.h){triangle->rect.h = triangle->sy3;}

}


void fill_triangle(SDL_Surface *surface, triangle_t *triangle, int x, int y)
{
    //Fargekoden er iterativ og vil derfor kjøre seg seg om igjen til den ikke har flere ruter å fylle
    //Først sjekker koden at pixelen den har fått er innenfor området den forventer. Hvis den er det
    //går den videre til å sjekke om den tegner på enten en linje eller et allerede utfyllt felt. Hvis den gjør
    //det vil den gå videre til neste iterasjon og prøve igjen. HVis felte ikke er fylt ut vil den gjøre det
    //og gå videre til neste pixel

  if (x <= triangle->rect.w && y <= triangle->rect.h && x > triangle->rect.x && y > triangle->rect.y) {
      if(get_pixel(surface,x,y) != triangle->fillcolor){
        set_pixel(surface,x,y,triangle->fillcolor);
        fill_triangle(surface,triangle,x+1,y);
        fill_triangle(surface,triangle,x-1,y);
        fill_triangle(surface,triangle,x,y+1);
        fill_triangle(surface,triangle,x,y-1);
      }
    }
}

 //Draw a filled triangle on the given surface

void draw_triangle(SDL_Surface *surface, triangle_t *triangle)
{
    int isOK;


    /* Scale. */
    scale_triangle(triangle);

    /* Translate. */
    translate_triangle(triangle);

    /* Determine bounding box */
    calculate_triangle_bounding_box(triangle);

    /* Sanity check that triangle is within surface boundaries. */
    isOK = sanity_check_triangle(surface, triangle);
    if (!isOK) {
        print_triangle(triangle, "Triangle outside surface boundaries");
        return;
    }

    /*
     * TODO: Insert calls to draw_line to draw the triangle.
     * Remember to use the on-surface coordinates (triangle->sx1, etc.)
     */
     draw_line(surface,triangle->sx1,triangle->sy1,triangle->sx2,triangle->sy2,triangle->fillcolor);
     draw_line(surface,triangle->sx2,triangle->sy2,triangle->sx3,triangle->sy3,triangle->fillcolor);
     draw_line(surface,triangle->sx3,triangle->sy3,triangle->sx1,triangle->sy1,triangle->fillcolor);

    /* Fill triangle */
    //Finner midten av trekanten ved å legge sammen x og y fra alle punktene og dele de på tre
    int midx = (triangle->sx1 + triangle->sx2 + triangle->sx3)/3;
    int midy = (triangle->sy1 + triangle->sy2 + triangle->sy3)/3;
    fill_triangle(surface, triangle,midx,midy);
}
