/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 2 C++
 * 
 * Realiza operaciones ingresadas como parametros en la
 * ejecucion del programa
************************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
using namespace std;

// Devuelve 1 o 0 si la cadena ingresada es un numero entero o no
int valid_string(std::string str){
    int i = 0, len = str.length(), pass = 0;
    int special = 0;
    int sign = 0;
    while (i<len) {
        switch ((int)str[i]) {
            case '+':
                sign++;
            case '-':
                sign++;
            case '0':
                if ((i==0 && sign==0) || (i==1 && sign==1))
                    return 0;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            case 'e':
            case 'E':
                special++;
                if (special>1)
                    return 0;
                break;
            default:
                return 0;

        }
        if (sign>1 && special==0)
            return 0;
        i++;
    }
    return 1;
}

/* 
Devuelve un vector de enteros donde el primer elemento es su signo 
y los restantes, sus digitos. Si no se define el segundo argumento, 
su tamano es la cantidad de digitos del numero ingresado, de lo contrario
se devuelve un vector con los digitos faltantes en ceros.
*/
std::vector<int> digits_char(std::string str, int size = 0){
    if (valid_string(str)==0){
        std::cerr << "NUMERO INVALIDO" << endl;
        return 
    }
    int factor = 1, sign = 0;
    int len = str.length();
    if (size==0)
        size = len;
    if (str[0]=='-'){
        factor = -1;
        sign++;
        len--;
    } else if (str[0]=='+'){
        sign++;
        len--;
    }

    std::vector<int> d(size);
    for (int i=0;i<size;i++)
        d[size-len+i] = factor*(str[i+j]-'0');
    
    return d;
}

void even_out(std::vector<int> digits){
    
    for (int )
}

// Devuelve el signo que tendra el resultado de la suma o resta de los dos numeros ingresado
int final_sign(std::vector<int> a, std::vector<int> b){
    int size = a.size();
    int sign_a = a[size-1]/abs(a[size-1]);
    int sign_b = b[size-1]/abs(b[size-1]);
    
    int i = pass = 0;
    while (pass==0 && i<)
    for (int i)
}

// Suma o resta dos numeros enteros ingresados en forma de string
std::vector<int> sum_digits(std::string a, char op, std::string b){
    int lena = a.length();
    int lenb = b.length();
    int factor = 1, len;
    if (op=='-')
        factor = -1;
    if (lena>lenb)
        len = lena;
    else
        len = lenb;

    std::vector<int> digits_a = digits_char(a, len);
    std::vector<int> digits_b = digits_char(b, len);
    for (int i=0;i<len;i++)
        cout << digits_a[i] << " ";
    cout << endl;
    for (int i=0;i<len;i++)
        cout << digits_b[i] << " ";
    cout << endl;

    std::vector<int> res_digits(len);
    for (int i=0;i<len;i++)
        res_digits[i] += digits_a[i] + factor * digits_b[i];

    int res = 0;
    factor = 1;
    for (int i=len-1;i>=0;i--){
        res += factor * res_digits[i];
        factor *= 10;
    }

    return res;
}

int main(int argc, char *argv[]){
    if (argc<4){
        std::cerr << "Uso: ./" << argv[0] << "a [ + | - | * | / ] b" << endl;
        return -1;
    }

    int result, choice = (argv[2])[0];
    switch (choice) {
        case '+':
            result = sum_digits(argv[1], (argv[2])[0], argv[3]);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = " << result << endl;
            break;
        case '-':
            result = sum_digits(argv[1], (argv[2])[0], argv[3]);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = " << result << endl;
            break;
        default:
            std::cerr << "NO SE INGRESO UNA OPERACION VALIDA" << endl;
            return -1;
    }

    return 0;
}