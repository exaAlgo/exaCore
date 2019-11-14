#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void square(__global double* input,__global double *output,unsigned int count)
{
   int i = get_global_id(0);
   if(i < count){
       output[i] = input[i]*input[i];
   }
}

__kernel void vectorAdd(__global double* inA,__global double *inB,__global double *out,
  unsigned int count)
{
   int i = get_global_id(0);
   if(i < count){
       out[i] = inA[i]+inB[i];
   }
}

__kernel void mxm(const unsigned int M,
                  const unsigned int N,
                  const unsigned int K,
                  const __global float *A,
                  const __global float *B,
                  __global float *C)
{
  const int gRow=get_global_id(0);
  const int gCol=get_global_id(1);

  float acc=0.f;
  for(int n=0;n<N;n++)
    acc+=A[gRow*N+n]*B[n*K+gCol];

  C[gRow*K+gCol]=acc;
}
