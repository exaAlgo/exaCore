#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void copyVector(__global double* input,__global double *output,unsigned int count)
{
   int i=get_global_id(0);
   if(i<count)
       output[i]=input[i];
}

__kernel void scaleVector(__global double *output,__global double *input,
  double alpha,unsigned int count)
{
   int i=get_global_id(0);
   if(i<count)
     output[i]=alpha*input[i];
}

__kernel void axpbyVector(__global double *z,__global double *x,double alpha,
  __global double *y,double beta,unsigned int count)
{
  int i=get_global_id(0);
  if(i<count)
    z[i] = alpha*x[i]+beta*y[i];
}
