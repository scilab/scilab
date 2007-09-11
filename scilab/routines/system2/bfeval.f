      subroutine bfeval(nn,x1,x2,xres,itype,ename)
c     ==========================================================
C     soft external for feval 
C     the result is real or complex according to itype value 
c     ==========================================================
c     Copyright INRIA
      include '../stack.h'
      integer sadr,iadr
      character*6 ename
      double precision x1,x2,xres(2)
      common / fevaladr / kfeval,kx1top,kx2top
      common / fevaltyp / itfeval
      logical allowptr

      data mlhs/1/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' bydot  top:'//buf(1:4))
      endif
c
      mrhs=nn
      if(itfeval.eq.10) then
         call ffeval(nn,x1,x2,xres,itype,ename)
         return
      endif
c     Putting Fortran arguments on Scilab stack 
c+    
      call ftob(x1,1,kx1top)
      if(err.gt.0.or.err1.gt.0) return
      if (nn.eq.2) then
         call ftob(x2,1,kx2top)
         if(err.gt.0.or.err1.gt.0) return
      endif
c+    
      if(itfeval.ne.15) then
         fin=lstk(kfeval)
      else
         ils=iadr(lstk(kfeval))
         nelt=istk(ils+1)
         l=sadr(ils+3+nelt)
         ils=ils+2
c     external adress 
         fin=l
c     Extra arguments in calling list that westore on the Scilab stack
         call extlarg(l,ils,nelt,mrhs)
         if(err.gt.0.or.err1.gt.0) return
      endif

c     Macro execution 
c     
      pt=pt+1
      if(pt.gt.psiz) then
         call  error(26)
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
      icall=5

      include '../callinter.h'
cxxx
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     transfert des variables (scilab) de sortie vers fortran
c     avec test du cas complexe
      il=iadr(lstk(top))
 
      if(istk(il).ne.1) then 
         call error(98)
         return
      endif
      itype=istk(il+3)
      call btof(xres,itype+1)
      if(err.gt.0.or.err1.gt.0) return
c     normal return
      return

c     the external produces an error
 9999 continue
      niv=niv-1
      if(err1.gt.0) then
c     .  the error has been catched
         lhs=ids(1,pt)
         rhs=ids(2,pt)
         pt=pt-1
         fun=0
      endif
      return

      end

