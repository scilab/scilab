      subroutine bitops(op)
c     addition
      include '../stack.h'
      common /mtlbc/ mmode
      integer is1,top0,op
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1     
c
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=il2+4
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=il1+4
      mn1=m1*n1

      if(((istk(il1).ne.8.or.istk(il2).ne.8).or.(it1.ne.it2)).and.
     $        mn1.ne.0.and.mn2.ne.0) then
         top=top+1
         fin=-fin
         return
      endif


 07   continue
      if (mn1.eq.0) then
         call error(43)
         return
      elseif (mn2.eq.0) then
         call error(43)
         return
      elseif (m1 .lt. 0) then
c     .  eye+vector
         call error(43)
         return
      elseif (m2 .lt. 0) then
c     .  vector+eye
         call error(43)
         return
      elseif (mn2.eq.1) then
c     .  vector+const
         call genbitops(it1,op,mn1,istk(l2),0,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn1))
      elseif (mn1.eq.1) then
c     .  cst+vector
         call gencopy(it1,1,istk(l1),1,is1,1)
         call gencopy(it1,mn2,istk(l2),1,istk(l1),1)
         call genbitops(it1,op,mn2,is1,0,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn2))

         istk(il1+1)=m2
         istk(il1+2)=n2
      else
c     .  vector+vector
         if (m1 .ne. m2.or.n1 .ne. n2) then
            call error(8)
            return
         endif
         call genbitops(it1,op,mn1,istk(l2),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn1))
      endif
 999  return
      end

