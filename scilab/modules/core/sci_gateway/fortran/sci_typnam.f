c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine inttypnam(fname)
c     --------------------------
      character*(*) fname
      logical checkrhs,checklhs
      include 'stack.h'
      logical getscalar
c     following common defines the initial database of type names
      integer maxtyp,nmmax,ptmax
      parameter (maxtyp=50,nmmax=200)
      integer tp(maxtyp),ptr(maxtyp),ln(maxtyp),namrec(nmmax)
      common /typnams/ tp,ptr,ln,namrec,ptmax
C
      integer iadr,sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
C
      nbvars = 0
      rhs = max(0,rhs)
      
      if(rhs.eq.0) then
         if(.not.checklhs(fname,1,2)) return
c     compute number of defined types
         nt=0
         do 01 it=1,maxtyp
            if(ln(it).ne.0) nt=nt+1
 01      continue
c     allocate results
         top=top+1
c     .  vector of type numbers
         il=iadr(lstk(top))
         l=sadr(il+4)
         iln=iadr(l+nt)
         lw1=sadr(iln+5+nt+ptmax)
         err=lw1-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
c
         istk(il)=1
         istk(il+1)=nt
         istk(il+2)=1
         istk(il+3)=0
         lstk(top+1)=l+nt
c
         top=top+1
c     .  vector of type names
         iln=iadr(lstk(top))
         istk(iln)=10
         istk(iln+1)=nt
         istk(iln+2)=1
         istk(iln+3)=0
         istk(iln+4)=1
         ilc=iln+5+nt

         i1=0

         do 02 it=1,maxtyp
            if(ln(it).ne.0) then
               stk(l+i1)=tp(it)
               istk(iln+5+i1)=istk(iln+4+i1)+ln(it)
               call icopy(ln(it),namrec(ptr(it)),1,istk(ilc),1)
               ilc=ilc+ln(it)
               i1=i1+1
            endif
 02      continue
         lstk(top+1)=sadr(ilc)
         if(lhs.eq.1) top=top-1
         return
      endif
c
      if(.not.checkrhs(fname,0,2)) return
      if(.not.checklhs(fname,0,1)) return
      if(.not.getscalar(fname,top,top,lr)) return
      itype=stk(lr)
      if(itype.le.0) then
         err=1
         call error(116)
         return
      endif
      top=top-1
      il=iadr(lstk(top))
      if(istk(il).ne.10) then
         err=1
         call error(55)
         return
      endif
      if(istk(il+1).ne.1.or.istk(il+2).ne.1) then 
         err=1
         call error(60)
         return
      endif
      n=istk(il+5)-1
      call cvstr(n,istk(il+6),buf,1)

      call addtypename(itype,buf(1:n),ierr)
      if(ierr.eq.1) then
         call error(224)
         return
      elseif(ierr.eq.2) then
         call error(225)
         return
      elseif(ierr.eq.3) then
         call error(224)
         return
      endif
      istk(il)=0
      return
      end
