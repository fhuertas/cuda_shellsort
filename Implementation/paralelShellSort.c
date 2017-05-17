#include "linealShellSort.h"
#include "paralelShellSort.h"
#include "openClUtilities.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define IMPRIMIRVECTORES

void printVectoro(long* vector, long size) {
    printf("(%ld", vector[0]);
    for (int i = 1; i < size; i++) {
        printf(",%ld", vector[i]);
    }
    printf(")\n");
}

void printVectorA(int* vector, long size) {
    printf("(%d", vector[0]);
    for (int i = 1; i < size; i++) {
        printf(",%d", vector[i]);
    }
    printf(")\n");
}

void inicializeContext(parallel_context *context, const char* fileName, const char* function, const char* selected_platform_name, const char* selected_device_name) {
    context->src = readCode(fileName);
    selectPlatformAndDevice(selected_platform_name, selected_device_name, &(context->platformId), &(context->deviceId));

    cl_context_properties properties[] = {
        CL_CONTEXT_PLATFORM, (cl_context_properties) context->platformId,
        0 // Las propiedades deben acabar en 0, NULL también vale pero a saber // http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clCreateContext.html
        // ¿? Comentario del archivo // Fetch the Platform and Device IDs; we only want one.
    };

    context->context = clCreateContext(properties, 1, &(context->deviceId), NULL, NULL, &(context->result));
    if (context->result != CL_SUCCESS)
        return;
    context->cq = clCreateCommandQueue(context->context, context->deviceId, 0, &(context->result));
    if (context->result != CL_SUCCESS)
        return;
    const char **pchar = (const char**) &(context->src);
    size_t code_size = strlen(context->src);
    context->prog = clCreateProgramWithSource(context->context, 1, pchar, &code_size, &(context->result));
    if (context->result != CL_SUCCESS)
        return;

    context->result = clBuildProgram(context->prog, 0, NULL, "", NULL, NULL);
    if (context->result != CL_SUCCESS)
        return;
    //printf("%s",function);
    context->kernel = clCreateKernel(context->prog, function, &(context->result));
    if (context->result != CL_SUCCESS)
        return;

}

void writeMemory(parallel_context *context, int params, void **arguments, int* sizes, cl_mem_flags *flags, int rewrite) {
    // creating buffers
    if (rewrite) {
        context->nParams = params;
        context->params = (cl_mem*) realloc(context->params, params * sizeof (cl_mem));
    }else {
        context->nParams = params;
        context->params = (cl_mem*) malloc(params * sizeof (cl_mem));
        
    }
    for (int i = 0; i < params; i++) {
        if (arguments[i] != NULL) {
            // reservando memoria
            context->params[i] = clCreateBuffer(context->context, flags[i], sizes[i], NULL, &(context->result));
            if (context->result != CL_SUCCESS)
                return;
            // enlazando la memoria con el parametro
            context->result = clSetKernelArg(context->kernel, i, sizeof (context->params[i]), &(context->params[i]));
            if (context->result != CL_SUCCESS)
                return;
            // enlazando la memoria con la funcion
            //void *pvoid = ( void*)&(arguments[i]);
            context->result = clEnqueueWriteBuffer(context->cq, context->params[i], CL_FALSE, 0, sizes[i], arguments[i], 0, NULL, NULL);
            if (context->result != CL_SUCCESS)
                return;
        }
    }
}

void readMemory() {

}

void cleanContext(parallel_context *context) {
    free(context->params);

}

void exec(parallel_context *context, size_t dimensions, const size_t* general_worksize, const size_t* local_worksize) {
    context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensions, NULL, general_worksize, local_worksize, 0, NULL, NULL);
}

