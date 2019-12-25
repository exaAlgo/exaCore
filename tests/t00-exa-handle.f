      program t00_exa_handle
      include 'mpif.h'
      include 'EXA'

      integer ierr
      call mpi_init(ierr)

      call mpi_finalize(ierr)
      end
