      subroutine i_c_i
      include '../stack.h'
      common /mtlbc/ mmode
      integer top0
      external memused
      integer memused
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
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
      

      if((istk(il1).ne.istk(il2).or.it1.ne.it2).and.
     $     mn1.ne.0.and.mn2.ne.0) then
         top=top0
         fin=-fin
         return
      endif
      ityp=max(istk(il1),istk(il2))

c     concatenation [a b]
 75   if(m1.lt.0.or.m2.lt.0) then
         call error(14)
         return
      endif
      if(m2.eq.0) then
         return
      elseif(m1.eq.0)then
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(m1.ne.m2) then
         call error(5)
         return
      endif
c     
      call genconcatrow(it1,istk(l1),m1,n1,istk(l2),m2,n2)
      n=n1+n2
      lstk(top+1)=sadr(il1+4+memused(it1,m1*n))


      istk(il1+1)=m1
      istk(il1+2)=n
      istk(il1+3)=it1
      return
      end

