#include "linealShellSort.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

long sedgewick_a(long i);
long sedgewick_b(long i);


void linealInsertShort(long *vector, long offset, long size, long incrementos);

long precalculated[] = {1, //0
    5, //1
    19, //2
    41, //3
    109, //4
    209, //5
    505, //6
    929, //7
    2161, //8
    3905, //9
    8929, //10
    16001, //11
    36289, //12
    64769, //13
    146305, //14
    260609, //15
    587521, //16
    1045505, //17
    2354689, //18
    4188161, //19
    9427969, //20
    16764929 //21
};

/**
 * Calcula el incremento de Sedgedwick para una cantidad de elementos.
 * La funcion no realiza la operación para menos de 16M (aprox) de elementos 
 */
long incrementosSedgewick(long max) {

    long i = 0;
    long incrementoAntorior = 0;
    printf("Calculando incremento de %ld\n", max);
    long precalculatedElements = 22;
    for (long i = 1; i < precalculatedElements; i++) {
        if (max < precalculated[i])
            return precalculated[i - 1];
    }
    while (1) {
        long incremento = sedgewick_a(i);
        printf("Incremento %ld?\n", incremento);

        if (max < incremento)
            return incrementoAntorior;
        incrementoAntorior = incremento;
        incremento = sedgewick_b(i);
        printf("Incremento %ld?\n\n", incremento);
        if (max < incremento)
            return incrementoAntorior;
        i++;
    }
    //9*4^i-9*2^i+1
    //2^(i+2)-(2^(i+2)-3)+1
    
    return 0;
}

long calcularIncrementoSedgewick(int pos){
    
    if (pos %2 == 0) {
        return sedgewick_a(pos/2);
    }
    return sedgewick_b(pos/2);
    
}

long incrementoN(int n){
    return precalculated[n];
}

long listaIncrementosSedgewick(long max) {
    long precalculatedElements = 22;

    for (long i = 0; i < precalculatedElements; i++) {
        if (max < precalculated[i])
            return i;
    }
    return 0;
}

long posicionIncrementoSedgewick(long max){
    long incrementoAntorior = 0;
    int i = 0;
    while (1) {
        long incremento = sedgewick_a(i);

        if (max < incremento) {
            return i*2;
        }
        incrementoAntorior = incremento;
        incremento = sedgewick_b(i);
        if (max < incremento){
            return i*2+1;
        }
        incrementoAntorior = incremento;
        i++;
    }
}


long sedgewick_a(long i) {
    //9*4^i-9*2^i+1
    long factorA = (long) 9 * pow(4, i);
    long factorB = (long) 9 * pow(2, i);
    return factorA - factorB + 1;
}
//2^(i+2)-(2^(i+2)-3)+1

long sedgewick_b(long i) {
    long factorA = (long) pow(2, (i + 2));
    long factorB = (long) pow(2, (i + 2)) - 3;
    return factorA * factorB + 1;
}

long linealShellShort(long* vector, long size, double *tiempos) {
    clock_t i1, f1;
    //long pasadas = listaIncrementosSedgewick(size);
    long pasadas = posicionIncrementoSedgewick(size);
    
    //printf("Tamaño=%d, Pasadas=%d\n", size, pasadas);
    for (int i = pasadas - 1; i >= 0; i--) {
        i1 = clock();
        //long incremento = precalculated[i];
        long incremento = calcularIncrementoSedgewick(i);
        //printf("Incrementos=%ld\n", incremento);
        for (int j = 0; j < incremento; j++) {
            linealInsertShort(vector, j, size, incremento);
        }
        f1 = clock();
        tiempos[i] += difftime(f1, i1)/ CLOCKS_PER_SEC;

    }
    return (EXIT_SUCCESS);
}

void linealInsertShort(long *vector, long offset, long size, long incrementos) {
    // for i = 1 ; i < size; i++
    for (long pos = offset + incrementos; pos < size; pos += incrementos) {
        //printf("offset=%d, incrementos=%d, pos=%d\n", offset, incrementos, pos);
        long pos2;
        for (pos2 = pos - incrementos; (pos2 >= offset) && vector[pos] < vector[pos2]; pos2 -= incrementos);
        // hay que cambiar por la posicion antes de pos2
        pos2 += incrementos;
        if (pos2 != pos) {
            //printf("offset=%d, incrementos=%d, pos=%d, pos2=%d\n", offset, incrementos, pos, pos2);
            long temp = vector[pos];
            for (long j = pos; j > pos2; j -= incrementos) {
                //printf("j=%d, offset=%d, incrementos=%d, pos=%d, pos2=%d\n", j, offset, incrementos, pos, pos2);
                vector[j] = vector[j - incrementos];
            }
            vector[pos2] = temp;
        }
    }
}