double paralelShellSort_0(parallel_context *context, long* vectorA, size_t size, int imprimir, double* tiempoPorVuelta) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 4;
    void *args[nParams];
    int sizes[nParams];
    cl_mem_flags flags[nParams];
    if (imprimir)
        printVectoro(vectorA, size);
    inicio = clock();

    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = NULL;
    args[2] = (void*) &size;
    sizes[2] = sizeof (size_t);
    flags[2] = CL_MEM_READ_ONLY;
    args[3] = NULL;
    writeMemory(context, nParams, args, sizes, flags,0);
    if (context->result != 0) {
        printf("writeMemory1=%d\n", context->result);
        return 0;
    }
    long pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        i1 = clock();

        long incremento = incrementoN(i);
        for (int i = 0; i < incremento; i++) {
            args[0] = NULL;

            args[1] = (void*) &i;
            sizes[1] = sizeof (size_t);
            flags[1] = CL_MEM_READ_ONLY;

            args[2] = NULL;


            args[3] = (void*) &incremento;
            sizes[3] = sizeof (size_t);
            flags[3] = CL_MEM_READ_ONLY;
            writeMemory(context, nParams, args, sizes, flags,1);

            if (context->result != 0) {
                printf("writeMemory2=%d\n", context->result);
                return 0;
            }

            int dimensiones = 1;
            size_t general_worksize[] = {1};
            size_t local_worksize[] = {1};

            context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
            if (context->result != 0) {
                printf("Error de ejecucion=%d\n", context->result);
                return 0;
            }
            context->result = clFinish(context->cq);
        }
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;
        context->result = clFinish(context->cq);
    }
    long vectorTemp[size];

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    if (context->result != 0) {
        printf("clEnqueueReadBuffer1=%d\n", context->result);
        //return 0;
    }
    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, &vectorTemp[0], 0, NULL, NULL);
    if (context->result != 0) {
        printf("clEnqueueReadBuffer2=%d\n", context->result);
        //return 0;
    }
    context->result = clFinish(context->cq);
    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectoro(vectorA, size);
    if (imprimir)
        printVectoro(vectorTemp, size);
}

double paralelShellSort_A(parallel_context *context, long* vectorA, size_t size, int imprimir, double* tiempoPorVuelta) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 2;
    void *args[nParams];
    int sizes[nParams];
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (size_t);
    flags[1] = CL_MEM_READ_ONLY;

    writeMemory(context, nParams, args, sizes, flags,0);

    if (imprimir)
        printVectoro(vectorA, size);
    inicio = clock();

    long pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        long incremento = incrementoN(i);
        //long incremento = incrementoN(pasadas);

        int dimensiones = 1;
        size_t general_worksize[] = {incremento};
        size_t local_worksize[] = {1};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("Error de ejecucion=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;

        //long incremento = precalculated[i];
        //if (imprimir) printf("Incrementos=%ld\n", incremento);
        context->result = clFinish(context->cq);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectoro(vectorA, size);



    return total;
}

#define DATOS_POR_THREAD 100

