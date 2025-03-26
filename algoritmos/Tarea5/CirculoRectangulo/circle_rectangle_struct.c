/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5

 * Se crean estructuras de describen un punto o un 
 * rectangulo y se analizan usando funciones
************************************************************/

#include <stdio.h>
#include <math.h>

#define RECTANGLE 1
#define CIRCLE 0
#define PI 3.14159

struct point {
    int x,y;
};

struct shape {
    int shape_kind;
    struct point center;
    union {
        struct {
            int height, width;
        } rectangle;
        struct {
            int radius;
        } circle;
    } u;
} s;

float area_shape(struct shape s){
    if (s.shape_kind==RECTANGLE)
        return (float)s.u.rectangle.height*(float)s.u.rectangle.width;
    if (s.shape_kind==CIRCLE)
        return PI*pow(s.u.circle.radius, 2); 
}

struct shape offset(struct shape s, int x, int y){
    struct shape new_s;

    new_s = *(&s);
    new_s.center.x += x;
    new_s.center.y += y;

    return new_s;
}

struct shape scale(struct shape s, double c){
    struct shape new_s;

    new_s = *(&s);
    if (new_s.shape_kind==RECTANGLE){
        new_s.u.rectangle.height *= c;
        new_s.u.rectangle.width *= c;
    } else
        new_s.u.circle.radius *= c;

    return new_s;
}

int main(){
    struct shape c, r, new;

    /******************************************************
     * 
     * // (a) s.shape_kind = RECTANGLE
     * s.shape_kind = RECTANGLE; // correcta
     * // (b) s.center.x = 10
     * s.center.x = 10; // correcta
     * // (c) s.height = 25
     * s.u.rectangle.height = 25; // incorrecta, corregida
     * // (d) s.u.rectangle.width = 8;
     * s.u.rectangle.width = 8; // correcta
     * // (e) s.u.circle = 5
     * s.u.circle.radius = 5; // incorrecta, corregida
     * // (f) s.u.radius = 5
     * s.u.circle.radius = 5; // incorrecta, corregida

    ******************************************************/

    c.shape_kind = CIRCLE;
    r.shape_kind = RECTANGLE;

    c.center.x = 6;
    c.center.y = 3;
    r.center.x = 4;
    r.center.y = 3;

    c.u.circle.radius = 12;
    r.u.rectangle.height = 10;
    r.u.rectangle.width = 5;

    printf("Area rectangulo: %f\n",area_shape(r));
    printf("Area circulo: %f\n",area_shape(c));

    new = offset(r,1,3);
    printf("\nOld center r: (%d,%d)\n",r.center.x,r.center.y);
    printf("New center r: (%d,%d)\n",new.center.x,new.center.y);

    new = offset(c,1,3);
    printf("\nOld center c: (%d,%d)\n",c.center.x,c.center.y);
    printf("New center c: (%d,%d)\n",new.center.x,new.center.y);

    printf("Area rectangulo: %f\n",area_shape(scale(r,2.5)));
    printf("Area circulo: %f\n",area_shape(scale(c,2.5)));

    return 0;
}