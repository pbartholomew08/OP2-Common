//
// auto-generated by op2.py
//

//user function
__device__ void update_gpu( double *phim, double *res, const double *u, double *rms) {
  *phim -= *u;
  *res = 0.0;
  *rms += (*u) * (*u);

}

// CUDA kernel function
__global__ void op_cuda_update(
  double *arg0,
  double *arg1,
  const double *__restrict arg2,
  double *arg3,
  int   set_size ) {


  //process set elements
  for ( int n=threadIdx.x+blockIdx.x*blockDim.x; n<set_size; n+=blockDim.x*gridDim.x ){
    for ( int d=0; d<1; d++ ){
      arg3[n+d]=ZERO_double;
    }

    //user-supplied kernel call
    update_gpu(arg0+n*1,
           arg1+n*1,
           arg2+n*1,
           arg3+n*1);
  }

  //global reductions

}


//host stub function
void op_par_loop_update(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3){

  double*arg3h = (double *)arg3.data;
  int nargs = 4;
  op_arg args[4];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(8);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[8].name      = name;
  OP_kernels[8].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  update");
  }

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //set CUDA execution parameters
    #ifdef OP_BLOCK_SIZE_8
      int nthread = OP_BLOCK_SIZE_8;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    //transfer global reduction data to GPU
    int reduct_bytes = 0;
    int reduct_size  = 0;
    reduct_bytes += ROUND_UP(set_size*arg3.size);
    reduct_size   = MAX(reduct_size,sizeof(double));
    reallocReductArrays(reduct_bytes);
    reduct_bytes = 0;
    arg3.data   = OP_reduct_h + reduct_bytes;
    arg3.data_d = OP_reduct_d + reduct_bytes;
    reduct_bytes += ROUND_UP(set_size*arg3.size);
    mvReductArraysToDevice(reduct_bytes);

    int nshared = reduct_size*nthread;
    op_cuda_update<<<nblocks,nthread,nshared>>>(
      (double *) arg0.data_d,
      (double *) arg1.data_d,
      (double *) arg2.data_d,
      (double *) arg3.data_d,
      set->size );
    //transfer global reduction data back to CPU
    mvReductArraysToHost(reduct_bytes);
    reprLocalSum(&arg3,set_size,(double*)arg3.data);
    arg3.data = (char *)arg3h;
    op_mpi_repr_inc_reduce_double(&arg3,(double*)arg3.data);
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[8].time     += wall_t2 - wall_t1;
  OP_kernels[8].transfer += (float)set->size * arg0.size * 2.0f;
  OP_kernels[8].transfer += (float)set->size * arg1.size * 2.0f;
  OP_kernels[8].transfer += (float)set->size * arg2.size;
}