!-----------------------------------------------------------------------
! Parameters for Fortran 77 interface
!-----------------------------------------------------------------------

!-----------------------------------------------------------------------
! Define stdin,stdout and stderr
!-----------------------------------------------------------------------
      integer stdin,stdout,stderr
      parameter(stdin =5)
      parameter(stdout=6)
      parameter(stderr=0)

!-----------------------------------------------------------------------
! exaDataType 
!-----------------------------------------------------------------------
      integer exa_int 
      parameter(exa_int=0)

      integer exa_uint 
      parameter(exa_uint=1)

      integer exa_long 
      parameter(exa_long=2)

      integer exa_ulong 
      parameter(exa_ulong=3)

      integer exa_scalar 
      parameter(exa_scalar=4)

      integer exa_char 
      parameter(exa_char=5)

      integer exa_str 
      parameter(exa_str=6)
