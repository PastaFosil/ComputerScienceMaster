/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5

 * Se crean estructuras de describen un punto o un 
 * rectangulo y se analizan usando funciones
************************************************************/

#include <stdio.h>
#define true 1
#define false 0

struct point {
    int x,y;
};

struct rectangle {
    struct point upper_left, lower_right;
};

/*Calcula area de la figura*/
int area_rectangle(struct rectangle r){
    return (r.lower_right.x-r.upper_left.x)*(r.upper_left.y-r.lower_right.y);
}

/*Calcula el punto central de r*/
struct point center(struct rectangle r){
    struct point c;

    c.x = r.lower_right.x-(r.lower_right.x-r.upper_left.x)/2;
    c.y = r.upper_left.y-(r.upper_left.y-r.lower_right.y)/2;

    return c;
}

/*Traslada r en la direccion especificada*/
struct rectangle offset(struct rectangle r, int x, int y){
    struct rectangle new_r;

    new_r.lower_right.x = r.lower_right.x + x;
    new_r.lower_right.y = r.lower_right.y + y;
    new_r.upper_left.x = r.upper_left.x + x;
    new_r.upper_left.y = r.upper_left.y + y;

    return new_r;
}

/*Revisa si el punto dado se encuentra dentro de la figura o no*/
int inside_rectangle(struct rectangle r, struct point p){
    int inside = true;

    if (p.x<r.upper_left.x||p.x>r.lower_right.x)
        inside = false;
    if (p.y<r.lower_right.y||p.y>r.upper_left.y)
        inside = false;
    
    return inside;
}

int main(){
    struct rectangle rec, new;
    struct point c;

    rec.lower_right.x = 3;
    rec.lower_right.y = 1;
    rec.upper_left.x = -2;
    rec.upper_left.y = 6;

    printf("Area=%d\n",area_rectangle(rec));
    
    c = center(rec);
    printf("Punto central: (%d,%d)\n",c.x,c.y);

    new = offset(rec,1,1);
    c = center(new);
    printf("Punto central: (%d,%d)\n",c.x,c.y);

    struct point p;
    p.x = 1;
    p.y = 5;
    printf("inside (%d,%d): %d\n",p.x,p.y, inside_rectangle(rec,p));
}