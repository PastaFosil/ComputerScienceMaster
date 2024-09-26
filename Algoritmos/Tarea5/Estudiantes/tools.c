/*****************************************
        Programacion y algoritmos
        Tarea 5
        2024/09/24
        Juan Carlos Perez Ramirez
        Funciones de archivo estudiantes
*****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


struct escuela {
    char grupo;
    char turno[12];
};

struct estudiante {
    char nombre[30];
    float calif_promedio;
    int edad;
    struct escuela *E;
};

/*Escribe el archivo de estudiantes*/
void write_students(char *filename, char **str, int n){
    FILE *file = fopen(filename,"w");
    if (file == NULL){
        printf("No se pudo abrir el archivo para escritura.\n");
        return;
    }

    fprintf(file, "%d\n",n);
    for (int i=0;i<n;i++){
        fprintf(file, "%s\n",str[i]);
    }

    fclose(file);
}

/*Lee el archivo de estudiantes*/
struct estudiante *read_students(char *filename, int *n){
    errno = 0;

    // abre archivo de estudiantes
    FILE *file = fopen(filename,"r");
    if(file == NULL){
        printf("READ. ERROR ARBIENDO ARCHIVO: %s\n", strerror(errno));
        return NULL;
    }

    // lee el numero de estudiantes
    if(fscanf(file, "%d", n)!=1){
        printf("READ. ERROR LEYENDO DATOS: %s\n", strerror(errno));
        fclose(file);
        return NULL;
    }
    // asigna memoria para el arreglo de datos
    struct estudiante *students = (struct estudiante *)malloc(*n*sizeof(struct estudiante));
    if (students==NULL){
        printf("READ. ERROR ASIGNANDO MEMORIA: %s\n", strerror(errno));
        fclose(file);
        return NULL;
    }

    // lee la informacion del archivo y la guarda en el arreglo
    for (int i=0;i<*n;i++){
        // se signa espacio de memoria para la estructura escuela del alumno actual
        students[i].E = (struct escuela *)malloc(sizeof(struct escuela));
        if (students[i].E==NULL){
            printf("READ. ERROR ASIGNANDO MEMORIA: %s\n", strerror(errno));
            for (int j=0;j<=i;j++)
                free(students[j].E);
            free(students);
            fclose(file);
            return NULL;
        }
        // guarda la informacion del alumno actual
        if (fscanf(file, "%30s %f %d %c %12s", students[i].nombre, &students[i].calif_promedio, &students[i].edad, &students[i].E->grupo, students[i].E->turno)!=5){
            printf("READ. ERROR LEYENDO DATOS DE %d: %s\n", i, strerror(errno));
            for (int j=0;j<=i;j++)
                free(students[j].E);
            free(students);
            fclose(file);

            return NULL;
        }
    }

    fclose(file);

    return students;
}

/*Imprime la informacion de los estudiantes*/
void print_students(struct estudiante *students, int n){
    printf("====================================\n\n");

    printf("LISTA DE ESTUDIANTES\n\n");
    
    printf("Nombre\tPromedio\tEdad\tGrupo\tTurno\n\n");
    for (int i=0;i<n;i++){
        printf("%s\t",students[i].nombre);
        printf("%g\t\t",students[i].calif_promedio);
        printf("%d\t",students[i].edad);
        printf("%c\t",students[i].E->grupo);
        printf("%s\n",students[i].E->turno);
    }
}

/*Ordena a los estudiantes alfabeticamente*/
void order_by_name(struct estudiante *students, int n){
    errno = 0;

    struct estudiante aux; // ayudara en el intercambio de elementos

    // ordena a los alumnos alfabeticamente
    int order = 0;
    while (order==0){
        order = 1;
        for (int i=1;i<n;i++){
            if (strcmp(students[i-1].nombre,students[i].nombre)>0){
                order = 0;
                aux = students[i-1];
                students[i-1] = students[i];
                students[i] = aux;
            }
        }
    }
}

/*Ordena a los estudiantes segun su edad (de mayor a menor)*/
void order_by_age(struct estudiante *students, int n){
    errno = 0;

    struct estudiante aux; // ayudara en el intercambio de elementos

    // ordena a los alumnos basado en la edades (de mayor a menor)
    int order = 0;
    while (order==0){
        order = 1;
        for (int i=1;i<n;i++){
            if (students[i-1].edad<students[i].edad){
                order = 0;
                aux = students[i-1];
                students[i-1] = students[i];
                students[i] = aux;
            }
        }
    }
}

