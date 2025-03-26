/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 4

 * En el codigo se aplican diferentes operaciones sobre 
 * cadenas
************************************************************/

#include <stdio.h>
#include <stdlib.h>

int repeated_letters(char *str){
    /*********************************************
        Devuelve 1 si hay letras repetidas en 
        una palabra, y 0 si no
    *********************************************/
    
    int letters[26]; // arreglo que guarda las ocurrencias de cada letra
    for (int j=0;j<26;j++) // inicializacion en ceros
        letters[j] = 0;

    int i = 0;
    while (str[i]!='\0'){ // se recorre el string hasta llegar a '\0'
        if (str[i]>='A'&&str[i]<='Z') // si el caracter actual es una letra, se registra su ocurrencia
            letters[str[i]+32-'a'] += 1;
        else if (str[i]>='a'&&str[i]<='z')
            letters[str[i]-'a'] += 1;
        i++;
    }

    // si alguna letra tiene mas de una ocurrencia, se devuelve 1
    for (int j=0;j<26;j++)
        if (letters[j]>1)
            return 1;
    // si no hay multiples ocurrencias de ninguna letra, devuelve 0
    return 0;
}

void *no_rep(char *in_str, char *out_str){
    char str1[20];
    char str2[100];
    char *out_ptr=out_str;

    int k = 0;
    while (in_str[k]!='\0'){ // recorre el string hasta llegar a '\0'
        int i = 0;
        while (in_str[k]!=' '&&in_str[k]!='\0'){ // guarda los caracteres en un string hasta encontrar un espacio
            str1[i] = in_str[k];
            k++;
            i++;
        }
        str1[i] = '\0'; // agrega un '\0' al final de la palabra
        if (repeated_letters(str1)==0){ // si no hay letras repetidas en la palabra, se guarda str1 en str2
            for (int j=0;j<i;j++){
                *out_ptr = str1[j];
                out_ptr++;
            }
            *out_ptr = ' ';
            out_ptr++;
        }
        if (in_str[k]==' ')
            k++;
    }
    *out_ptr = '\0'; // se agrega el '\0' en la ultima direccion a donde apunta out_ptr
}

int compara_str(char *str1, char *str2){
    int i = 0;
    while (str1[i]!='\0'&&str2[i]!='\0'){
        if (str1[i]-str2[i]==0)
            i++;
        else  
            break;
    }
    return str1[i]-str2[i];
}

int main(){
    int max_char = 100;
    //char str[max_char], str2[100];
    //printf("Ingrese un string:\n");
    //fgets(str,max_char,stdin);

    /*
    no_rep(str, str2);

    if (str2[0]=='\0'){
        printf("NO HAY PALABRAS SIN LETRAS REPETIDAS\n");
    } else {
        printf("\n\nPalabras sin letras repetidas:\n");
        puts(str2);
    }
    */
    /*
    char str1[max_char],str2[max_char];
    printf("Ingrese primer string: ");
    fgets(str1,max_char,stdin);
    printf("Ingrese segundo string: ");
    fgets(str2,max_char,stdin);

    printf("%d\n",compara_str(str1,str2));
    */
    return 0;
}