      subroutine intdiary
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer mode(2)
      logical opened,eptover
      integer iadr,sadr
c

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(rhs.gt.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=0
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return

c     
      if(wio.ne.0) then
         mode(1)=0
         mode(2)=0
         call clunit(-wio,buf,mode)
      endif
      if(lunit.eq.0) goto 29
      wio=lunit
      istk(il)=0
      goto 999
 29   wio=0
      istk(il)=0
      goto 999
 999  return
      end
