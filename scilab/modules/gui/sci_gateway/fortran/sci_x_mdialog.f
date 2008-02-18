c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scimdial(fname)
C     ================================================================
c     SCILAB function : x_dialog 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,3,4)) return
      topk=top
      if (rhs.eq.4) then 
         if (.not.getwsmat(fname,topk,top,m4,n4,il4,ild4)) return
         top  = top-1
      endif
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      if (m3.ne.1.and.n3.ne.1) then 
         buf= fname// ': third argument must be a vector'
         call error(999)
         return
      endif
      top = top-1
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      if ( rhs.eq.4) then 
         if ( m2*n2.ne.m4 ) then 
            buf= fname// ': incompatible second and fourth argument'
            call error(999)
            return
         endif
         if ( m3*n3.ne.n4 ) then 
            buf= fname// ': incompatible third and fourth argument'
            call error(999)
            return
         endif
      endif
      if ( rhs.eq.3) then 
         if ( m3*n3.ne.m2*n2 ) then 
            buf= fname// ': incompatible second and third argument'
            call error(999)
            return
         endif
      endif

C     (we can use top since the top objet is used before being 
C     scratched 
C     we MUST use ierr inside xdialg to check for space 
      ilres=iadr(lstk(top))
      if (rhs.eq.4 ) then 
         mres = m4
         nres = n4
         ilw=ilres+4+m4*n4+1
         ierr=iadr(lstk(bot))-ilw
         call xmatdg(istk(il1),istk(ild1),m1*n1,
     $        istk(il4),istk(ild4),
     $        istk(il2),istk(ild2),
     $        istk(il3),istk(ild3),mres,nres,
     $        istk(ilw),istk(ilres+4),ierr)
      else
         mres=m2*n2
         nres=1
         ilw=ilres+4+m3*n3+1
         ierr=iadr(lstk(bot))-ilw
         call xmdial(istk(il1),istk(ild1),m1*n1,
     $        istk(il3),istk(ild3),
     $        istk(il2),istk(ild2),mres,
     $        istk(ilw),istk(ilres+4),ierr)
      endif
      if(ierr.eq.3) then
         call error(113)
         return
      elseif(ierr.eq.2) then
         call error(17)
         return
      elseif(ierr.eq.1) then
         call error(112)
         return
      endif
      if(mres.eq.0) then
         istk(ilres)=1
         istk(ilres+1)=0
         istk(ilres+2)=0
         istk(ilres+3)=0
         lstk(top+1)=sadr(ilres+4)
      else
         istk(ilres)=10
         istk(ilres+1)=mres
         istk(ilres+2)=nres
         istk(ilres+3)=0
         lstk(top+1)=sadr(ilres+5+mres*nres+istk(ilres+4+nres*mres))
         return
      endif
      return
      end
C     ================================================================
