      subroutine bashos( ligne, n, nout, ierr)
c
c ================================== ( Inria    ) =============
c
c     Copyright INRIA
      include '../stack.h'
      integer    ierr,n,nout
      character  ligne*(*)
c
      nout=0
      ierr=0

      ln = len ( ligne )
      ln = min ( ln , n )

      if (ln.le.0) goto 100
      if(ligne(1:ln).eq.' ') goto 100
      buf=ligne(1:ln)// char(0)
      call systemc(buf,ierr)
  100 continue
      end
