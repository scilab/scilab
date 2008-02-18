c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scidialog(fname)
C     ================================================================
c     SCILAB function : x_dialog 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr,isize(2),sadr
      integer m3,n3,il3,ild3,nlr3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat1,cresmat2
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,1,2)) return
      topk=top
C     creation of extra arguments
C     size of 'OK' and 'Cancel'
      isize(1) = 2
      isize(2) = 6 
C     2nd argument if missing 
      if (rhs.lt.2) then 
         top  = top+1
         nlr2 = 1
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),' ',0)
      endif
C     we create ['OK';'Cancel'] 
      top = top+1 
      if (.not.cresmat1(fname,top,2,isize)) return
      call getsimat(fname,top,top,m3,n3,2,1,lr3,nlr3)
      call cvstr(isize(2),istk(lr3),'Cancel',0)
      call getsimat(fname,top,top,m3,n3,1,1,lr3,nlr3)
      call cvstr(isize(1),istk(lr3),'OK',0)
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top = top-1
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': Second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C    first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
C     using the whole stack from top to bottom 
C     to get back the result 
C     (we can use top since the top objet is used before being 
C     scratched 
C     we MUST use ierr inside xdialg to check for space 
      ilmes=iadr(lstk(top))
      nrep=200
      ilw=ilmes+4+nrep+1
      ierr=iadr(lstk(bot))-ilw
      call xdialg(istk(il2),istk(ild2),m2*n2,
     $     istk(il1),istk(ild1),m1*n1,
     $     istk(il3),istk(ild3),m3*n3,
     $     istk(ilw),istk(ilmes+4),nrep,ierr)
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
      if(nrep.eq.0) then
         istk(ilmes)=1
         istk(ilmes+1)=0
         istk(ilmes+2)=0
         istk(ilmes+3)=0
         lstk(top+1)=sadr(ilmes+4)
      else
         istk(ilmes)=10
         istk(ilmes+1)=nrep
         istk(ilmes+2)=1
         il = ilmes+5+nrep
         call icopy(istk(il-1)-1,istk(ilw),1,istk(il),1)
         lstk(top+1)=sadr(il +istk(il-1))
      endif
      return
      end
C     ================================================================
