c     =======================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     =======================================
      subroutine sciremez(fname)
c      
      include 'stack.h'
      character*(*) fname
      integer lw,lo
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      
      il1=iadr(lstk(top))
      itv1=istk(il1)
      m1=istk(il1+1)
      n1=istk(il1+2)
      ngrid=m1*n1
      it1=istk(il1+3)
      l1=sadr(il1+4)
      call simple(ngrid,stk(l1),stk(l1))
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      m2=istk(il2+1)
      n2=istk(il2+2)
      it2=istk(il2+3)
      l2=sadr(il2+4)
      call simple(ngrid,stk(l2),stk(l2))
      il3=iadr(lstk(top-2))
      itv3=istk(il3)
      m3=istk(il3+1)
      n3=istk(il3+2)
      it3=istk(il3+3)
      l3=sadr(il3+4)
      call simple(ngrid,stk(l3),stk(l3))
      il4=iadr(lstk(top-3))
      itv4=istk(il4)
      m4=istk(il4+1)
      n4=istk(il4+2)
      nc=m4*n4-2
      it4=istk(il4+3)
      l4=sadr(il4+4)
      ir4=il4+4
      call entier(nc+2,stk(l4),stk(l4))
      lw=lstk(top+1)
      err=lw+7*(nc+2)-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      lw1=lw+nc+2
      lw2=lw1+nc+2
      lw3=lw2+nc+2
      lw4=lw3+nc+2
      lw5=lw4+nc+2
      lw6=lw5+nc+2
      call remez(ngrid,nc,stk(l4),stk(lw1),stk(lw2),stk(lw3),
     *     stk(l3),stk(l2),stk(l1),stk(lw4),
     *     stk(lw5),stk(lw6),stk(lw))
      top=top-3
      ilo=iadr(lstk(top))
      istk(ilo)=1
      istk(ilo+1)=1
      istk(ilo+2)=nc+1
      istk(ilo+3)=0
      lo=sadr(ilo+4)
      call unsfdcopy(nc+1,stk(lw),1,stk(lo),1)
      lstk(top+1)=lo+nc+1
      return

      end
c     =======================================
      