c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c     =======================================
      subroutine sciamell(fname)
c      
      include 'stack.h'
      character*(*) fname
      integer lw
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
            
      if(rhs.ne.2) then
         call error(39)
         return
      endif
c
      il=iadr(lstk(top))
      itv=istk(il)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      l1=sadr(il+4)
      if(stk(l1).lt.0.or.stk(l1).gt.1)then
         err=rhs
         call error(36)
         return
      endif
      il=iadr(lstk(top-1))
      itv=istk(il)
      m=istk(il+1)
      n=istk(il+2)
      length=m*n
      it=istk(il+3)
      l=sadr(il+4)
      lw=lstk(top+1)
      err=lw+length-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      call amell(stk(l),stk(l1),stk(lw),length)
      call unsfdcopy(length,stk(lw),1,stk(l),1)
      top=top-1
      lstk(top+1)=l+length
      
      return
      end
c     =======================================
      
