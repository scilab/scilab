    	subroutine scilyap(fname)
c     Copyright INRIA
      include '../stack.h'
      character*(*) fname
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c      
      call cvname(ids(1,pt+1),'lyap',0)
      fun=-1
c      
			end