      subroutine intwrite4b
c     --------------------
c     write binaire real*4
c     --------------------
c     Copyright INRIA
      INCLUDE '../stack.h'
      integer mode(2),top2

      logical opened
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      lw=lstk(top+1)
      il=iadr(lstk(top))
      if(lhs.gt.1) then
         call error(41)
         return
      endif
      if(rhs.eq.3) then
         iacces=top
         ilb=iadr(lstk(iacces))
         if(istk(ilb+3).ne.0) then
            err=3
            call error(52)
            return
         endif
         nb=istk(ilb+1)*istk(ilb+2)
         lb=sadr(ilb+4)
         top=top-1
         rhs=rhs-1
         mode(1)=110
      elseif(rhs.eq.2) then
         mode(1)=100
         iacces=0
      else
         call error(39)
         return
      endif


c     opening file
      top2 = top
      top = top-rhs+1
      il=iadr(lstk(top))
      mode(2)=0
      call v2unit(top,mode,lunit,opened,ierr)
      if(ierr.gt.0) return
c     
      if(lunit.eq.wte) then
         call error(49)
         return
      endif
      nc=0
      il=iadr(lstk(top2))
      if(istk(il).ne.1) then
         err=2
         call error(53)
         return
      endif
      if(istk(il+3).ne.0) then
         err=2
         call error(52)
         return
      endif

c     
      m=istk(il+1)
      n=istk(il+2)
      l=sadr(il+4)
      ls=iadr(l)
      do 180 i=0,m*n-1
         sstk(ls+i)=stk(l+i)
 180  continue
      if(iacces.ne.0) then
         if(nb.ne.m) then
            call error(42)
            return
         endif
         call entier(nb,stk(lb),istk(ilb))
         li=ls
         do 181 i=1,m
            write(lunit,rec=istk(ilb-1+i),err=998)
     $           (sstk(li+(j-1)*m),j=1,n)
            li=li+1
 181     continue
      else
         li=ls
         do 182 i=1,m
            write(lunit,err=998) (sstk(li+(j-1)*m),j=1,n)
            li=li+1
 182     continue
      endif
c     
      il=iadr(lstk(top))
      istk(il)=0
      if(.not.opened) then
         mode(1)=0
         mode(2)=0
         call clunit(-lunit,buf,mode)
      endif
      goto 999
c     --------------
c     erreur lecture
c     --------------
c     
 998  call error(49)
      if(.not.opened) call clunit(-lunit,buf,mode)
      return
c     
 999  return
      end

