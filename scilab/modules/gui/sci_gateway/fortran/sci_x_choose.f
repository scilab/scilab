c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scichoose(fname) 
C     ================================================================
c     SCILAB function : x_choose
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr,sadr
      integer m3,n3,il3,ild3,m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat2
      logical cremat
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,2,3)) return
      topk=top
      if(rhs.eq.2) then
C     Create button list ( just Cancel in fact ) 
         top  = top+1
         nlr2 = 6
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),'Cancel',0)
      endif
      if (.not.getwsmat(fname,topk,top,m3,n3,il3,ild3)) return
      top = top -1 
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.and.n2.ne.1) then 
         buf= fname// ': Second argument must be a vector'
         call error(999)
         return
      endif
      top=top-1
C     first argument is a string vector 
      if (.not.getwsmat(fname,topk,top,m1,n1,il1,ild1)) return
      if (m1.ne.1.and.n1.ne.1) then 
         buf= fname// ': First argument must be a vector'
         call error(999)
         return
      endif
      call xchoose(istk(il2),istk(ild2),m2*n2,
     $     istk(il1),istk(ild1),m1*n1,
     $     istk(il3),istk(ild3),m3*n3,nrep,ierr)
      if(ierr.eq.1) then 
         call error(112)
         return
      endif
      if (.not.cremat(fname,top,0,1,1,lr1,lc)) return
      stk(lr1)= nrep
      return
      end
C     ================================================================
