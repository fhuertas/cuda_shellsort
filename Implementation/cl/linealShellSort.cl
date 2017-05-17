// created on 08-jun-2012

/**
 * @author paco
 */
kernel void linealShellSort(global long *vector, global size_t *offset, global size_t *size, global size_t *incrementos) {
    // for i = 1 ; i < size; i++
    for (long pos = (*offset) + (*incrementos); pos < (*size); pos += (*incrementos)) {
        //printf("offset=%d, incrementos=%d, pos=%d\n", offset, incrementos, pos);
        long pos2;
        for (pos2 = pos - (*incrementos); (pos2 >= (*offset)) && vector[pos] < vector[pos2]; pos2 -= (*incrementos));
        // hay que cambiar por la posicion antes de pos2
        pos2 += (*incrementos);
        if (pos2 != pos) {
            //printf("offset=%d, incrementos=%d, pos=%d, pos2=%d\n", offset, incrementos, pos, pos2);
            long temp = vector[pos];
            for (long j = pos; j > pos2; j -= (*incrementos)) {
                //printf("j=%d, offset=%d, incrementos=%d, pos=%d, pos2=%d\n", j, offset, incrementos, pos, pos2);
                vector[j] = vector[j - (*incrementos)];
            }
            vector[pos2] = temp;
        }
    }
}

