/* 
 * File:   main.c
 * Author: paco
 *
 * Created on 1 de junio de 2012, 19:09
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <strings.h>
#include <CL/cl.h>
#include "linealShellSort.h"
#include "paralelShellSort.h"

/*
 * 
 */


#define SIZE    400000
#define MAX_NUM 640000
#define REPETICIONES 10
// maximo 512
#define MAX_DIM_X 512
#define MAX_GLOBAL_DIM_X 50000
// no hay minimo
#define MIN_DATA_X_THREAD 5
#define IMPL 1
#define IMP0 0
#define IMP1 0
#define IMP2 1
#define IMP3 0
#define IMP4 1
#define IMP5 0
#define IMP6 0

#define PRINT 0

#define SELECTED_PLATFORM_VENDOR "NVIDIA corporation"
#define SELECTED_PLATFORM_NAME "NVIDIA CUDA"
#define SELECTED_DEVICE_NAME "NVS 3100M"



void printVector(long* vector, long size) {
    printf("(%ld", vector[0]);
    for (int i = 1; i < size; i++) {
        printf(",%ld", vector[i]);
    }
    printf(")\n");
}

int checkVector(long* vector, long pasada, long size) {
    for (long i = 1; i < size; i++) {
        if (vector[i - 1] > vector[i]) {
            if (PRINT) {
                printf("pasada%ld, pos=%ld", pasada, i);
                printVector(vector + i - 1, 4);
            }

            return 1;
        } else {


        }
    }
    return 0;
}

int checkVector2(long *vectorA, long *vectorB, long size) {
    int printed = 0;
    for (int i = 0; i < size; i++) {
        int finded = 0;
        int j = 0;
        for (int j = 0; j < size && !finded; j++) {
            if (vectorA[i] == vectorB[j]) {
                finded = 1;
            }
        }
        if (!finded) {
            if (PRINT)
                printf("Primer no encontrado=%ld[%d]\n", vectorA[i], i);
            return 1;
        }
    }
    return 0;

}

void checkVector3(long *vectorA, long size) {
    int printed = 0;
    for (long i = 1; i < size; i++) {
        if (vectorA[i - 1] == vectorA[i]) {
            if (!printed) {
                printed = 1;
                printf("Repetidos=");

            } else {
                printf(",");
            }
            printf("%ld[%ld-%ld]", vectorA[i], i, i + 1);

        }
    }
    if (printed) {
        printf("\n");
    }
}

void printVectorDobleInvertido(double* vector, long size) {
    printf("(%lf", vector[size - 1]);
    for (int i = size - 1; i >= 0; i--) {
        printf(",%lf", vector[i]);
    }
    printf(")\n");
}

