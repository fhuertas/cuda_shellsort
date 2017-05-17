// created on 05-jun-2012

/**
 * Básicamente es se paraleliza la parte de linealShort.
 * V1: un hilo por incremento
 * @author paco
 */
kernel void shellSortCLB(global long* vector, constant size_t* psize, constant int *numXThread){
    size_t incrementos = get_global_size(0); 
    size_t bloque = get_global_size(0) * (*numXThread);
    size_t offset = get_global_id(1) * bloque + get_global_id(0); 
    size_t end_offset = offset + bloque;
    if (end_offset > *psize)
        end_offset = *psize;
    for (size_t pos = offset + incrementos; pos < end_offset; pos += incrementos){
        long temp = vector[pos];
        
        size_t pos2 = pos - incrementos;
        size_t pos3 = pos;
        while((pos3 > offset ) && temp < vector[pos2]){
            vector[pos3] = vector[pos2];
            pos2-=incrementos;
            pos3-=incrementos;
        }
        vector[pos3] = temp;
    }
    
}

