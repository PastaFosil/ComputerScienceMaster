/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 10
 * 
 * Devuelve el valor de los arreglos de coeficientes del
 * metodo de interpolacion cubica natural
************************************************************/

#include <stdio.h>
#include "../Tools/tools.c"

void natural_cubic_spline(double *data, double *a, double *c, double *b, double *d, int n){
    double h[n], alpha[n], l[n], mu[n], z[n];

    for (int i=0; i<n-1; i++)
        h[i] = data[i+1]-data[i];

    for (int i=1;i<n-1;i++)
        alpha[i] = 3*(a[i+1]-a[i])/h[i] - 3*(a[i]-a[i-1])/h[i-1];

    l[0] = 1;
    mu[0] = 0;
    z[0] = 0;

    for (int i=1;i<n;i++){
        l[i] = 2*(data[i+1]-data[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i]-h[i-1]*z[i-1])/l[i];
    }
    l[n-1] = 1;
    z[n-1] = 0;
    c[n-1] = 0;

    for (int j=n-2;j>=0;j--){
        c[j] = z[j]-mu[j]*c[j+1];
        b[j] = (a[j+1]-a[j])/h[j] - h[j]*(c[j+1]+2*c[j])/3;
        d[j] = (c[j+1]-c[j])/(3*h[j]);
    } 
}

int main()
{
    int n = 21;
    double data[] = {0.9, 1.3, 1.9, 2.1, 2.6, 3.0,3.9,4.4,4.7,5.0,6.0,7.0,8.0,9.2,10.5,11.3,11.6,12.0,12.6,13.0,13.3};
    double a[] = {1.3, 1.5, 1.85, 2.1, 2.6, 2.7,2.4,2.15,2.05,2.1,2.25,2.3,2.25,1.95,1.4,0.9,0.7,0.6,0.5,0.4,0.25};
    double c[n], b[n], d[n];

    natural_cubic_spline(n, data, a, c, b, d);

    print_vector(b, n);
    print_vector(c, n);
    print_vector(d, n);
}