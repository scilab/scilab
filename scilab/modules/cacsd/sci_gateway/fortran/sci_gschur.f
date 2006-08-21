c     --------------------------
c     Copyright INRIA
c     --------------------------
      subroutine scigschur(fname)
c     Copyright INRIA
      INCLUDE 'stack.h'
c     
      character*(*) fname
      integer iadr,sadr
     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1
      
c     obsolete 
c     call macro for emulation
      call cvname(ids(1,pt+1),'gschur',0)
      fun=-1
      return
      end
c     --------------------------
      
      