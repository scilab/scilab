      subroutine i_d_i
c     element wise right division
      include '../stack.h'
      integer is1
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
      if(it1.ne.it2) then
         top=top+1
         fin=-fin
         return
      endif

 55   continue
      i1=1
      i2=1

      if(n1.lt.0.and.mn2.ne.1.or.n2.lt.0.and.mn1.ne.1) then
         call error(14)
         return
      endif
      if(mn1.ne.1.and.mn2.ne.1) then
c       check dimensions
         if (m1.ne.m2 .or. n1.ne.n2) then
            buf='inconsistent element-wise operation'
            call error(9999)
            return
         endif
      endif
      if(mn2.eq.1) then
c     .  vector ./ cst
         call genrdiv(it1,istk(l1),1,istk(l2),0,istk(l1),1,mn1,ierr)
      elseif(mn1.eq.1) then
c     .  cst ./ vector
         istk(il1+1)=m2
         istk(il1+2)=n2
         call gencopy(it1,1,istk(l1),1,is1,1)
         call genrdiv(it1,is1,0,istk(l2),1,istk(l1),1,mn2,ierr)
         lstk(top+1)=sadr(l1+memused(it1,mn2))
      else
c     .  vector ./ vector
         call genrdiv(it1,istk(l1),1,istk(l2),1,istk(l1),1,mn2,ierr)
         lstk(top+1)=sadr(l1+memused(it1,mn2))
      endif
      if(ierr.ne.0) then
         call error(27)
         return
      endif
      return
      end
