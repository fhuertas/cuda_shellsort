// created on 05-jun-2012

/**
 * Básicamente es se paraleliza la parte de linealShort.
 * V1: un hilo por incremento
 * @author paco
 */
kernel void shellSortCL(global long* vector, constant size_t* psize) {
    size_t offset = get_global_id(0);
    size_t incrementos = get_global_size(0);
    // menor que psize porque si es 0 pasa a valer mucho
    for (size_t pos = *psize-incrementos-1; pos < *psize; pos -= incrementos){
        long temp = vector[pos];
        
        size_t pos2 = pos + incrementos;
        size_t pos3 = pos;
        while((pos2 < *psize) && temp > vector[pos2]){
            vector[pos3] = vector[pos2];
            pos2+=incrementos;
            pos3+=incrementos;
        }
        vector[pos3] = temp;
    }
}


