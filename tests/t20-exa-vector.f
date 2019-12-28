      program t10_exa_handle
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer ierr,nargs,i

      integer handle

      integer vector
      integer length
      integer*8 in_offset,out_offset
      parameter(in_offset=0)

      integer M
      parameter(M=10)

      real*8 in_(M),out_(M)

      character*128 arg

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,handle,ierr)
      call exavectorcreate(handle,M,exa_scalar,vector,ierr)

      call exavectorgetsize(length,vector,ierr)
      if(length .ne. M) then
        write(stderr,*) 'exavectorgetsize failed.'
      endif

      do i=1,M
        in_(i) =i+0.1
        out_(i)=0.0
      enddo
      call exavectorwrite(vector,in_,in_offset,ierr)

      call exavectorread(vector,out_,out_offset,ierr)
      do i=1,M
        if(abs(out_(i)-(i+0.1))) then
          write(stderr,*) 'Error reading vector.'
        endif
      enddo

      call exavectorfree(vector,ierr)
      call exafinalize(handle,ierr)

      call mpi_finalize(ierr)
      end
