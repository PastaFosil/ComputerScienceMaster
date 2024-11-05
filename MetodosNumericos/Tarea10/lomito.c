/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 10
 * 
 * Devuelve el valor de los arreglos de coeficientes del
 * metodo de interpolacion cubica fija
************************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../Tools/tools.c"

void clamped_cubic_spline(double data[], double a[], double b[], double c[], double d[], double fp[2], int n){
    double *h = genVector_double(n);
    double *alpha = genVector_double(n);
    double *l = genVector_double(n+1);
    double *mu = genVector_double(n);
    double *z = genVector_double(n+1);
    if (h==NULL || alpha==NULL || l==NULL || mu==NULL || z==NULL){
        printf("ERROR ASIGNANDO MEMORIA\n");
        return;
    }
    
    for (int i=0;i<n-1;i++)
        h[i] = data[i+1]-data[i];
    alpha[0] = 3*(a[1]-a[0])/h[0]-3*fp[0];
    alpha[n-1] = 3*fp[1]-3*(a[n-1]-a[n-2])/h[n-2];
    for (int i=1;i<n-1;i++)
        alpha[i] = 3*(a[i+1]-a[i])/h[i]-3*(a[i]-a[i-1])/h[i-1];
    l[0] = 2*h[0];
    mu[0] = 0.5;
    z[0] = alpha[0]/l[0];
    for (int i=1;i<n-1;i++){
        l[i] = 2*(data[i+1]-data[i-1])-h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
    }
    l[n-1] = h[n-2]*(2-mu[n-2]);
    z[n-1] = (alpha[n-1]-h[n-2]*z[n-2])/l[n-1];
    c[n-1] = z[n-1];
    for (int i=n-2;i>=0;i--){
        c[i] = z[i]-mu[i]*c[i+1];
        b[i] = (a[i+1]-a[i])/h[i]-h[i]*(c[i+1]+2*c[i])/3;
        d[i] = (c[i+1]-c[i])/(3*h[i]);
    }

    free(h);
    free(alpha);
    free(l);
    free(mu);
    free(z);
}

int main(){
    //int n = 9; double curve[] = {1.0, 2.0, 5.0, 6.0, 7.0, 8.0, 10.0, 13.0, 17.0}; double a[] = {3.0, 3.7, 3.9, 4.2, 5.7, 6.6, 7.1, 6.7, 4.5}; double fp[] = {1.0, -0.67};
    //int n = 7; double curve[] = {17.0, 20.0, 23.0, 24.0, 25.0, 27.0, 27.7}; double a[] = {4.5, 7.0, 6.1, 5.6, 5.8, 5.2, 4.1}; double fp[] = {3.0, -4.0};
    int n = 4; double curve[] = {27.7, 28.0, 29.0, 30.0}; double a[] = {4.1, 4.3, 4.1, 3.0}; double fp[] = {0.33, -1.5};
    
    double b[n+1], c[n+1], d[n+1];

    clamped_cubic_spline(curve, a, b, c, d, fp, n);
    print_vector(b, n+1);
    print_vector(c, n+1);
    print_vector(d, n+1);

    return 0;
}