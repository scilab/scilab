      subroutine unarybit(op)
c     addition
      include '../stack.h'
      common /mtlbc/ mmode
      character*8 fct
      integer op
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1     
c

      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=il1+4
      mn1=m1*n1

 07   continue
      if (mn1.eq.0) then
         call error(43)
         return
      elseif (m1 .lt. 0) then
c     .  eye+vector
         call error(43)
         return
      else
         call genbitops(it1,op,mn1,istk(l1),1,0,1)
         lstk(top+1)=sadr(l1+memused(it1,mn1))
      endif
 999  return
      end

