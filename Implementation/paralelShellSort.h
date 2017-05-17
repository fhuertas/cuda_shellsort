/* 
 * File:   paralelShellSort.h
 * Author: paco
 *
 * Created on 6 de junio de 2012, 9:48
 */
#include <CL/cl.h>

#ifndef PARALELSHELLSORT_H
#define	PARALELSHELLSORT_H

#ifdef	__cplusplus
extern "C" {
#endif

    struct t_paralel_context {
        char * src;
        cl_platform_id platformId;
        cl_device_id deviceId;
        cl_uint result;
        cl_context context;
        cl_command_queue cq;
        cl_kernel kernel;
        cl_program prog;
        cl_mem *params;
        cl_uint nParams;
    };

    typedef struct t_paralel_context parallel_context;


    double paralelShellSort_0(parallel_context *context, long* vectorA, size_t size, int imprimir, double* tiempoPorVuelta);
    double paralelShellSort_A(parallel_context *context, long* vectorA, size_t size, int imprimir, double* tiempoPorVuelta);
    double paralelShellSort_B(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread);
    double paralelShellSort_C(parallel_context *context, int* vectorA, int size, int imprimir, double* tiempoPorVuelta);
    double paralelShellSort_D(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread);
    double paralelShellSort_E(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread);
    double paralelShellSort_F(parallel_context *context, long* vectorA, int size, int imprimir, double* tiempoPorVuelta, size_t datos_x_thread);
    void inicializeContext(parallel_context *context, const char* fileName, const char* function, const char* selected_platform_name, const char* selected_device_name);
    void writeMemory(parallel_context *context, int params, void **arguments, int* sizes, cl_mem_flags *flags, int rewrite);
    void exec(parallel_context *context, size_t dimensions, const size_t* general_worksize, const size_t* local_worksize);
    void cleanContext(parallel_context *context);


#ifdef	__cplusplus
}
#endif

#endif	/* PARALELSHELLSORT_H */

