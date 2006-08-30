c			================================================
c     Copyright INRIA
c			================================================

      subroutine matlog
c     
c     & | ~
c     
c     Copyright INRIA
      include '../stack.h'
c     
      integer top0,op
      double precision  e1,e2
      integer et,ou,non
      integer iadr,sadr
      data ou/57/,et/58/,non/61/

c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      op=fin
      lw=lstk(top+1)+1
c     
      if(rhs.eq.2) then
         il2=iadr(lstk(top))
         if(istk(il2).lt.0) il2=iadr(istk(il2+1))
         m2=istk(il2+1)
         n2=istk(il2+2)
         it2=istk(il2+3)
         l2=sadr(il2+4)
         mn2=m2*n2
         top=top-1
      endif
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

      if(fin.eq.non) then
         if(mn1.eq.0) then
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         else
            istk(il1)=4
            do 10 i=0,mn1-1
               e1=stk(l1+i)
               if(e1.eq.0.0d0) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
 10         continue
            lstk(top+1)=sadr(il1+3+mn1)
         endif
      else
         if(mn1.eq.0.or.mn2.eq.0) then
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
            return
         endif
         if(mn1.eq.1) then
            i1=0
            mn1=mn2
         else
            i1=1
         endif
         if(mn2.eq.1) then
            i2=0
            mn2=mn1
         else
            i2=1
         endif
         if(mn1.ne.mn2) then
            call error(60)
            return
         endif
         if(fin.eq.ou) then
            do 30 i=0,mn1-1
               e1=stk(l1+i*i1)
               e2=stk(l2+i*i2)
               if(e1.ne.0.0d0.or.e2.ne.0.0d0) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
 30         continue
         else
            do 31 i=0,mn1-1
               e1=stk(l1+i*i1)
               e2=stk(l2+i*i2)
               if(e1.ne.0.0d0.and.e2.ne.0.0d0) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
 31         continue
         endif
         istk(il1)=4
         istk(il1+1)=max(m1,m2)
         istk(il1+2)=max(n1,n2)
         lstk(top+1)=sadr(il1+3+mn1)
         return
      endif
      end
c			================================================
