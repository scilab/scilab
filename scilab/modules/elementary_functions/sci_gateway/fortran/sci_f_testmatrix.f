c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine inttestmatrix(id)
      INCLUDE 'stack.h'
      integer id(nsiz)

      integer tops,top2
      integer magi,frk,hilb
      integer iadr,sadr
      data magi/22/,frk/15/,hilb/17/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      tops=top

      if (lhs .ne. 1) then
         call error(41)
         return
      endif
      if (rhs .ne. 2) then
         call error(42)
         return
      endif

      top2=top-rhs+1
      il2=iadr(lstk(top2))
      if(istk(il2).lt.0) il2=iadr(istk(il2+1))
      if(istk(il2).ne.10) then
         err=1
         call error(55)
         return
      endif
      kletr=abs(istk(il2+5+istk(il2+1)*istk(il2+2)))

      il=iadr(lstk(top))
      if(istk(il).lt.0) il=iadr(istk(il+1))
      l=sadr(il+4)
      n=max(int(stk(l)),0)
c
      top=top-1
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=n
      istk(il+2)=n
      istk(il+3)=0
      l=sadr(il+4)
      lstk(top+1)=l+n*n
      err=lstk(top+1)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      if(kletr.eq.magi) goto 131
      if(kletr.eq.hilb) goto 132
      if(kletr.eq.frk) goto 133
c
c     carre magique
  131 if (n .eq. 2) n = 0
      if (n .gt. 0) call magic(stk(l),n,n)
      istk(il+1)=n
      istk(il+2)=n
      lstk(top+1)=l+n*n
      go to 999
c
c     hilbert
 132  call hilber(stk(l),n,n)
      go to 999
c
c matrice de franck
 133  continue
      job=0
      if (n .gt. 0) call franck(stk(l),n,n,job)
      go to 999
 999  return
      end
c     -------------------------------
