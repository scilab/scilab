      subroutine i_x_i
c     element wise product
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

c     element wise operations
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
      lstk(top+1)=sadr(l1+memused(it1,max(mn1,mn2)))
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif

      if (mn1 .eq. 1) then
c     .  cst.,a  (inline product code)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye.*cst
               istk(il1+1)=m1
               istk(il1+2)=n1
               istk(il1+3)=it1
            else
               call error(14)
               return
            endif
         else
            istk(il1+1)=m2
            istk(il1+2)=n2
            istk(il1+3)=it1
         endif
         call gencopy(it1,1,istk(l1),1,is1,1)
         call gencopy(it1,mn2,istk(l2),1,istk(l1),1)
         call genscal(it1,mn2,is1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,mn2))
      elseif (mn2 .eq. 1) then
c     .  a.*cst (inline product code)
         if(m2.lt.0) then
            call error(14)
            return
         endif
         call genscal(it1,mn1,istk(l2),istk(l1),1)
      else
c     matrix.*matrix
         call genvmul(it1,mn1,istk(l2),i2,istk(l1),i1)
      endif
      return
      end
