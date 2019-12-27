      program t10_exa_handle
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer ierr,nargs
      integer handle
      integer settings
      integer order

      character*128 arg
      character*128 backend

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,handle,ierr)
      call exasettingscreate(handle,exa_str_null,settings,ierr)
      call exasettingssetint('order',10,settings,ierr)

      call exasettingsgetstr(backend,'backend::name',settings,ierr)
      if(backend .ne. arg) then
        write(stderr,*) "exasettingsgetstr failed."
      endif

      order=-1
      call exasettingsgetint(order,'order',settings,ierr)
      if(order.ne.10) then
        write(stderr,*) "exasettingsgetint failed.",order
      endif

      call exasettingsfree(settings,ierr)
      call exafinalize(handle,ierr)

      call mpi_finalize(ierr)
      end
