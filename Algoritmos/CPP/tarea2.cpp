/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 2 C++
 * 
 * Realiza operaciones ingresadas como parametros en la
 * ejecucion del programa
************************************************************/

#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

#define MAX 100

// Devuelve 1 o 0 si la cadena ingresada es un numero entero o no
int valid_string(char *str){
    int i = 0, len = std::strlen(str), pass = 0;
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

// Imprime el arreglo de digitos
void print_digits(int digits[MAX], int len){
    int factor = 1;
    if (digits[MAX-1]<0){
        factor = -1;
        cout << "-";
    }
    
    bool left_zero = true;
    for (int i=MAX-len;i<MAX;i++){
        if (digits[i]!=0 || i==MAX-1)
            left_zero = false;
        if (!left_zero)
            cout << factor*digits[i];
    }
    cout << endl;
}

// Devuelve el numero en digits escrito en forma de string
void arr2str(int digits[MAX], char str[MAX+1], int len){
    int factor = 1, sign = 0;
    if (digits[MAX-1]<0){
        factor = -1;
        str[0] = '-';
        sign++;
    }
    for (int i=MAX-len;i<MAX;i++)
        str[i-MAX+len+sign] = factor*digits[i]+'0';
    str[len] = '\0';
}

/* 
Devuelve un arreglo de enteros donde el primer elemento es su signo 
y los restantes, sus digitos. Si no se define el segundo argumento, 
su tamano es la cantidad de digitos del numero ingresado, de lo contrario
se devuelve un arreglo con los digitos faltantes en ceros.
*/
void digits_char(char *str, int digits[MAX], int size = 0){
    if (valid_string(str)==0){
        std::cerr << "NUMERO INVALIDO" << endl;
        exit(1);
    }
    int factor = 1, sign = 0;
    int len = std::strlen(str);
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

    for (int i=0;i<MAX-len;i++)
        digits[i] = 0;
    for (int i=MAX-len;i<MAX;i++){
        digits[i] = factor*(str[i+sign-MAX+len]-'0');
    }
}

/*
Si hay casillas con numeros con valor absoluto mayor a 9, modifica
el arreglo para que todos sean digitos positivos o negativos.
*/
void even_out(int digits[MAX], int *size, int sign, int operation){
    for (int i=MAX-1;i>MAX-*size;i--){
        if (digits[i]!=0){
            if (operation*digits[i]/abs(digits[i])!=sign){
                digits[i] += sign*10;
                digits[i-1] -= sign;
            }
            if (abs(digits[i])>=10){
                digits[i] -= sign*10;
                digits[i-1] += sign;
            }
        }
    }
    if (abs(digits[MAX-*size])>=10){
        digits[MAX-*size] -= 10*sign;
        *size += 1;
        digits[MAX-*size] += sign;
    }
}

/*
 Devuelve el signo que tendra el resultado de la suma o resta de los dos numeros 
 ingresados como arreglos del mismo tamano
*/
int final_sign(int a[MAX], int b[MAX], int size){
    int sign_a = a[MAX-1]/abs(a[MAX-1]);
    int sign_b = b[MAX-1]/abs(b[MAX-1]);

    int i = MAX-size;
    int pass = 0;
    while (pass==0 && i<MAX){
        if (abs(a[i])>abs(b[i]))
            pass = sign_a;
        if (abs(b[i])>abs(a[i]))
            pass = sign_b;
        i++;
    }

    return pass;
}

// Suma o resta dos numeros enteros ingresados en forma de string
int sum_digits(char *a, char op, char *b, int result[MAX]){
    int lena = std::strlen(a);
    int lenb = std::strlen(b);
    if (a[0]=='+' || a[0]=='-')
        lena--;
    if (b[0]=='+' || b[0]=='-')
        lenb--;

    int factor = 1, len, sign_result = 1;
    if (op=='-')
        factor = -1;
    if (lena>lenb)
        len = lena;
    else
        len = lenb;
    
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, len);
    digits_char(b, digits_b, len);

    for (int i=MAX-len;i<MAX;i++)
        result[i] = digits_a[i] + factor * digits_b[i];
    
    // si los signos de los numeros son opuestos o se ejecuto una resta se calcula el signo del resultado
    int sign_a = (digits_a[MAX - lena] < 0) ? -1 : 1;
    int sign_b = (digits_b[MAX - lenb] < 0) ? -1 : 1;
    if ((sign_a != sign_b) || op=='-')
        sign_result = final_sign(digits_a,digits_b, len);
    
    even_out(result, &len, sign_result, factor);

    int res = 0;
    factor = 1;
    for (int i=MAX-1;i>=MAX-len;i--){
        res += factor * result[i];
        factor *= 10;
    }

    return len;
}

