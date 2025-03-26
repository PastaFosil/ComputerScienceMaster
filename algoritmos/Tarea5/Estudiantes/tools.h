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

//===================================================================

// LECTURA Y ESCRITURA DE ARCHIVO ESTUDIANTES

/*Escribe el archivo de estudiantes*/
void write_students(char *filename, char **str, int n);

/*Lee el archivo de estudiantes*/
struct estudiante *read_students(char *filename, int *n);

//===================================================================

// OPERACIONES SOBRE ARREGLO DE ESTUDIANTES

/*Imprimir arreglo*/
void print_students(struct estudiante *students, int n);

/*Ordena a los estudiantes alfabeticamente*/
void order_by_name(struct estudiante *students, int n);

/*Ordena a los estudiantes segun su edad (de mayor a menor)*/
void order_by_age(struct estudiante *students, int n);

/*Ordena a los estudiantes segun su calificacion (de mayor a menor)*/
void order_by_note(struct estudiante *students, int n);

/*Cuenta a los estudiantes por grupo*/
void students_per_group(struct estudiante *students, int n);

/*Cuenta a los estudiantes por turno*/
void students_per_turn(struct estudiante *students, int n);

/*Dar de baja a un estudiante*/
void drop_out(char *filename, char *student_name, struct estudiante **students, int *n);

/*Dar de alta a un alumno*/
void sign_in(char *filename, char *student_data, struct estudiante **students, int *n);