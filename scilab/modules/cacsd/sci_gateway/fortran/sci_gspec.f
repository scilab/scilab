c     --------------------------
c     Copyright INRIA
c     --------------------------
      subroutine scigspec(fname)
      INCLUDE 'stack.h'
c     
      character*(*) fname
      integer iadr,sadr
     
      sadr(l)=(l/2)+1
      iadr(l)=l+l-1

c     osolete
c     call macro for emulation
      call cvname(ids(1,pt+1),'gspec',0)
      fun=-1
      return

      end
c     --------------------------
      