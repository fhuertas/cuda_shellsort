// created on 05-jun-2012

/**
 * Básicamente es se paraleliza la parte de linealShort.
 * V1: un hilo por incremento
 * @author paco
 */
#define MAX_DATA 10


kernel void shellSortF(global long* vector, constant size_t* psize, constant int *numXThread){
    size_t incrementos = get_global_size(0); 
    global size_t *changes;
    size_t bloque = get_global_size(0) * (*numXThread) ;
    size_t offset = get_global_id(1) * bloque + get_global_id(0); 
    size_t end_offset = offset + bloque;
    if (end_offset > *psize)
        end_offset = *psize;

    *changes = 1;
    long cached[MAX_DATA];
    
    size_t elements = 0;
    size_t last_element;
    size_t j;
    for (j = offset; j < end_offset; elements++, j+=incrementos){
        cached[elements] = vector[j];
    }
    last_element = j-incrementos;
    while (*changes) {
        *changes = 0;
        for (size_t i = 1 ; i < elements; i++){
            size_t pos1 = i-1; 
            size_t pos2 = i;
            long temp = cached[pos2];
            while (pos2 > 0 && temp < cached[pos1]){
                cached[pos2] = cached[pos1];
                pos2--;
                pos1--;
            }
            cached[pos2] = temp;
        }
        vector[last_element] = cached[elements-1];
        vector[offset] = cached[0];
        barrier(CLK_GLOBAL_MEM_FENCE);
        if (end_offset != *psize){
            long temp = vector[end_offset];
            long temp2 = vector[end_offset-incrementos];
            if (temp < temp2){
                vector[end_offset] = temp2;
                vector[end_offset-incrementos] = temp;
                *changes = 1;
            }
        }
        barrier(CLK_GLOBAL_MEM_FENCE);
        cached[elements-1] = vector[last_element];
        cached[0] = vector[offset];
//        for (size_t pos = offset + incrementos; pos < end_offset ; pos += incrementos){
//            size_t pos2 = pos - incrementos;
//            size_t pos3 = pos;
//            long temp = vector[pos3];
//            long temp2 = vector[pos2];
//            while((pos3 > offset ) && temp < temp2){
//                //atomic_add(&changes,1);
//                vector[pos3] = temp2;
//                pos2-=incrementos;
//                pos3-=incrementos;
//                temp2 = vector[pos2];
//                
//            }
//            vector[pos3] = temp;
//        }
//        barrier(CLK_LOCAL_MEM_FENCE);
//        if (end_offset != *psize){
//            long temp = vector[end_offset];
//            long temp2 = vector[end_offset-incrementos];
//            if (temp < temp2){
//                vector[end_offset] = temp2;
//                vector[end_offset-incrementos] = temp;
//                changes = 1;
//            }
//        }
//        barrier(CLK_LOCAL_MEM_FENCE);
    }
    elements = 1;
    for (size_t j = offset+incrementos; j < last_element; elements++, j+=incrementos){
        vector[j] = cached[elements];
    }
}

