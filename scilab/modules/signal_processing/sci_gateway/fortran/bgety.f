c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine bgety(y, incr, istart)
c ======================================================================
c     macros or list externals for corr 
c ======================================================================


      INCLUDE 'stack.h'
      integer iadr,sadr
      double precision y(*)
      character*(nlgh+1) namex,namey
      common / corrname / namex,namey
      common / corradr  / kgxtop,kgytop,ksec,kisc
      common / corrtyp /  itxcorr,itycorr
      common/  iercorr /iero
      logical allowptr

      data mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     number of arguments of the external 
      mrhs=2

c     external is a Scilab function

c     on return iero=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      iero=1

c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(dble(incr),1,ksec)
       if(err.gt.0.or.err1.gt.0) return
      call ftob(dble(istart),1,kisc)
       if(err.gt.0.or.err1.gt.0) return
c+    
      if(itycorr.ne.15) then
         fin=lstk(kgytop)
      else
         ils=iadr(lstk(kgytop))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that we store on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
          if(err.gt.0.or.err1.gt.0) return
      endif
c     Macro execution 
      pt=pt+1
      if(pt.gt.psiz) then
         call error(26)
         return
      endif
      ids(1,pt)=lhs
      ids(2,pt)=rhs
      rstk(pt)=1001
      lhs=mlhs
      rhs=mrhs
      niv=niv+1
      fun=0
c     
c     
      icall=5

      include 'callinter.h.f'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfert des variables  de sortie vers fortran
      call btof(y,incr)
      if(err.gt.0.or.err1.gt.0) return
c     normal return iero set to 0
      iero=0
      return
c     
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return
      end