/*Ordena a los estudiantes segun su calificacion (de mayor a menor)*/
void order_by_note(struct estudiante *students, int n){
    errno = 0;
    
    struct estudiante aux; // ayudara en el intercambio de elementos

    // ordena a los alumnos basado en las calificaciones (de mayor a menor)
    int order = 0;
    while (order==0){
        order = 1;
        for (int i=1;i<n;i++){
            if (students[i-1].calif_promedio<students[i].calif_promedio){
                order = 0;
                aux = students[i-1];
                students[i-1] = students[i];
                students[i] = aux;
            }
        }
    }
}

/*Cuenta a los estudiantes por grupo*/
void students_per_group(struct estudiante *students, int n){
    // guarda el numero de estudiantes por grupo
    int number[6] = {0,0,0,0,0,0};

    // cuenta a los estudiantes en cada grupo
    for (int i=0;i<n;i++){
        if (students[i].E->grupo=='A')
            number[0]++;
        else if (students[i].E->grupo=='B')
            number[1]++;
        else if (students[i].E->grupo=='C')
            number[2]++;
        else if (students[i].E->grupo=='D')
            number[3]++;
        else if (students[i].E->grupo=='E')
            number[4]++;
        else if (students[i].E->grupo=='F')
            number[5]++;
    }

    printf("\n\n====================================\n\n");

    printf("ESTUDIANTES POR GRUPO\n\n");

    printf("Grupo\tCantidad\n\n");
    for (int i=0;i<6;i++)
        printf("%c\t%d\n",i+'A',number[i]);
}

/*Cuenta a los estudiantes por turno*/
void students_per_turn(struct estudiante *students, int n){
    // guarda el numero de estudiantes por turno
    int number[2] = {0,0};
    for (int i=0;i<n;i++){
        if (strcmp(students[i].E->turno,"Matutino")==0)
            number[0]++;
        else if (strcmp(students[i].E->turno,"Vespertino")==0)
            number[1]++;
        else {

        }
    }

    printf("\n\n====================================\n\n");

    printf("ESTUDIANTES POR TURNO\n\n");

    printf("Turno\t\tCantidad\n\n");
    printf("%s\t%d\n","Matutino",number[0]);
    printf("%s\t%d\n","Vespertino",number[1]);
}

/*Dar de baja a un estudiante*/
void drop_out(char *student_name, struct estudiante **students, int *n){
    errno = 0;

    // escribe datos de estudiantes que se mantienen en archivo
    int found = 0;
    for (int i=0;i<*n;i++){
        // encuentra el estudiante a dar de baja
        printf("'%s'='%s'\n",student_name,(*students)[i].nombre);
        if (strcmp((*students)[i].nombre,student_name)==0){
            found = 1;
            free((*students)[i].E);
        }
        printf("found=%d\n",found);
        // reasigna valores para llenar el espacio del estudiante dado de baja para poder liberar el ultimo elemento
        if (found==1&&i<*n-1){
            printf("REASIGNANDO VALORES\n");
            (*students)[i] = (*students)[i+1];
        }
    }

    *n -= 1;
    
    *students = (struct estudiante *)realloc(*students, *n*sizeof(struct estudiante));
    if (*students==NULL){
        printf("DROP. NO SE PUDO ASIGNAR MEMORIA\n");
        return;
    }

    return;
}

/*Dar de alta a un alumno*/
void sign_in(char *student_data, struct estudiante **students, int *n){
    errno = 0;

    *students = (struct estudiante *)realloc(*students, (*n+1)*sizeof(struct estudiante));
    (*students)[*n].E = (struct escuela *)malloc(sizeof(struct escuela));
    if (*students==NULL||(*students)[*n].E==NULL){
        printf("DROP. NO SE PUDO ASIGNAR MEMORIA: %s\n", strerror(errno));
        return;
    }

    if (sscanf(student_data, "%s %f %d %c %s", (*students)[*n].nombre, &(*students)[*n].calif_promedio, &(*students)[*n].edad, &(*students)[*n].E->grupo, (*students)[*n].E->turno)!=5){
        printf("SIGN. ERROR LEYENDO DATOS: %s\n", strerror(errno));
        free((*students)[*n].E);
        return;
    }

    *n += 1;
}

void clear_buffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void free_students(struct estudiante *students, int n){
    for (int i=0;i<n;i++)
        free(students[i].E);
    free(students);
}