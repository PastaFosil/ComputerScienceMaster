/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5 C++
 * 
 * Verifica si el numero ingresado es un flotante valido y
 * la evalua para convertirla a la norma IEEE754.
************************************************************/

#include <iostream>
#include <regex>
#include <string>
#include <cmath>
using namespace std;

bool isFloat(std::string str){
    regex pattern("[+-]?[0-9]+[.][0-9]*");
    if (regex_match(str, pattern))
        return true;
    else
        return false;
}

void iee754(float in, char *ieee){
    ieee[32] = '\0';
    // SIGNO
    if (in<0.0) ieee[0] = '1';
    else ieee[0] = '0';
    float num = fabs(in);

    // EXPONENTE
    int xp = 0;
    // "normaliza" el numero a uno de la forma 1.xxx
    if (num>=1.0){
        while (num>2.0){
            num /= 2;
            xp++;
        }
    } else {
        while (static_cast<int>(num)<=0){
            num *= 2;
            xp--;
        }
    }
    // conversion del exponente a binario
    int i = 8;
    if (num!=0)
        xp += 127;
    while (xp>0){
        if (xp%2==0) ieee[i] = '0';
        else ieee[i] = '1';

        xp /= 2;

        i--;
    }
    for (i;i>0;i--) ieee[i] = '0';

    // MANTISA
    float entero = static_cast<float>(static_cast<int>(num));
    float decimal = num-entero;
    i = 9;
    // convierte la mantisa a binario
    while (decimal>0.0 && i<32){
        decimal *= 2.0;
        entero = static_cast<float>(static_cast<int>(decimal));
        if (static_cast<int>(entero)==0) ieee[i] = '0';
        else ieee[i] = '1';
        decimal = decimal-entero;
        i++;
    }
    for (i;i<32;i++)
        ieee[i] = '0';
}

int main(int argc, char **argv){
    if (argc<2){
        cout << "NO SE INGRESO NINGUN NUMERO" << endl;
        return -1;
    }

    if (!isFloat(argv[1])){
        cout << "NO SE INGRESO UN NUMERO VALIDO" << endl;
        return -1;
    }

    char str[33];
    float num = stof(argv[1]);
    iee754(num, str);

    cout << "IEEE754 de " << num << ": ";
    for (int i=0;i<33;i++){
        cout << str[i];
        if (i==0) cout << " ";
        else if (i==8) cout << " ";
    }
    cout << endl;

    // EPSILON
    float epsilon = 1.0f;
    while (num+epsilon>num) epsilon /= 2.0f;
    epsilon *= 2.0f;

    // REPRESENTACION DE 1.0 Y 1.0+epsilon
    iee754(num+epsilon, str);
    cout << "IEEE754 de " << num << " + " << epsilon << ": ";
    for (int i=0;i<33;i++){
        cout << str[i];
        if (i==0) cout << " ";
        else if (i==8) cout << " ";
    }
    cout << endl;
}