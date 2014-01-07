c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) Scilab Enterprises - 2013
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c
      subroutine bpjacd (res, ires, neq, t, y, ydot, rewt, savr,
     *                     wk, h, cj, wp, iwp, ier, rpar, ipar)
c
c ======================================================================
c     Management of external dealing with preconditioning of linear systems.
c ======================================================================
c
      INCLUDE 'stack.h'
      integer iadr,sadr
c
      logical allowptr
c
      character tmpbuf * (bsiz)
      double precision res(*), t, y(*), ydot(*), rewt(*), savr(*),
     *                  wk(*), h, cj, wp(*), rpar(*)
      integer ires, neq, iwp(*), ier, ipar(*)
      double precision dneq
      integer vol,tops,nordre,hsize
      data nordre/5/,mlhs/3/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
c

      if (ddt .eq. 4) then
         write(tmpbuf(1:12),'(3i4)') top,r,sym
         call basout(io,wte,' bpjacd  top:'//tmpbuf(1:4))
      endif
c     nordre is the external's order number in the data structure,
c     mlhs (mrhs) is the number of output (input) parameters
c     of the simulator
c
      mrhs=8
      ierror=0
c
      ilp=iadr(lstk(top))
      il=istk(ilp+nordre)
c
      tops=istk(il)
      ils=iadr(lstk(tops))
c
      if(istk(ils).eq.10) then
c     Fortran simulator case
         call fpjacd (res, ires, neq, t, y, ydot, rewt, savr,
     *                wk, h, cj, wp, iwp, ier, rpar, ipar)
         return
      endif
c     external is a Scilab function

c     On return ierror=1 is used to notify to the ode solver that
c     scilab was not able to evaluate the external
      ierror=1

c     Putting Fortran arguments on Scilab stack
c
c     Minimum entry arguments for the simulator. The value of these arguments
c     comes from the Fortran context (call list)
c+
c     function res
      dneq = neq
      call ftob(dneq,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(t,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(y,neq,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(ydot,neq,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(h,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(cj,1,istk(il+1))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(rewt,neq,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
      call ftob(savr,neq,istk(il+2))
      if(err.gt.0.or.err1.gt.0) return
c+
c
      if(istk(ils).eq.15) goto 10
c
c     Retrieving the simulator's address
      fin=lstk(tops)
c
      goto 40
c     If the simulator is defined by a list
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
c     Executing the macro defining the simulator
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
      call btof(ier,1)
      if(err.gt.0.or.err1.gt.0) return
c      call btof(iwp,2*neq*neq)
      il2=iadr(lstk(top))
      hsize=4
      n=istk(il2+1)*istk(il2+2)*(istk(il2+3)+1)
      liwp = 2*neq*neq
c     Test if the variable on the stack has same type and size as the theoretical iwp
      if (istk(il2).ne.1.or.n.ne.liwp) then
         call error(98)
         return
      endif
      lx=sadr(il2+hsize)
      do 900 i=1,liwp
         iwp(i) = stk(lx+i-1)
900   continue
      top = top-1
      if(err.gt.0.or.err1.gt.0) return
      lwp = neq*neq
      call btof(wp,lwp)
      if(err.gt.0.or.err1.gt.0) return
c+

c     Normal return ierror set to 0
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

