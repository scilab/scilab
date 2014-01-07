c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c ...
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine bresd(t,y,ydot,res,ires,rpar,ipar)
c     
c ======================================================================
c     external "soft" management dealing with
c     ddassl residual calculation
c ======================================================================
c
      INCLUDE 'stack.h'
      integer iadr,sadr
c     
      logical allowptr
c      
      character tmpbuf * (bsiz) 
      double precision t, y(*),ydot(*),res(*),rpar(*)
      integer ires,ipar(*)
      integer vol,tops,nordre
      data nordre/1/,mlhs/2/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
c
      if (ddt .eq. 4) then
         write(tmpbuf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' bresd  top:'//tmpbuf(1:4))
      endif
c     nordre is the order number of that external in the
c     data structure,
c     mlhs (mrhs) is the number of output (input) parameters
c     of the simulator
c     
      mrhs=3
      ierror=0
c     
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)
c
      tops=istk(il)
      ils=iadr(lstk(tops))
c
      if(istk(ils).eq.10) then
c     Case of a Fortran simulator
         call fresd(t,y,ydot,res,ires,rpar,ipar)
         return
      endif
c     external is a Scilab function

c     On return ierror=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      ierror=1

c     Putting Fortran arguments on Scilab stack 
c     
c     Transferring the minimal input arguments of the simulator
c     their value comes from the Fortran context (call list)
c     the structure comes from the context
c+    
      neq=istk(il+1)
      call ftob(t,1,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(y,neq,istk(il+3))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(ydot,neq,istk(il+3))
      if(err.gt.0.or.err1.gt.0) return
c+    
c     
      if(istk(ils).eq.15) goto 10
c     
c     Retrieving the simulator's address
      fin=lstk(tops)
c     
      goto 40
c     Case when the simulator is described by a list
 10   nelt=istk(ils+1)
      l=sadr(ils+3+nelt)
      ils=ils+2
c     
c     Retrieving the simulator's address
      fin=l
c     
c     Managing the additional simulator parameters coming from
c     the context (elements of the list describing the simulator)
c     
      nelt=nelt-1
      if(nelt.ne.0) then
         l=l+istk(ils+1)-istk(ils)
         vol=istk(ils+nelt+1)-istk(ils+1)
         if(top+1+nelt.ge.bot) then
            call error(18)
            return
         endif
         err=lstk(top+1)+vol-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(vol,stk(l),1,stk(lstk(top+1)),1)
         do 11 i=1,nelt
            top=top+1
            lstk(top+1)=lstk(top)+istk(ils+i+1)-istk(ils+i)
 11      continue
         mrhs=mrhs+nelt
      endif
 40   continue
c     
c     Running the macro defining the simulator
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

      include 'callinter.h.f'
c     
 200  lhs=ids(1,pt)
      rhs=ids(2,pt)
      pt=pt-1
      niv=niv-1
c+    
c     Transferring the output to Fortran
      call btof(res,1)
      if(err.gt.0.or.err1.gt.0) return
      ires=res(1)
      call btof(res,neq)
      if(err.gt.0.or.err1.gt.0) return
c+    
c     normal return ierror set to 0
      ierror=0 
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

