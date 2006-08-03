 
      subroutine intmtlbmode
c     Copyright INRIA
      include 'stack.h'
      integer offset
      logical checkrhs,checklhs,crebmat,getbmat,checkval
      common /mtlbc/ mmode
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      rhs=max(rhs,0)
c
      if(.not.checklhs('mtlb_mode',1,1)) return
      if(.not.checkrhs('mtlb_mode',0,1)) return

      if(rhs.eq.0) then
         top=top+1
         if(.not.crebmat('mtlb_mode',top,1,1,l)) return
         istk(l)=mmode
      else
         if(.not.getbmat('mtlb_mode',top,top,m,n,l)) return
         if(.not.checkval('mtlb_mode',m*n,1) ) return
         mmode=istk(l)
         call objvide('mtlb_mode',top)
      endif
      return
      end
