      subroutine i_f_i
c     concatenation [a;b]
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
      lw=lstk(top+1)
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

 76   if(n1.lt.0.or.n2.lt.0) then
         call error(14)
         return
      elseif(n2.eq.0) then
c     .  [a;[]]
         return
      elseif(n1.eq.0)then
c     .  [[];b]
         call unsfdcopy(lstk(top+2)-lstk(top+1),stk(lstk(top+1))
     $        ,1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+lstk(top+2)-lstk(top+1)
         return
      elseif(n1.ne.n2) then
         call error(6)
         return
      endif
      m=m1+m2
      mn=m*n1

      lw1=max(iadr(lw),l1+memused(it1,mn)+1)
      lw2=lw1+memused(it1,mn1)+1
      err=sadr(lw2+memused(it1,mn2))-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call gencopy(it1,mn2,istk(l2),1,istk(lw2),1)
      call gencopy(it1,mn1,istk(l1),1,istk(lw1),1)

      call genconcatcol(it1,istk(lw1),m1,n1,istk(lw2),m2,n2,istk(l1))
      lstk(top+1)=sadr(il1+4+memused(it1,mn))

      istk(il1+1)=m
      istk(il1+2)=n1
      istk(il1+3)=it1

 999  return
      end