int main(int argc, char** argv) {
    long vectorA[SIZE];
    //long vectorB[SIZE];
    clock_t inicio, final;
    time_t t1, t2, t3;
    int num_err = 0;
    size_t datos_x_thread;
    double tiempoSinCopia = 0;
    int pasadas = listaIncrementosSedgewick(SIZE);
    double tiempos[pasadas];
    int incorrects, incorrects2;

    srand(time(NULL));

    double total = 0;
    printf("Elementos=%d, Repeticiones=%d, Maximo de hilos=%d, Minimo de datos=%d\n", SIZE, REPETICIONES,MAX_DIM_X,MIN_DATA_X_THREAD);


    // PARALELO 4
    if (IMP4) {
        parallel_context context4;
        //printf("ALGORITMO PARALELO D\n");
        inicializeContext(&context4, "cl/shellSortD.cl", "shellSortD",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        if (context4.result != 0)
            printf("inicializeContext=%d\n", context4.result);

        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }
        total = 0;
        tiempoSinCopia = 0;

        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        datos_x_thread = (SIZE / MAX_DIM_X) + 1;
        //size_t threads = (SIZE/16);


        if (datos_x_thread < MIN_DATA_X_THREAD)
            datos_x_thread = MIN_DATA_X_THREAD;
        incorrects = 0;
        incorrects2 = 0;

        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                //do {
                vectorA[i] = rand() % MAX_NUM;
                //} while (vectorA[i] < 1000);
                //vectorB[i] = vectorA[i];

            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }

            inicio = clock();
            tiempoSinCopia += paralelShellSort_D(&context4, vectorA, SIZE, imprimir, tiempos, datos_x_thread);
            if (context4.result != 0)
                printf("paralelShellSort_D=%i\n", context4.result);
            final = clock();
            //printVector(vectorA,SIZE);
            incorrects += checkVector(vectorA, j, SIZE);
            //incorrects2 += checkVector2(vectorB, vectorA, SIZE);
            //checkVector3(vectorA, SIZE);
            total += difftime(final, inicio);
        }

        //printf("Media de algo!!!=%lf\n",tiempoSinCopia/REPETICIONES);
        tiempoSinCopia /= CLOCKS_PER_SEC;
        total /= CLOCKS_PER_SEC;
        //printf("Ordenaciones incorrectas por no ordenado=%d\n", incorrects);
        //printf("Ordenaciones incorrectas por elementos elimniados=%d\n", incorrects2);
        //printf("Numero de datos por thread=%lu\n", datos_x_thread);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("Tiempo (D)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
    }
    // fin paralelo 4


    if (IMPL) {
        //printf("ALGORITMO LINEAL\n");

        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            inicio = clock();
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
            }
            linealShellShort(vectorA, SIZE, tiempos);
            final = clock();
            total += difftime(final, inicio);
        }
        total /= CLOCKS_PER_SEC;
        printf("Tiempo (l)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        // FIN LINEAL

    }

    if (IMP0) {
        parallel_context context0;
        //printf("ALGORITMO PARALELO 0\n");
        total = 0;
        tiempoSinCopia = 0;
        pasadas = listaIncrementosSedgewick(SIZE);
        tiempos[pasadas];
        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }


        inicializeContext(&context0, "cl/linealShellSort.cl", "linealShellSort",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        if (context0.result != 0)
            printf("paralelShellSort_B=%d\n", context0.result);
        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }

        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }
            inicio = clock();
            tiempoSinCopia += paralelShellSort_0(&context0, vectorA, SIZE, imprimir, tiempos);
            final = clock();
            total += difftime(final, inicio);
        }

        total /= CLOCKS_PER_SEC;
        tiempoSinCopia /= CLOCKS_PER_SEC;
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("Tiempo (0)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
    }




    if (IMP6) {
        parallel_context context6;
        //printf("ALGORITMO PARALELO F\n");
        inicializeContext(&context6, "cl/shellSortF.cl", "shellSortF",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        if (context6.result != 0)
            printf("inicializeContext=%d\n", context6.result);

        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }
        total = 0;
        tiempoSinCopia = 0;

        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        datos_x_thread = (SIZE / MAX_GLOBAL_DIM_X) + 1;
        //size_t threads = (SIZE/16);


        if (datos_x_thread < MIN_DATA_X_THREAD)
            datos_x_thread = MIN_DATA_X_THREAD;
        incorrects = 0;
        incorrects2 = 0;
        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
                //vectorB[i] = vectorA[i];
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }

            inicio = clock();
            tiempoSinCopia += paralelShellSort_F(&context6, vectorA, SIZE, imprimir, tiempos, datos_x_thread);
            if (context6.result != 0)
                printf("paralelShellSort_E=%i\n", context6.result);
            final = clock();
            //printVector(vectorA,SIZE);
            incorrects += checkVector(vectorA, j, SIZE);
            //incorrects2 += checkVector2(vectorA, vectorB, SIZE);
            //checkVector3(vectorA, SIZE);
            total += difftime(final, inicio);
        }
        tiempoSinCopia /= CLOCKS_PER_SEC;
        total /= CLOCKS_PER_SEC;
        //printf("Ordenaciones incorrectas por no ordenado=%d\n", incorrects);
        //printf("Ordenaciones incorrectas por elementos elimniados=%d\n", incorrects2);
        //printf("Numero de datos por thread=%i\n", datos_x_thread);
        printf("Tiempo (F)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        // fin paralelo 6
    }
    if (IMP5) {
        // PARALELO 5
        parallel_context context5;
        //printf("ALGORITMO PARALELO E\n");
        inicializeContext(&context5, "cl/shellSortE.cl", "shellSortE",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        if (context5.result != 0)
            printf("inicializeContext=%d\n", context5.result);

        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }
        total = 0;
        tiempoSinCopia = 0;

        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        datos_x_thread = (SIZE / MAX_DIM_X) + 1;
        //size_t threads = (SIZE/16);


        if (datos_x_thread < MIN_DATA_X_THREAD)
            datos_x_thread = MIN_DATA_X_THREAD;
        incorrects = 0;
        incorrects2 = 0;
        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
                //vectorB[i] = vectorA[i];
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }

            inicio = clock();
            tiempoSinCopia += paralelShellSort_E(&context5, vectorA, SIZE, imprimir, tiempos, datos_x_thread);
            if (context5.result != 0)
                printf("paralelShellSort_E=%i\n", context5.result);
            final = clock();
            //printVector(vectorA,SIZE);
            incorrects += checkVector(vectorA, j, SIZE);
            //incorrects2 += checkVector2(vectorA, vectorB, SIZE);
            //checkVector3(vectorA, SIZE);
            total += difftime(final, inicio);
        }
        tiempoSinCopia /= CLOCKS_PER_SEC;
        total /= CLOCKS_PER_SEC;
        //printf("Ordenaciones incorrectas por no ordenado=%d\n", incorrects);
        //printf("Ordenaciones incorrectas por elementos elimniados=%d\n", incorrects2);
        //printf("Numero de datos por thread=%i\n", datos_x_thread);
        printf("Tiempo (E)=%f, ", total);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        // fin paralelo 5
    }
    if (IMP1) {
        parallel_context context;


        //printf("ALGORITMO PARALELO A\n");
        total = 0;
        tiempoSinCopia = 0;
        pasadas = listaIncrementosSedgewick(SIZE);
        tiempos[pasadas];
        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        inicializeContext(&context, "cl/shellSortCL.cl", "shellSortCL",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }

        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }
            inicio = clock();
            tiempoSinCopia += paralelShellSort_A(&context, vectorA, SIZE, imprimir, tiempos);
            final = clock();
            total += difftime(final, inicio);
        }
        total /= CLOCKS_PER_SEC;
        tiempoSinCopia /= CLOCKS_PER_SEC;
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("Tiempo (A)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        cleanContext(&context);
    }

    // fin paralelo1

    if (IMP2) {
        parallel_context context2;
        //printf("ALGORITMO PARALELO B\n");
        inicializeContext(&context2, "cl/shellSortCLB.cl", "shellSortCLB",SELECTED_PLATFORM_NAME, SELECTED_DEVICE_NAME);
        if (context2.result != 0)
            printf("inicializeContext=%d\n", context2.result);

        for (long i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % MAX_NUM;
        }
        total = 0;
        tiempoSinCopia = 0;

        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (long i = 0; i < SIZE; i++) {
                vectorA[i] = rand() % MAX_NUM;
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = PRINT;
            }
            inicio = clock();
            tiempoSinCopia += paralelShellSort_B(&context2, vectorA, SIZE, imprimir, tiempos, datos_x_thread);
            if (context2.result != 0)
                printf("paralelShellSort_B=%d\n", context2.result);
            final = clock();

            total += difftime(final, inicio);
        }
        tiempoSinCopia /= CLOCKS_PER_SEC;
        total /= CLOCKS_PER_SEC;
        //printf("Numero de datos por thread=%lu\n", datos_x_thread);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("Tiempo (B)=%f, ", total);
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        //printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
    }
    // fin paralelo 2













    /*
    
        parallel_context context3;
        total = 0;
        tiempoSinCopia = 0;
        pasadas = listaIncrementosSedgewick(SIZE);
        tiempos[pasadas];
        for (int i = 0; i < pasadas; i++) {
            tiempos[i] = 0;
        }
        inicializeContext(&context3, "cl/shellSortCLC.cl", "shellSortCLC");
        if (context3.result != CL_SUCCESS)
            printf("Gola=%d\n",context3.result);
        for (int i = 0; i < SIZE; i++) {
            vectorC[i] = rand() % MAX_NUM;
        }

        for (int j = 0; j < REPETICIONES; j++) {
            // inicializando vector 
            for (int i = 0; i < SIZE; i++) {
                vectorC[i] = rand() % MAX_NUM;
            }
            int imprimir = 0;
            if (j == (REPETICIONES - 1)) {
                imprimir = 0;
            }
            inicio = clock();
            tiempoSinCopia += paralelShellSort_C(&context3, vectorC, SIZE, imprimir, tiempos);
            final = clock();
            total += difftime(final, inicio);
        }
        printf("Fases=");
        printVectorDobleInvertido(tiempos, pasadas);
        total /= CLOCKS_PER_SEC;
        tiempoSinCopia /= CLOCKS_PER_SEC;
        printf("Tiempo de ejecucion paralela sin contar la copia de memoria=%f\n", tiempoSinCopia);
        printf("Tiempo de ejecucion paralela Total=%f\n", total);
        cleanContext(&context3);
     */




    return (EXIT_SUCCESS);
}


