c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt 
      subroutine interror
      include 'stack.h'
      logical checklhs,checkrhs,getscalar,getsmat
      integer topk,errn
      integer iadr
c 
      iadr(l)=l+l-1
c
      rhs=max(0,rhs)
c
      if(.not.checkrhs('error',1,2)) return
      if(.not.checklhs('error',1,1)) return

      topk=top

      il=iadr(lstk(top+1-rhs))
      if(istk(il).eq.1) then
c     .  error(errn [,numero_d'argument])
         if(rhs.eq.2) then
c     .     get arg number if given
            if(.not.getscalar('error',topk,top,l)) return
            nb=int(stk(l))
            top=top-1
         else
            nb=0
         endif
c     .  get error number 
         if(.not.getscalar('error',topk,top,l)) return
         errn=int(stk(l))
         err=nb
      else
c     .  error(str [,errn])
         if(rhs.eq.2) then
c     .  get error number  if given
            if(.not.getscalar('error',topk,top,l)) return
            errn=int(stk(l))
            top=top-1
         else
c Bug 1421 corrected - Francois VOGEL June 2006
            errn=10000
         endif
         if(.not.getsmat('error',topk,top,m,n,1,1,il,n1)) return
         buf=' '
         call cvstr(n1,istk(il),buf,1)
      endif

      if(errn.le.0) then
         err=2
         call error(116)
         return
      endif
      call error(errn)
      return
      end
      
