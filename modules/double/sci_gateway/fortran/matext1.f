
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      
      subroutine matext1
c     
c     B=A(i)

      include 'stack.h'
c     
      integer iadr,sadr
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
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

c     arg2(arg1)
      if (istk(il1).eq.0) then
         call error(220)
         return
      endif
      if(istk(il2).eq.129) then
c     implied polynomials vector extraction
         top=top+1
         call polops
         return
      endif

      if(mn2.eq.0) then 
c     .  arg2=[]
         if(m1.le.0) then
c            arg2(:) or arg2([])
            il1=iadr(lstk(top))
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         else
c     should be an error but a lot of code uses this feature
c     we will change it just after 3.1 release  see also matext2
c            call error(21)
            il1=iadr(lstk(top))
            istk(il1)=1
            istk(il1+1)=0
            istk(il1+2)=0
            istk(il1+3)=0
            lstk(top+1)=sadr(il1+4)
         endif
         return

      elseif(m2.lt.0) then
c     .  arg2=eye
         call error(14)
         return
      elseif(m1.lt.0) then
c     .  arg2(:), just reshape to column vector
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=mn2
         istk(il1+2)=1
         istk(il1+3)=istk(il2+3)
         l1=sadr(il1+4)
         call unsfdcopy(mn2*(it2+1),stk(l2),1,stk(l1),1)
         lstk(top+1)=l1+mn2*(it2+1)
         return
      endif
c     check and convert indices variable
      call indxg(il1,mn2,ilr,mi,mx,lw,1)
      if(err.gt.0) return
      if(mx.gt.mn2) then
         call error(21)
         return
      endif
 79   if(mi.eq.0) then
c     arg2([])
         il1=iadr(lstk(top))
         istk(il1)=1
         istk(il1+1)=0
         istk(il1+2)=0
         istk(il1+3)=0
         l1=sadr(il1+4)
         lstk(top+1)=l1
         return
      endif
c     get memory for the result
      il1=iadr(lstk(top))
      l1=sadr(il1+4)
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
      istk(il1)=1
      istk(il1+1)=m
      istk(il1+2)=n
      istk(il1+3)=it2
      if(lr.ne.l1) call unsfdcopy(mi*(it2+1),stk(lr),1,stk(l1),1)
      lstk(top+1)=l1+mi*(it2+1)
      return
      end
c			================================================
