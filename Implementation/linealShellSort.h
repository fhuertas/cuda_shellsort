/* 
 * File:   linealShellShort.h
 * Author: paco
 *
 * Created on 1 de junio de 2012, 19:12
 */

#ifndef LINEALSHELLSHORT_H
#define	LINEALSHELLSHORT_H

#ifdef	__cplusplus
extern "C" {
#endif
    long incrementoN(int n);
    long listaIncrementosSedgewick(long max);
    long linealShellShort(long* vector, long size, double *tiempos);
    long incrementosSedgewick(long max);


#ifdef	__cplusplus
}
#endif

#endif	/* LINEALSHELLSHORT_H */

