      subroutine intprint
c     Copyright INRIA/ENPC
      INCLUDE '../stack.h'
c     
      integer flag,top2,tops
      integer mode(2)
      logical opened,eptover
      integer iadr,sadr
c
      save opened,lunit
c     
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if(rhs.le.1) then
         call error(42)
         return
      endif
      if(lhs.gt.1) then
         call error(41)
         return
      endif

c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(1)=0
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      l = lct(2)
      if(lunit.ne.wte) then
         lct(2) = 0
      endif
      top=top2
      do 26 i=2,rhs
         tops=top
         call print(idstk(1,top),tops,lunit)
         top=top-1
 26   continue
      lct(2) = l
      istk(il)=0
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      go to 999
 999  return
      end
