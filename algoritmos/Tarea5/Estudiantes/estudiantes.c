/**********************************************************
 * JUAN CARLOS PEREZ RAMIREZ
 * PROGRAMACION Y ALGORITMOS
 * TAREA 5
 * 
 * Se opera sobre un archivo txt con los datos de 
 * estudiantes
************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tools.c"

int main(int argc, char **argv){
    if (argc<2){
        printf("Uso: ./%s alumnos.txt\n",argv[0]);
        return -1;
    }

    errno = 0;

    int n, option;
    char str[50];
    struct estudiante *students = read_students(argv[1], &n);;
    if (students==NULL){
        printf("NO SE PUDO ASIGNAR MEMORIA\n");
        return -1;
    }

    print_students(students,n);

    while (option!=9){
        printf("\n\n====================================\n\n");
        printf("Ingrese la operacion deseada: \n");
        printf("1) Imprimir archivo\n2) Ordenar por nombre\n3) Ordenar por Edad\n4) Ordenar por Promedio\n5) Número de estudiantes por grupo\n6) Número de estudiantes por turno\n7) Baja estudiante\n8) Alta estudiante\n9) Fin -> Imprimir arreglo final en archivo\n\n");
        printf("R: ");
        scanf("%d", &option);

        switch (option){
            case 1:
                // imprimir archivo
                print_students(students,n);
                break;
            case 2:
                // ordenar por nombre
                order_by_name(students,n);
                print_students(students,n);
                break;
            case 3:
                // ordenar por edad
                order_by_age(students,n);
                print_students(students,n);
                break;
            case 4:
                // ordenar por calificacion
                order_by_note(students,n);
                print_students(students,n);
                break;
            case 5:
                // estudiantes por grupo
                students_per_group(students, n);
                break;
            case 6:
                // estudiantes por turno
                students_per_turn(students,n);
                break;
            case 7:
                // baja de estudiante
                printf("\n\nIngresa el nombre del estudiante a dar de baja: ");
                clear_buffer();
                if (fgets(str,50,stdin)!=NULL){
                    size_t len = strlen(str);
                    if (len>0&&str[len-1]=='\n')
                        str[len-1] = '\0';
                    drop_out(str,&students,&n);
                    print_students(students,n);    
                } else {
                    printf("MAIN. ERROR LEYENDO DATOS: %s\n", strerror(errno));
                    return -1;
                }
                
                break;
            case 8:
                printf("\n\nIngresa los datos del estudiante a dar de alta\nFormato: Nombre promedio edad grupo turno\n");
                printf("R: ");
                clear_buffer();
                fgets(str,50,stdin);
                sign_in(str,&students,&n);
                print_students(students,n);
                break;
            case 9:
                clear_buffer();
                break;
            default:
                printf("Ingrese una opcion valida.\n\n");
        }
    }

    printf("====================================\n\n");
    printf("           ARCHIVO FINAL\n\n");
    print_students(students,n);
    
    printf("\n\nArchivo txt para guardar informacion final:\n");
    //scanf("%s",str);
    //FILE *file = fopen(str,"w");
    FILE *file = fopen("out_test.txt","w");
    if (file==NULL){
        printf("No se pudo abrir el archivo para escritura: %s\n", strerror(errno));
        return -1;
    }

    if (fprintf(file, "%d\n",n)==-1){
        printf("ERROR ESCRIBIENDO EN ARCHIVO: %s\n", strerror(errno));
            fclose(file);
            return -1;
    }
    for (int i=0;i<n;i++){
        if (fprintf(file, "%s %f %d %c %s\n", students[i].nombre, students[i].calif_promedio, students[i].edad, students[i].E->grupo,students[i].E->turno)==-1){
            printf("ERROR ESCRIBIENDO EN ARCHIVO: %s\n", strerror(errno));
            fclose(file);
            return -1;
        }
    }

    fclose(file);

    free_students(students, n);

    return 0;
}