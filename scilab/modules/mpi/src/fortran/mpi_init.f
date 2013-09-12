      subroutine scimpiinit(ierr)
      include "mpif.h"
      call MPI_INIT(ierr)
      end


      subroutine scimpicommrank(comm, myid, ierr)
      include "mpif.h"
      call MPI_COMM_RANK(comm, myid, ierr)
      end


      subroutine scimpicommsize(comm, numprocs, ierr)
      include "mpif.h"
      call MPI_COMM_SIZE(comm, numprocs, ierr)
      end
