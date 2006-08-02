c     **********************
c     Copyright INRIA
c     **********************
      subroutine intgetlabel
c
c
      include 'stack.h'
      integer iadr, sadr
      integer kfun
      integer getscilabel
      external getscilabel
      common /curblk/ kfun
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(rhs.gt.1) then
         call error(39)
         return
      endif
      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if(rhs.eq.1) then
         il=iadr(lstk(top))
         if(istk(il).ne.1) then 
            err=1
            call error(54)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=1
            call error(36)
            return
         endif
         kf=stk(sadr(il+4))
         top=top-1
      else
         kf=kfun
      endif
c
      lw = lstk(top+1)
      top=top+1
      il1=iadr(lstk(top))

      id1=il1+4
      l1=id1+2
      err=sadr(l1+50)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      ierr=getscilabel(kf,istk(l1),n)
      if(ierr.ne.0) then
         buf='scicosim is not running'
         call error(999)
         return
      endif
      istk(il1)=10
      istk(il1+1)=1
      istk(il1+2)=1
      istk(il1+3)=0
      istk(il1+4)=1
      istk(il1+5)=1+n
      lstk(top+1)=sadr(l1+n)
      return
      end
c     **********************
