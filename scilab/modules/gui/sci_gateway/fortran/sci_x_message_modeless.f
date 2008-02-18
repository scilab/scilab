c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

      subroutine scimess1(fname) 
C     ================================================================
c     SCILAB function : xmessage 
C     ================================================================
      character*(*) fname
cc      implicit undefined (a-z)
      include 'stack.h'
      integer topk,iadr,sadr
      integer m2,n2,il2,ild2,m1,n1,il1,ild1
      logical getwsmat,checkrhs,cresmat2
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      if (.not.checkrhs(fname,1,1)) return
      topk=top
C     creation of extra arguments
C     add button name
      if (rhs.lt.2) then 
         top  = top+1
         nlr2 = 2
         if(.not.cresmat2(fname,top,nlr2,lr2)) return
         call cvstr(nlr2,istk(lr2),'Ok',0)
      endif
C     second argument is a string vector
      if (.not.getwsmat(fname,topk,top,m2,n2,il2,ild2)) return
      if (m2.ne.1.or.n2.ne.1) then 
         buf= fname// ': Second argument must be a string'
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
      call xmsg1(istk(il1),istk(ild1),m1*n1,
     $     istk(il2),istk(ild2),m2*n2,ierr)
      if(ierr.eq.1) then 
         call error(112)
         return
      endif
      call objvide(fname,top)
      end
C     ================================================================
