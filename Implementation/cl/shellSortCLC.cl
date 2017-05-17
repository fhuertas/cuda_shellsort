// created on 05-jun-2012

kernel void shellSortCLC(global int* vector, constant int* psize) {
    size_t offset = get_global_id(0);
    size_t incrementos = get_global_size(0);
    for (size_t pos = offset + incrementos; pos < *psize; pos += incrementos){
        size_t pos2;
        for (pos2 = pos - incrementos; (pos2 >= offset) && vector[pos] < vector[pos2]; pos2 -= incrementos);
        pos2 += incrementos;
        if (pos2 != pos) {
            //long temp = vector[pos];
            int temp = vector[pos];
            for (size_t j = pos; j > pos2; j -= incrementos) {
                vector[j] = vector[j - incrementos];
            }
            vector[pos2] = temp;
        }
    }
}


