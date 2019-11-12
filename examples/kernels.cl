#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void square(__global double* input)
{
   int i = get_global_id(0);
   if(i < 10){
       input[i]  = 5.0;
   }
}
