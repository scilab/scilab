
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

    	subroutine matins1
c     
c     A(i)=B

      include 'stack.h'
      common /mtlbc/ mmode
c     
      logical isany
      integer top0
      integer iadr,sadr
      double precision xr,xi
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      top0=top
      lw=lstk(top+1)+1

      il3=iadr(lstk(top))
      if(istk(il3).lt.0) il3=iadr(istk(il3+1))
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      mn3=m3*n3
      top=top-1
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
      ilrs=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if (istk(il1).eq.10.or.istk(il1).eq.15) then
         top=top0
         fin=-fin
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      it1=istk(il1+3)
      l1=sadr(il1+4)
      mn1=m1*n1
c     arg3(arg1)=arg2
c     
      if (istk(il2)*istk(il1).eq.0) then
         call error(220)
         return
      endif
      if (m2.eq.0) then
c     .  arg3(arg1)=[] -->[]
         if(m1.eq.-1) then
c     .    arg3(:)=[] 
            istk(ilrs)=1
            istk(ilrs+1)=0
            istk(ilrs+2)=0
            istk(ilrs+3)=0
            lstk(top+1)=sadr(ilrs+4)
            return
         elseif(m1.eq.0) then
c     .     arg3([])=[]  --> arg3
            call icopy(4,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
            return
         else
c     .     arg3(arg1)=[]
            if(istk(il1).eq.4.and.m3.eq.m1.and.n3.eq.n1) then
               if(.not.isany(il1)) then
c     .           arg3([])=[]  --> arg3
                  call icopy(4,istk(il3),1,istk(ilrs),1)
                  l=sadr(ilrs+4)
                  call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
                  lstk(top+1)=l+mn3*(it3+1)
                  return
               endif
            endif
c     .     arg3(arg1)=[] -->arg3(compl(arg1))
 97         call indxgc(il1,mn3,ilr,mi,mx,lw)
            if(err.gt.0) return
            l2=l3
            n2=n3
            m2=m3
            mn2=m2*n2
            it2=it3
c     .     call extraction
            goto 79
         endif
      elseif(m2.lt.0.or.m3.lt.0) then
c     .  arg3=eye,arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg3(:)=arg2
         if(mn2.eq.mn3) then
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
            return
         elseif(mn3.eq.0) then
            istk(ilrs)=1
            istk(ilrs+1)=m2
            istk(ilrs+2)=n2
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            call unsfdcopy((it2+1)*mn2,stk(l2),1,stk(l1),1)
            lstk(top+1)=l1+mn2*(it2+1)
            return
         elseif(mn2.eq.1) then
            istk(ilrs)=1
            istk(ilrs+1)=m3
            istk(ilrs+2)=n3
            istk(ilrs+3)=it2
            l1=sadr(ilrs+4)
            if (it2.eq.1) then
               xr=stk(l2)
               xi=stk(l2+1)
               call dset(mn3,xr,stk(l1),1)
               call dset(mn3,xi,stk(l1+mn3),1)
            else
               call dset(mn3,stk(l2),stk(l1),1)
            endif
            lstk(top+1)=l1+mn3*(it2+1)
            return
         else
            call error(15)
            return
         endif
      endif
      call indxg(il1,mn3,ili,mi,mxi,lw,1)
      if(err.gt.0) return
      if(mi.eq.0) then
c     .  arg3([])=arg2
         if(mn2.eq.1) then
c     .  arg3([])=c  --> arg3
            call icopy(4,istk(il3),1,istk(ilrs),1)
            l=sadr(ilrs+4)
            call unsfdcopy(mn3*(it3+1),stk(l3),1,stk(l),1)
            lstk(top+1)=l+mn3*(it3+1)
            return
         else
            call error(15)
            return
         endif  
      endif
      inc2=1
      if(mi.ne.mn2) then
         if(mn2.eq.1) then
            inc2=0
         else
            call error(15)
            return
         endif
      endif
c     
      if (n3.gt.1.and.m3.gt.1) then
c     .  arg3 is not a vector
         if(n2.gt.1.and.m2.gt.1) then
            call error(15)
            return
         endif
         if(mxi.gt.m3*n3) then
            call error(21)
            return
         endif
         mr=m3
         nr=n3

c     commented lines for matlab compatibility
c      elseif (n3.le.1.and.m3.le.1) then
c         if(n2.le.1) then
c            mr=max(m3,mxi)
c            nr=max(n3,1)
c         else
c            nr=max(n3,mxi)
c            mr=max(m3,1)
c         endif
c      elseif (n3.le.1) then
c     .  arg3 and arg2 are  column vectors
c         mr=max(m3,mxi)
c         nr=max(n3,1)
c      elseif (m3.le.1) then
c     .  row vectors
c         nr=max(n3,mxi)
c         mr=max(m3,1)
      elseif (n3.le.1.and.n2.le.1) then
c     .  arg3 and arg2 are  column vectors
         mr=max(m3,mxi)
         nr=max(n3,1)
      elseif (m3.le.1.and.m2.le.1) then
c     .  row vectors
         nr=max(n3,mxi)
         mr=max(m3,1)
      else
c     .  arg3 and arg2 dimensions dont agree
         call error(15)
         return
      endif

      lr=l3
      mnr=mr*nr
      itr=max(it2,it3)
      if(mnr*(itr+1).ne.mn3*(it3+1) ) then
c     .  resulting matrix is bigger than original
         lr=lw
         lw=lr + mnr*(itr+1)
         err = lw - lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
c     .  initialise result r to 0
         call dset(mnr*(itr+1),0.0d+0,stk(lr),1)
c     .  write arg3 in r
         if(mn3.ge.1) then
            call dmcopy(stk(l3),m3,stk(lr),mr,m3,n3)
            if(it3.eq.1) then
               call dmcopy(stk(l3+mn3),m3,stk(lr+mnr),mr,m3,n3)
            endif
         endif
      endif
c     write arg2 in r
      do 98 i = 0, mi-1
         ll = lr+istk(ili+i) - 1
         ls = l2+i*inc2
         stk(ll) = stk(ls)
         if(it2.eq.1) then
            stk(ll+mnr)=stk(ls+mn2)
         elseif(itr.eq.1) then
            stk(ll+mnr)=0.0d0
         endif
 98   continue
c     
      if(lr.ne.l3) then
         l1=sadr(ilrs+4)
         call unsfdcopy(mnr*(itr+1),stk(lr),1,stk(l1),1)
         istk(ilrs)=1
         if(mmode.eq.1.and.nr.eq.1.and.m3.eq.0) then
         istk(ilrs+1)=nr
         istk(ilrs+2)=mr
         else
         istk(ilrs+1)=mr
         istk(ilrs+2)=nr
         endif
         istk(ilrs+3)=itr
         lstk(top+1)=l1+mnr*(itr+1)
      else
c     la matrice a ete modifie sur place 
         k=istk(iadr(lstk(top0))+2)
         top=top-1
         call setref(k)
      endif
      return
c     inline extraction procedure copied from matext1
 79   if(mi.eq.0) then
c     arg2([])
         istk(ilrs)=1
         istk(ilrs+1)=0
         istk(ilrs+2)=0
         istk(ilrs+3)=0
         l1=sadr(ilrs+4)
         lstk(top+1)=l1
         return
      endif
c     get memory for the result
      l1=sadr(ilrs+4)
      if(sadr(ilr-1).le.l1+(it2+1)*mi) then
         lr=lw
         lw=lr+(it2+1)*mi
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
      else
         lr=l1
      endif
c     perform extraction
      do 81 i = 0, mi-1
         ind=istk(ilr+i)-1
         stk(lr+i) = stk(l2+ind)
         if(it2.eq.1) stk(lr+mi+i) = stk(l2+mn2+ind)
 81   continue
c     set output sizes
      if (m2.eq.1.and.n2.eq.1.and.m1.gt.0) then
         m = m1
         n = min(n1,mi)
      elseif (m2 .gt. 1.or.m1.lt.0) then
         m = mi
         n = 1
      else
         n = mi
         m = 1
      endif
c     form resulting variable
      istk(ilrs)=1
      istk(ilrs+1)=m
      istk(ilrs+2)=n
      istk(ilrs+3)=it2
      if(lr.ne.l1) call unsfdcopy(mi*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi*(it2+1)
      return

      end
c			================================================
