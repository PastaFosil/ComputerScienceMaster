/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 7 C++
 * 
 * Calcula arco seno, arco coseno y arco tangente como 
 * metodos de una clase.
************************************************************/

#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
using namespace std;

const double PI = 3.141592653589793;
double TOL = numeric_limits<double>::epsilon();

class Inversas{
    private:
        double busqueda_binaria(double res, double (*f)(double), double xmin, double xmax, double tol){
            double xcen = (xmin + xmax)/2, prev = NAN;
            while (fabs(f(xcen)-res)>tol && xcen!=prev){
                if (f(xcen)>res) xmax = xcen;
                else xmin = xcen;
                prev = xcen;
                xcen = (xmin + xmax)/2;
            }
            return xcen;
        }
    public:
        /*
        Devuelve el valor del arco seno de x con una cierta tolerancia tol; 
        si no se define tol, se empleara la epsilon de la maquina.
        */
        void arcsen(double x, double tol=TOL){
            if (fabs(x)>1){
                cout << "Error: x fuera de rango" << endl;
                return;
            }
            double res = busqueda_binaria(x, sin, -PI/2, PI/2, tol);
            double func = asin(x);
            cout << left << setw(15) << "Biseccion" << setw(15) << "Funcion" << setw(15) << "Error" << endl;
            cout << fixed << setprecision(10); // Fixed point, 10 decimal places
            cout << left << setw(15) << res << setw(15) << func << setw(15) << fabs(res - func) << endl;
            
        }

        /*
        Devuelve el valor del arco coseno de x con una cierta tolerancia tol; 
        si no se define tol, se empleara la epsilon de la maquina.
        */
        void arccos(double x, double tol=TOL){
            if (fabs(x)>1){
                cout << "Error: x fuera de rango" << endl;
                return;
            }
            double res = -busqueda_binaria(x, cos, -PI, 0, tol);
            double func = acos(x);
            cout << left << setw(15) << "Biseccion" << setw(15) << "Funcion" << setw(15) << "Error" << endl;
            cout << fixed << setprecision(10); // Fixed point, 10 decimal places
            cout << left << setw(15) << res << setw(15) << func << setw(15) << fabs(res - func) << endl;
        }

        /*
        Devuelve el valor del arco tangente de x con una cierta tolerancia tol; 
        si no se define tol, se empleara la epsilon de la maquina.
        */
        void arctan(double x, double tol=TOL){
            double res = busqueda_binaria(x, tan, -PI/2, PI/2, tol);
            double func = atan(x);
            cout << left << setw(15) << "Biseccion" << setw(15) << "Funcion" << setw(15) << "Error" << endl;
            cout << fixed << setprecision(10); // Fixed point, 10 decimal places
            cout << left << setw(15) << res << setw(15) << func << setw(15) << fabs(res - func) << endl;
        }
};

int main(){
    Inversas inv;
    double x, lim = TOL;
    
    cout << "Ingrese un valor para x: ";
    cin >> x;
    cout << "\nSEN\n";
    inv.arcsen(x, lim);
    cout << "\nCOS\n";
    inv.arccos(x, lim);
    cout << "\nTAN\n";
    inv.arctan(x, lim);
    return 0;
}