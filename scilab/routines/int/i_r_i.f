      subroutine i_r_i
c     division a droite
      include '../stack.h'
      integer iadr,sadr
c     
      iadr(l)=l+l-1
c      sadr(l)=(l/2)+1
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

      if (mn2 .ne. 1) then
         fin = -fin
         top = top+1
      else
c     .  vector / cst
         if(m2.lt.0.and.mn1.ne.1) then 
            call error(14)
            return
         endif
         call genrdiv(it1,istk(l1),1,istk(l2),0,istk(l1),1,mn1,ierr)
         if(ierr.ne.0) then
            call error(27)
            return
         endif
      endif
 999  return
      end