double paralelShellSort_B(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 3;
    void *args[nParams];
    int sizes[nParams];
    if (imprimir)
        printVectoro(vectorA, size);
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (int);
    flags[1] = CL_MEM_READ_ONLY;
    int numXthreads = datos_x_thread;
    args[2] = (void*) &numXthreads;
    sizes[2] = sizeof (int);
    flags[2] = CL_MEM_READ_ONLY;

    /*    
        args[3] = (void*) &numXthreads;
        sizes[3] = sizeof (int);
        flags[3] = CL_MEM_READ_WRITE;
     */
    writeMemory(context, nParams, args, sizes, flags,0);
    if (context->result != 0)
        printf("writeMemory=%d\n", context->result);

    inicio = clock();

    long pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        long incremento = incrementoN(i);
        //long incremento = incrementoN(pasadas);

        size_t threadXGrupo = (size / incremento) / datos_x_thread;
        if (threadXGrupo == 0) {
            threadXGrupo = 1;
        }

        int dimensiones = 2;
        size_t general_worksize[] = {incremento, threadXGrupo};
        size_t local_worksize[] = {1, threadXGrupo};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("clEnqueueNDRangeKernel=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        if (context->result != 0)
            printf("clFinish1=%d\n", context->result);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;

        //long incremento = precalculated[i];
        //printf("Incrementos=%ld\n", incremento);
        context->result = clFinish(context->cq);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    if (context->result != 0)
        printf("clFinish2=%d\n", context->result);
    clReleaseMemObject(context->params[0]);
    clReleaseMemObject(context->params[1]);

    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectoro(vectorA, size);



    return total;
}

double mediaVectorInf(long* vector, int max, long SIZE){
    int cantidad = 0;
    int acumulado = 0;
    for (int i = 0; i < SIZE; i++){
        if (vector[i] < max){
            cantidad++;
            acumulado += vector[i];
            
        }
    }
    return (acumulado / (cantidad * 1.0));
}

double paralelShellSort_D(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 3;
    void *args[nParams];
    int sizes[nParams];
    if (imprimir)
        printVectoro(vectorA, size);
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (int);
    flags[1] = CL_MEM_READ_ONLY;
    int numXthreads = datos_x_thread;
    args[2] = (void*) &numXthreads;
    sizes[2] = sizeof (int);
    flags[2] = CL_MEM_READ_ONLY;

    /*    
        args[3] = (void*) &numXthreads;
        sizes[3] = sizeof (int);
        flags[3] = CL_MEM_READ_WRITE;
     */
    writeMemory(context, nParams, args, sizes, flags,0);
    if (context->result != 0)
        printf("writeMemory=%d\n", context->result);

    inicio = clock();

    long pasadas = listaIncrementosSedgewick(size);
    //pasadas = 1;
    for (int i = pasadas - 1; i >= 0; i--) {

        long incremento = incrementoN(i);
        //incremento = 1;

        size_t threadXGrupo = (size / incremento) / datos_x_thread;
        if (threadXGrupo == 0) {
            threadXGrupo = 1;
        } else {
            if (size / incremento % datos_x_thread){
                threadXGrupo++;
            }
        }

        int dimensiones = 2;
        size_t general_worksize[] = {incremento, threadXGrupo};
        size_t local_worksize[] = {1, threadXGrupo};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("clEnqueueNDRangeKernel=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        if (context->result != 0)
            printf("clFinish1=%d\n", context->result);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;
        context->result = clFinish(context->cq);
        context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    if (context->result != 0)
        printf("clFinish2=%d\n", context->result);
    clReleaseMemObject(context->params[0]);
    clReleaseMemObject(context->params[1]);

    final = clock();
    total += difftime(final, inicio);
    //double media = mediaVectorInf(vectorA, 999, size);
    //total = media;
    if (imprimir)
        printVectoro(vectorA, size);



    return total;
}
double paralelShellSort_E(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 3;
    void *args[nParams];
    int sizes[nParams];
    if (imprimir)
        printVectoro(vectorA, size);
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (int);
    flags[1] = CL_MEM_READ_ONLY;
    int numXthreads = datos_x_thread;
    args[2] = (void*) &numXthreads;
    sizes[2] = sizeof (int);
    flags[2] = CL_MEM_READ_ONLY;

    /*    
        args[3] = (void*) &numXthreads;
        sizes[3] = sizeof (int);
        flags[3] = CL_MEM_READ_WRITE;
     */
    writeMemory(context, nParams, args, sizes, flags,0);
    if (context->result != 0)
        printf("writeMemory=%d\n", context->result);

    inicio = clock();

    long pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        long incremento = incrementoN(i);
        //long incremento = incrementoN(pasadas);

        size_t threadXGrupo = (size / incremento) / datos_x_thread;
        if (threadXGrupo == 0) {
            threadXGrupo = 1;
        } else {
            if (size / incremento % datos_x_thread){
                threadXGrupo++;
            }
        }

        int dimensiones = 2;
        size_t general_worksize[] = {incremento, threadXGrupo};
        size_t local_worksize[] = {1, threadXGrupo};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("clEnqueueNDRangeKernel=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        if (context->result != 0)
            printf("clFinish1=%d\n", context->result);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;

        //long incremento = precalculated[i];
        //printf("Incrementos=%ld\n", incremento);
        context->result = clFinish(context->cq);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    if (context->result != 0)
        printf("clFinish2=%d\n", context->result);
    clReleaseMemObject(context->params[0]);
    clReleaseMemObject(context->params[1]);

    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectoro(vectorA, size);



    return total;
}

double paralelShellSort_F(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 3;
    void *args[nParams];
    int sizes[nParams];
    if (imprimir)
        printVectoro(vectorA, size);
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (long) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (int);
    flags[1] = CL_MEM_READ_ONLY;
    int numXthreads = datos_x_thread;
    args[2] = (void*) &numXthreads;
    sizes[2] = sizeof (int);
    flags[2] = CL_MEM_READ_ONLY;

    /*    
        args[3] = (void*) &numXthreads;
        sizes[3] = sizeof (int);
        flags[3] = CL_MEM_READ_WRITE;
     */
    writeMemory(context, nParams, args, sizes, flags,0);
    if (context->result != 0)
        printf("writeMemory=%d\n", context->result);

    inicio = clock();

    long pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        long incremento = incrementoN(i);
        //long incremento = incrementoN(pasadas);

        size_t threadXGrupo = (size / incremento) / datos_x_thread;
        if (threadXGrupo == 0) {
            threadXGrupo = 1;
        } else {
            if (size / incremento % datos_x_thread){
                threadXGrupo++;
            }
        }
        //printf("threads%uld\n", threadXGrupo);

        int dimensiones = 2;
        size_t general_worksize[] = {incremento, threadXGrupo};
        size_t local_worksize[] = {1, 1};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("clEnqueueNDRangeKernel=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        if (context->result != 0)
            printf("clFinish1=%d\n", context->result);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;

        //long incremento = precalculated[i];
        //printf("Incrementos=%ld\n", incremento);
        context->result = clFinish(context->cq);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (long) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    if (context->result != 0)
        printf("clFinish2=%d\n", context->result);
    clReleaseMemObject(context->params[0]);
    clReleaseMemObject(context->params[1]);

    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectoro(vectorA, size);



    return total;
}


double paralelShellSort_C(parallel_context *context, int* vectorA, int size, int imprimir, double* tiempoPorVuelta) {
    double total = 0;
    clock_t inicio, final, i1, f1;
    int nParams = 2;
    void *args[nParams];
    int sizes[nParams];
    cl_mem_flags flags[nParams];
    args[0] = vectorA;
    sizes[0] = sizeof (int) * size;
    flags[0] = CL_MEM_READ_WRITE;
    args[1] = (void*) &size;
    sizes[1] = sizeof (int);
    flags[1] = CL_MEM_READ_ONLY;

    writeMemory(context, nParams, args, sizes, flags,0);


    inicio = clock();

    int pasadas = listaIncrementosSedgewick(size);
    for (int i = pasadas - 1; i >= 0; i--) {

        int incremento = incrementoN(i);
        //long incremento = incrementoN(pasadas);

        int dimensiones = 1;
        size_t general_worksize[] = {incremento};
        size_t local_worksize[] = {1};
        i1 = clock();
        context->result = clEnqueueNDRangeKernel(context->cq, context->kernel, dimensiones, NULL, general_worksize, local_worksize, 0, NULL, NULL);
        if (context->result != 0) {
            printf("Error de ejecucion=%d\n", context->result);
            return 0;
        }
        context->result = clFinish(context->cq);
        f1 = clock();
        tiempoPorVuelta[i] += difftime(f1, i1) / CLOCKS_PER_SEC;

        //long incremento = precalculated[i];
        //printf("Incrementos=%ld\n", incremento);
        context->result = clFinish(context->cq);
    }

    context->result = clEnqueueReadBuffer(context->cq, context->params[0], CL_FALSE, 0, sizeof (int) * size, vectorA, 0, NULL, NULL);
    context->result = clFinish(context->cq);
    //vectorB = &vectorA;
    final = clock();
    total += difftime(final, inicio);
    if (imprimir)
        printVectorA(vectorA, size);



    return total;
}