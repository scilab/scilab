      subroutine i_m_i
c     multiplication
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
c
      if(istk(il1).ne.8.or.istk(il2).ne.8) then
         top=top+1
         fin=-fin
         return
      endif

      if(it1.ne.it2) then
         top=top+1
         fin=-fin
         return
      endif

 10   continue
      if (mn1 .eq. 1) then
c     .  cst*a
         is1 = istk(l1)
         if (m1.lt.0) then
            if(mn2.eq.1) then
c     .     eye*cst
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
c     .  a*cst
         if(m2.lt.0) then
            call error(14)
            return
         endif
         call genscal(it1,mn1,istk(l2),istk(l1),1)
      else
c     .  matrix*matrix
         if (n1 .ne. m2) then
            call error(10)
            return
         endif
         lr=l2+mn2
         err=sadr(lr+m1*n2)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call genmmul(it1,istk(l1),m1,istk(l2),m2,istk(lr),m1,m1,n1,n2)
         call gencopy(it1,m1*n2,istk(lr),1,istk(l1),1)
         lstk(top+1)=sadr(l1+memused(it1,m1*n2))
         istk(il1+2)=n2
         istk(il1+3)=it1

      endif
 999  return
      end
