/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * METODOS NUMERICOS
 * TAREA 12
 * 
 * Resuelve un sistema de ecuaciones diferenciales por el
 * metodo de Runge-Kutta
************************************************************/

#include <iostream>

using namespace std;

int N = 1000;

void rk_system(double *x0, double (*f[])(double, double*, int), double a, double b, int n){
    double h = (b-a)/N;
    double t = a;
    double w[n], wi[n], k1[n], k2[n], k3[n], k4[n];

    for (int i=0;i<n;i++)
        w[i] = x0[i];
    
    while (t<=b){
        for (int j=0;j<n;j++){
            k1[j] = h*f[j](t,w,n);
            wi[j] = w[j]+k1[j]/2;
        }
        t += h/2; // actualizacion de t para calculo de k2 y k3
        for (int j=0;j<n;j++){
            k2[j] = h*f[j](t,wi,n);
            wi[j] = w[j]+k2[j]/2;
        }
        for (int j=0;j<n;j++){
            k3[j] = h*f[j](t,wi,n);
            wi[j] = w[j]+k3[j];
        }
        t += h/2; // actualizacion de t para k4 y siguiente iteracion
        for (int j=0;j<n;j++)
            k4[j] = h*f[j](t,wi,n);
        
        cout << t << "\t";
        for (int j=0;j<n;j++){
            w[j] += (k1[j]+2*k2[j]+2*k3[j]+k4[j])/6;
            cout << w[j] << "\t";
        }
        cout << endl;
        
    }
}

double conejos(double t, double x0[], int n){
    return 0.4*x0[0]-0.018*x0[0]*x0[1];
}
double linces(double t, double x0[], int n){
    return -0.8*x0[1]+0.023*x0[0]*x0[1];
}

int main(){
    double alpha[] = {30.0,4.0};
    int n = sizeof(alpha)/sizeof(alpha[0]);
    double a = 0.0, b = 25.0;
    double (*f[])(double, double*, int) = {conejos, linces};

    rk_system(alpha, f, a, b, n);

    return 0;
}