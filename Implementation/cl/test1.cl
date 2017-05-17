// created on 29-may-2012

/**
 * @author paco
 */
kernel void test1(global int* a,global int *psize) {
    int size = *psize;
    int dimX = 0;
    int dimY = 1;
    uint dim = get_work_dim();
    int groups = get_num_groups(0);
    int id = get_global_id(0);
    int localid = get_local_id(0);
    int id1 = get_group_id(0);
    // local (que dimension esta)
    size_t total = 0;
    // calculo de la poscion final
//    for (int i = 0;i < dim ;i++){
//        size_t dimSize = get_
//        total = dim *
//    }
    size_t i = get_local_id(dimX);
    size_t j = get_local_id(dimY);
    size_t A = i + (j * get_local_size(dimX));
    size_t k = get_global_id(dimX);
    size_t m = get_global_id(dimY);
    size_t B = k+m;
    size_t C = k + m * get_global_size(dimX);
    //C = get_global_offset(1);
    //C = m_ + k_;
    size_t localSize = get_local_size(1);
    size_t globalSize = get_global_size(0);    
    size_t offset = get_global_offset(0);
    size_t groupX = k/get_local_size(dimX);
    size_t groupY = m/get_local_size(dimY);
    if (C >= 0 && C< size-1) {
        int b;
        int c;
        b = get_local_id(dimX);
        c = get_local_id(dimY);
        b = get_num_groups(dimX);
        c = get_num_groups(dimY);
        b = get_local_size(dimX);
        c = get_local_size(dimY);
        b = groupX;
        c = groupY;
        
        a[C] = b + (100*c); 
        //a[C] = (i)+ (j *100) + (k * 10000 ) + (m * 1000000 );
        //a[C] = ((j+m_)*get_global_size(dimX));
        //a[B] = B;
        //a[B] = (k_)+ (m_ *100);
        //a[B] = m_ * get_global_size(dimX) * get_local_size(dimY);        //a[B] = (i)+ (j *100);
        //a[k_] = k_;        //a[B] = (i)+ (j *100);
        //atomic_add(&a[k_+32],1);
        //a[A] = get_local_size(dimY);
    }
    //atomic_add(&a[get_global_id(dimY)],1);
    //atomic_add(&a[size-1],1);
    if (id >= size) {
        return;
    }
}


// created on 29-may-2012

/**
 * @author paco
 */
kernel void test12(global int* a,global int *psize) {
    int size = *psize;
    uint dim = get_work_dim();
    int id = get_global_id(0);
    int id1 = get_group_id(dim);
    size_t localSize = get_local_size(dim);
    size_t globalSize = get_global_size(dim);
    if (0 >= id && id <= size) {
        a[id] = 1;
    }
    if (id >= size) {
        return;
    }
}
