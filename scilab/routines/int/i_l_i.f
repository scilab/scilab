      subroutine i_l_i
c     left division
      include '../stack.h'
      integer is1
      external memused
      integer memused
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

      if (m1*n1 .ne. 1) then
         if(m2.lt.0) then
            call error(14)
            return
         endif
         top=top+1
         fin=-fin
         return
      else
c     .  cst \ vector
         if(m1.lt.0.and.mn2.ne.1) then 
            call error(14)
            return
         endif
         istk(il1+1)=m2
         istk(il1+2)=n2
         istk(il1+3)=it1
c     
         call gencopy(it1,1,istk(l1),1,is1,1)
         call gencopy(it1,mn2,istk(l2),1,istk(l1),1)
         call genrdiv(it1,istk(l1),1,is1,0,istk(l1),1,mn2,ierr)
         lstk(top+1)=sadr(l1+memused(it1,mn2))
         if(ierr.ne.0) then
            call error(27)
            return
         endif

      endif
      return
      end

