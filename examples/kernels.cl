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

#define TS 16

__kernel void mxm(const unsigned int M,
                  const unsigned int N,
                  const unsigned int K,
                  const __global double *A,
                  const __global double *B,
                  __global double *C)
{
  const int row =get_local_id(0);
  const int col =get_local_id(1);
  const int gRow=TS*get_group_id(0)+row;
  const int gCol=TS*get_group_id(1)+col;

  __local double Asub[TS][TS];
  __local double Bsub[TS][TS];

  double acc=0.f;

  const int numTiles=N/TS;
  for(int i=0;i<numTiles;i++){
    const int tiledRow=TS*i+row;
    const int tiledCol=TS*i+col;
    Asub[row][col]=A[gRow*N+tiledCol];
    Bsub[row][col]=B[tiledRow*K+gCol];

    barrier(CLK_LOCAL_MEM_FENCE);

    for(int n=0;n<TS;n++)
      acc+=Asub[row][n]*Bsub[n][col];

    barrier(CLK_LOCAL_MEM_FENCE);
  }

  C[gRow*K+gCol]=acc;
}

__kernel void mxm1(const unsigned int M,
                  const unsigned int N,
                  const unsigned int K,
                  const __global double *A,
                  const __global double *B,
                  __global double *C)
{
  const int gRow=get_global_id(0);
  const int gCol=get_global_id(1);

  double acc=0.f;
  for(int n=0;n<N;n++)
    acc+=A[gRow*N+n]*B[n*K+gCol];

  C[gRow*K+gCol]=acc;
}
