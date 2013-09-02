
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine matcmp
c     
c     == <= >= <>

      include 'stack.h'
c     
      integer top0,op
      double precision  e1,e2,e1r,e2r,e1i,e2i
      integer less,great,equal

      integer isanan
      integer iadr,sadr
      data less/59/,great/60/,equal/50/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      op=fin
      lw=lstk(top+1)+1
c     
      il2=iadr(lstk(top))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      mn2=m2*n2
      top=top-1
c     
      il1=iadr(lstk(top))
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1

      if(fin.eq.61) then
         fin=-fin
         top=top+1
         return
      endif
c     comparaisons
      if(m1.eq.-1) then
c     .  eye op b
         err=lw+mn2*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         if(m2.eq.-1) then
            m2=1
            n2=1
         elseif(mn2.gt.0) then
            call dset(mn2,0.0d0,stk(lw),1)
            call dset(min(m2,n2),stk(l1),stk(lw),m2+1)
            if(it1.eq.1) then
               call dset(mn2,0.0d0,stk(lw+mn2),1)
               call dset(min(m2,n2),stk(l1+1),stk(lw+mn2),m2+1)
            endif
            l1=lw
         endif
         m1=m2
         n1=n2
         mn1=mn2
         istk(il1+1)=m1
         istk(il1+2)=n1
      elseif(m2.eq.-1) then
         err=lw+mn1*(it2+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,0.0d0,stk(lw),1)
         call dset(min(m1,n1),stk(l2),stk(lw),m1+1)
         if(it1.eq.1) then
             call dset(mn1,0.0d0,stk(lw+mn1),1)
            call dset(min(m1,n1),stk(l2+1),stk(lw+mn1),m1+1)
         endif
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      elseif(mn1.eq.1.and.mn2.gt.1) then
         err=lw+mn2*(it1+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn2,stk(l1),stk(lw),1)
         if(it1.eq.1) call dset(mn2,stk(l1+1),stk(lw+mn2),1)
         l1=lw
         mn1=mn2
         m1=m2
         n1=n2
         istk(il1+1)=m1
         istk(il1+2)=n1
      elseif(mn2.eq.1.and.mn1.gt.1) then
         err=lw+mn1*(it2+1)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call dset(mn1,stk(l2),stk(lw),1)
         if(it1.eq.1) call dset(mn1,stk(l2+1),stk(lw+mn1),1)
         l2=lw
         mn2=mn1
         m2=m1
         n2=n1
      endif
      if(mn2.eq.0.or.mn1.eq.0) then
         if(op.eq.equal.or.op.eq.less+great) then
            itrue=0
            if(mn2.eq.0.and.mn1.eq.0) itrue=1
            if(op.eq.less+great) itrue=1-itrue
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=itrue
            lstk(top+1)=sadr(il1+4)
            return
         else
            if(mn1.eq.1.or.mn2.eq.1) then
               istk(il1)=1
               istk(il1+1)=0
               istk(il1+2)=0
               istk(il1+3)=0
               lstk(top+1)=sadr(il1+4)
               return
            else
               call error(60)
               return
            endif
         endif
      endif
      if(n1.ne.n2.or.m1.ne.m2) then
         if(op.eq.equal.or.op.eq.less+great) then
            istk(il1)=4
            istk(il1+1)=1
            istk(il1+2)=1
            istk(il1+3)=0
            if(op.eq.less+great) istk(il1+3)=1
            lstk(top+1)=sadr(il1+4)
         else
            call error(60)
            return
         endif
      else if(max(it1,it2).eq.1) then
         if(op.ne.equal.and.op.ne.less+great) then
            fin=-fin
            top=top0
            return
         endif
c         itrue=1
c         if(op.eq.less+great) itrue=0
         istk(il1)=4
         do 131 i=0,mn1-1
            e1r=stk(l1+i)
            e2r=stk(l2+i)
            e1i=0.0d+0
            e2i=0.0d+0
            if(it1.eq.1) e1i=stk(l1+mn1+i)
            if(it2.eq.1) e2i=stk(l2+mn2+i)
            call idcmp(e1r,e2r,1,ir,op)
            call idcmp(e1i,e2i,1,ii,op)
            if (op.eq.less+great) then
               if(ir.eq.1.or.ii.eq.1) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
            else
               if(ir.eq.1.and.ii.eq.1) then
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
            endif

 131     continue
         lstk(top+1)=sadr(il1+3+mn1)
      else
         istk(il1)=4
         if(mn1.eq.0) then
            if(op.ne.equal.and.op.ne.less+great) then
               call error(226)
               return
            else
               istk(il1+1)=1
               istk(il1+2)=1
               istk(il1+3)=1
               if(op.ne.equal) istk(il1+3)=0
               lstk(top+1)=sadr(il1+4)
            endif
            return
         endif
         if (.true.) then 
c           add explicit nan tests when requested 
            call idcmp(stk(l1),stk(l2),mn1,istk(il1+3),op)
         else
         do 132 i=0,mn1-1
            e1=stk(l1+i)
            e2=stk(l2+i)
c     for vc++ we add an explicit test for nan 
            if(isanan(e1).eq.1.and.isanan(e2).eq.1) then 
               if (op.eq.less+great) then 
                  istk(il1+3+i)=1
               else
                  istk(il1+3+i)=0
               endif
            elseif(  (op.eq.equal   .and. e1.eq.e2) .or.
     &           (op.eq.less+great    .and. e1.ne.e2) .or.
     &           (op.eq.less          .and. e1.lt.e2) .or.
     &           (op.eq.great         .and. e1.gt.e2) .or.
     &           (op.eq.(less+equal)  .and. e1.le.e2) .or.
     &           (op.eq.(great+equal) .and. e1.ge.e2) ) then
               istk(il1+3+i)=1
            else
               istk(il1+3+i)=0
            endif
 132     continue
         endif
         lstk(top+1)=sadr(il1+3+mn1)
      endif
      return
      end

c			================================================
