      program t00_exa_handle
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer handle,ierr,nargs
      character arg*64

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,handle,ierr)
      call exafinalize(handle,ierr)

      call mpi_finalize(ierr)
      end
