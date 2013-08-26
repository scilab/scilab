c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txtc     -------------------------------
c
      subroutine intnearfl(id)

c     Interface for nearfloat :
c    
c         nearfloat("succ",x) -> succ of x
c         nearfloat("pred",x) -> pred of x
c
      implicit none

      INCLUDE 'stack.h'

      integer id(nsiz)

c     EXTERNAL FUNCTIONS
      double precision nearfloat
      external         nearfloat

c     EXTERNAL API FUNCTIONS
      logical  checkrhs, checklhs, getsmat, getrmat, cremat
      external checkrhs, checklhs, getsmat, getrmat, cremat

c     LOCAL VAR
      integer topk
      integer n, m, idxmat, mt, nt, lstr, nlstr, lm, lr, lc, i
      integer   lmax
      parameter(lmax = 4)
      character*(lmax) inputstring
      character*9 fname

c     TEXT
      fname = 'nearfloat'
      topk=top
      rhs=max(0,rhs)

      if (.not.checkrhs(fname,2,2)) return
      if (.not.checklhs(fname,1,1)) return

c     1/ get the adress of the matrix
      if( .not. getrmat(fname, topk, top, m, n, idxmat) ) return
      top = top - 1
c     2/ get the string
      if( .not. getsmat(fname,topk,top,mt,nt,1,1,lstr,nlstr)) return
c     pas de verif qu'il s'agit bien d'une matrice (1,1) ...
c     on recupere la chaine dans la variable inputstring
      lm = min(nlstr,lmax)
      call cvstr(lm,istk(lstr),inputstring,1)

c     3/ go on
      if (inputstring .eq. 'succ') then
         if (.not.cremat(fname,top,0,m,n,lr,lc)) return
         do i=0, m*n-1
            stk(lr+i) = nearfloat(stk(idxmat+i),1.d0)
         enddo

      elseif (inputstring .eq. 'pred') then
         if (.not.cremat(fname,top,0,m,n,lr,lc)) return
         do i=0, m*n-1
            stk(lr+i) = nearfloat(stk(idxmat+i),-1.d0)
         enddo

      else
         buf=fname//' : unknown string specifier (must be pred or succ)'
         call error(999)
         return
      endif

      end
c     -------------------------------
