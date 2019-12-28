      program t30_exa_program
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer ierr,nargs,i
      integer handle,settings,prog,kernel,vector

      character*1024 exename
      character*128  arg

      integer M
      parameter(M=10)

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(0,exename)
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,handle,ierr)
      call exasettingscreate(handle,exa_str_null,settings,ierr)

!     remote '-f' at the end of fortran tests
      exename=exename(1:len_trim(exename)-2)
      call exaprogramcreate(handle,trim(exename),settings,prog,ierr)

      call exakernelcreate(prog,'addVectors',kernel,ierr)

      call exavectorcreate(handle,M,exa_scalar,vector,ierr)

!     TODO run the kernel and compare answers

      call exavectorfree(vector,ierr)
      call exakernelfree(kernel,ierr)
      call exaprogramfree(prog,ierr)
      call exasettingsfree(settings,ierr)
      call exafinalize(handle,ierr)

      call mpi_finalize(ierr)
      end
