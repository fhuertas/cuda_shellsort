// created on 05-jun-2012

/**
 * Básicamente es se paraleliza la parte de linealShort.
 * V1: un hilo por incremento
 * @author paco
 */



kernel void shellSortD(global long* vector, constant size_t* psize, constant int *numXThread){
    size_t incrementos = get_global_size(0); 
    local size_t changes;
    int lecturas = 0;
    int escrituras = 0;
    int pasadas = 0;
    size_t bloque = get_global_size(0) * (*numXThread) ;
    size_t offset = get_local_id(1) * bloque + get_global_id(0); 
    size_t end_offset = offset + bloque;
    if (end_offset > *psize)
        end_offset = *psize;

    changes = 1;

    while (changes) {
        changes = 0;
        pasadas++;
        for (size_t pos = offset + incrementos; pos < end_offset ; pos += incrementos){
            size_t pos2 = pos - incrementos;
            size_t pos3 = pos;
            long temp = vector[pos3];
            lecturas ++;
            long temp2;// = vector[pos2];
            while((pos3 > offset ) && temp < (temp2 = vector[pos2])){
                lecturas++;
                vector[pos3] = temp2;
                escrituras++;
                pos3 = pos2;
                pos2= pos3 - incrementos;
                //temp2 = vector[pos2];
            }
            escrituras++;
            vector[pos3] = temp;
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        if (end_offset != *psize){
            long temp = vector[end_offset];
            long temp2 = vector[end_offset-incrementos];
            lecturas = lecturas +2;
            if (temp < temp2){
                vector[end_offset] = temp2;
                vector[end_offset-incrementos] = temp;
                changes = 1;
                
                escrituras = escrituras +2;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        
    }
    int csize = 1;
    if (get_global_size(0) == csize){
       vector[offset] = escrituras;
    }
}

//
//kernel void shellSortD_Variante(global long* vector, constant size_t* psize, constant int *numXThread){
//    size_t incrementos = get_global_size(0); 
//    local size_t changes;
//    size_t bloque = get_global_size(0) * (*numXThread) ;
//    size_t offset = get_local_id(1) * bloque + get_global_id(0); 
//    size_t end_offset = offset + bloque;
//    if (end_offset > *psize)
//        end_offset = *psize;
//
//
//    do {
//        int signo = 3;
//        changes = 0;
//        for (size_t pos = offset + incrementos; pos < end_offset ; pos += incrementos){
//            size_t pos2 = pos - incrementos;
//            size_t pos3 = pos;
//            long temp = vector[pos3];
//            long temp2;
//            while((pos3 > offset ) && temp < (temp2 = vector[pos2])){
//                vector[pos3] = temp2;
//                pos3 -= incrementos;
//                pos2 -= incrementos;
//                changes = 1;
//            }
//            vector[pos3] = temp;
//        }
//        barrier(CLK_LOCAL_MEM_FENCE);
//        if (changes) {
//            size_t n = signo * incrementos;
//            offset += n;
//            if (end_offset != *psize){
//                end_offset += n;
//            }
//            signo *= -1;
//        }
//        
//    } while (changes);
//}

