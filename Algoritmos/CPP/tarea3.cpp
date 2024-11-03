/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 3 C++
 * 
 * Realiza operaciones ingresadas por el usuario
************************************************************/

#include <iostream>
#include <cstring>
#include <string>
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
            case '-':   
                sign++;
                break;
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
            default:
                return 0;

        }
        if (sign>1 && special==0)
            return 0;
        i++;
    }
    return 1;
}

void initialize_digits(int digits[MAX]){
    for (int i=0;i<MAX;i++)
        digits[i] = 0;
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
    int len = std::strlen(str);
    if (!valid_string(str) && len>1){
        std::cerr << "NUMERO INVALIDO" << endl;
        exit(1);
    }
    int factor = 1, sign = 0;
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
void even_out(int digits[MAX], int *size, int sign){
    for (int i=MAX-1;i>MAX-*size;i--){
        if (digits[i]!=0){
            if (digits[i]*sign<0){
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
int final_sign(int a[MAX], int b[MAX], int sign_a, int sign_b, int size){
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
    int sign_a = (a[0]=='-') ? -1 : 1;
    int sign_b = (b[0]=='-') ? -1 : 1;
    int factor = (op == '-') ? -1 : 1;
    int len = (lena > lenb) ? lena : lenb;
    
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, lena);
    digits_char(b, digits_b, lenb);

    for (int i=MAX-len;i<MAX;i++)
        result[i] = digits_a[i] + factor * digits_b[i];

    // si los signos de los numeros son opuestos o se ejecuto una resta se calcula el signo del resultado
    int sign_result = sign_a;
    if ((sign_a != sign_b) || op=='-')
        sign_result = final_sign(digits_a,digits_b,sign_a,sign_b, len);
    
    even_out(result, &len, sign_result);

    // actualiza la longitud real del arreglo de digitos
    int i = MAX-len;
    while (i<MAX && result[i]==0)
        i++;
    len = MAX-i;
    if (len==0)
        len++;

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
    int sign_a = (a[0]=='-') ? -1 : 1;
    int sign_b = (b[0]=='-') ? -1 : 1;
    int sign_result = sign_a*sign_b;

    for (int bi=lenb-1;bi>=0;bi--){
        int carry = 0;
        for (int ai=lena-1;ai>=0;ai--){
            int prod = abs(digits_a[MAX-lena+ai])*abs(digits_b[MAX-lenb+bi]) + result[MAX-len+ai+bi+1] + carry;
            result[MAX-len + ai + bi + 1] = prod%10;
            carry = prod/10;
        }
        result[MAX-len + bi] += carry;
    }
    for (int i=MAX-len;i<MAX;i++)
        result[i] *= sign_result;
    
    // actualiza la longitud real del arreglo de digitos
    int i = MAX-len;
    while (i<MAX && result[i]==0)
        i++;
    len = MAX-i;
    if (len==0)
        len++;

    if (len==0)
        len++;

    return len;
}

int power(char *a, char *b, int result[MAX]){
    int lena = std::strlen(a); // longitudes de arreglos de digitos
    int lenb = std::strlen(b);
    int len = 1; // longitud del arreglo del resultado
    
    if (b[0]=='0' && lenb==1){ // si el exponente es 0, el resultado es 1
        result[MAX-1] = 1;
        return len;
    }

    // arreglos de digitos de a y b
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, lena);
    digits_char(b, digits_b, lenb);

    // inicializacion arreglo de digitos de ciclos que faltan por calcular
    int pow[MAX];
    for (int i=0;i<MAX-lenb;i++)
        pow[i] = 0;
    // se llena el arreglo de ciclos con los digitos de b
    for (int i=MAX-lenb;i<MAX;i++)
        pow[i] = digits_b[i];
    // se calcula el signo de a y b
    int sign_a = (a[0]=='-') ? -1 : 1;
    int sign_b = (b[0]=='-') ? -1 : 1;
    // inicializacion de arreglo de digitos de resultado
    result[MAX-1] = 1;
    // se llenan strings de resultado y de potencia para los calculos
    char result_str[MAX+1], pow_str[MAX+1];
    arr2str(result, result_str, 1);
    arr2str(pow,pow_str,lenb);
    
    char one[] = "1";
    while (pow_str[0]!='0'){ // se repite hasta que se hayan calculado todos los ciclos
        int aux = (len>lena)? len: lena;
        for (int i=MAX-aux;i<MAX;i++){
            result[i] = 0;
            pow[i] = 0;
        }
        
        len = product(result_str,a,result);
        arr2str(result,result_str,len);
        lenb = sum_digits(pow_str, '-', one, pow);
        arr2str(pow,pow_str,lenb);
    }

    return len;
}

int division(char *a, char *b, int result[MAX]){
    int lena = std::strlen(a); // longitudes de arreglos de digitos
    int lenb = std::strlen(b);
    int len = 1; // longitud del arreglo del resultado

    // arreglos de digitos de a y b
    int digits_a[MAX], digits_b[MAX];
    digits_char(a, digits_a, lena);
    digits_char(b, digits_b, lenb);

    // se calcula el signo de a y b
    int sign_a = (a[0]=='-') ? -1 : 1;
    int sign_b = (b[0]=='-') ? -1 : 1;

    char div[MAX+1];
    int j = (sign_b<0)? 1: 0;
    for (int i=0;i<MAX-lenb;i++)
        div[i] = b[i+j];
    // inicializacion arreglo de digitos de residuo
    int remainder[MAX];
    for (int i=0;i<MAX-lena;i++)
        remainder[i] = 0;
    // se llena el arreglo de ciclos con los digitos de a
    for (int i=MAX-lena;i<MAX;i++)
        remainder[i] = sign_a*digits_a[i];

    // se llenan strings de resultado y de potencia para los calculos
    char result_str[MAX+1], remainder_str[MAX+1];
    result_str[0] = '0';
    result_str[1] = '\0';
    arr2str(remainder,remainder_str,lena);
    char one[] = "1";
    while (remainder_str[0]!='0'){ // se repite hasta que se hayan calculado todos los ciclos
        for (int i=MAX-lena;i<MAX;i++)
            remainder[i] = 0;
        
        lena = sum_digits(remainder_str, '-', div, remainder);
        arr2str(remainder,remainder_str,lena);
        if (remainder_str[0]<'0')
            break;

        for (int i=MAX-len;i<MAX;i++)
            result[i] = 0;
        len = sum_digits(result_str, '+', one, result);
        arr2str(result,result_str,len);
    }

    for (int i=MAX-len;i<MAX;i++)
        result[i] *= sign_a*sign_b;
    return len;
}

void operate(char *a, char op, char *b){
    int result[MAX], len;
    for (int i=0;i<MAX;i++)
        result[i] = 0;

    switch (op) {
        case '+':
            len = sum_digits(a, op, b, result);
            break;
        case '-':
            len = sum_digits(a, op, b, result);
            break;
        case '*':
            len = product(a, b, result);
            break;
        case '#':
            len = power(a, b, result);
            break;
        case '/':
            len = division(a, b, result);
            break;
        default:
            std::cerr << "NO SE INGRESO UNA OPERACION VALIDA" << endl;
            exit(1);
    }

    cout << a << " " << op << " " << b << " = ";
    print_digits(result, len);
}

int main(){
    char input[MAX], a[MAX], b[MAX], op;
    bool quit = 1;

    while (quit){
        int i = 0;
        int j = 0;

        cout << "Ingrese la operacion a realizar (q para salir):\n";
        cout << "Operaciones validas: +, -, *, /, #\n";
        cout << "R: ";
        std::cin.getline(input, MAX);
        if (input[0]=='q')
            break;
        
        while (input[j]!=' ' && input[j]!='\0')
            a[i++] = input[j++];
        a[i] = '\0';
        i = 0; 
        while (input[j]==' ')
            j++;

        op = input[j];
        while (input[j++]==' ');

        i = 0; j++;
        while (input[j]!=' ' && input[j]!='\0')
            b[i++] = input[j++];
        b[i] = '\0';
        
        cout << a << op << b << endl;
        if (valid_string(a) && valid_string(b))
            operate(a, op, b);
        else
            std::cerr << "NUMERO INVALIDO" << endl;

        cout << "\n\n";
    }

    return 0;
}