int product(char *a, char *b, int result[MAX]){
    int lena = std::strlen(a);
    int lenb = std::strlen(b);
    int len = lena + lenb;
    
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, len);
    digits_char(b, digits_b, len);
    int sign_a = (digits_a[MAX - lena] < 0) ? -1 : 1;
    int sign_b = (digits_b[MAX - lenb] < 0) ? -1 : 1;
    int sign_result = sign_a*sign_b;

    for (int bi=lenb-1;bi>=0;bi--){
        int carry = 0;
        for (int ai=lena-1;ai>=0;ai--){
            int prod = sign_a*digits_a[MAX-lena+ai]*sign_b*digits_b[MAX-lenb+bi] + result[MAX-len+ai+bi+1] + carry;
            result[MAX-len + ai + bi + 1] = prod%10;
            carry = prod/10;
        }
        result[MAX-len + bi + lena] += carry;
        result[MAX-len + bi + lena] *= sign_result;
    }

    int i = MAX-len;
    while (i<MAX && result[i]==0)
        i++;
    
    len = MAX-i;

    if (len==0)
        len++;

    return len;
}

int power(char *a, char *b, int result[MAX]){
    int lena = std::strlen(a);
    int lenb = std::strlen(b);
    int len = 1;
    
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, len);
    digits_char(b, digits_b, len);
    int pow[lenb];
    for (int i=0;i<MAX-lenb;i++){
        cout << "i " << i << endl;
        pow[i] = 0;
    }
    cout << "AQUI" << endl;
    for (int i=MAX-lenb;i<MAX;i++)
        pow[i] = digits_b[i];
    cout << "AQUI" << endl;
    int sign_a = (digits_a[MAX - lena] < 0) ? -1 : 1;
    int sign_b = (digits_b[MAX - lenb] < 0) ? -1 : 1;
    //int sign_result = sign_a*sign_b;

    int temp[MAX];
    temp[MAX-1] = 1;

    char temp_str[MAX+1], pow_str[MAX+1];
    arr2str(temp, temp_str, 1);
    arr2str(pow, pow_str, lenb);
    cout << "temp: " << temp_str << " a: " << a << endl;
    while (lena>0){
        char one[] = "1";

        int aux = (len>lena)? len: lena;
        for (int i=MAX-aux;i<MAX;i++){
            temp[i] = 0;
            pow[i] = 0;
        }

        len = product(temp_str,a,temp);
        cout << temp[MAX-1] << endl;
        arr2str(temp,temp_str,len);
        cout << "AQUI " << lena << endl;
        lena = sum_digits(pow_str, '-', one, pow);
        arr2str(pow,pow_str,lena);
    }

    return len;
}

int main(int argc, char *argv[]){
    if (argc<4){
        std::cerr << "Uso: ./" << argv[0] << "a [ + | - | * | / ] b" << endl;
        return -1;
    }

    int result[MAX], choice = (argv[2])[0], len;
    for (int i=0;i<MAX;i++)
        result[i] = 0;

    switch (choice) {
        case '+':
            len = sum_digits(argv[1], (argv[2])[0], argv[3], result);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = ";
            print_digits(result, len);
            break;
        case '-':
            len = sum_digits(argv[1], (argv[2])[0], argv[3], result);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = ";
            print_digits(result, len);
            break;
        case '*':
            len = product(argv[1], argv[3], result);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = ";
            print_digits(result, len);
            break;
        case '#':
            len = power(argv[1], argv[3], result);
            cout << argv[1] << " " << argv[2] << " " << argv[3] << " = ";
            print_digits(result, len);
            break;
        default:
            std::cerr << "NO SE INGRESO UNA OPERACION VALIDA" << endl;
            return -1;
    }

    return 0;
}