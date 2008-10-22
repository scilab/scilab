c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$
      subroutine intdpotrf(fname)

c     U = chol(A)

      include 'stack.h'
      logical getrhsvar
      logical checklhs,checkrhs
      character fname*(*)
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(m.ne.n) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         lhsvar(1) = 1
         return
      elseif(N.eq.-1) then
         if(stk(lA).le.0.0d0) then
            call error(29)
            return
         endif
         lhsvar(1) = 1
         stk(lA)=sqrt(stk(lA))
         return
      endif

      call DPOTRF( 'U', N, stk(lA), N, INFO )
c     SUBROUTINE DPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         if(info.gt.0)  call error(29)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               stk(lA+ij-1)=0.0d0    
 10         continue
 20      continue
      endif
      
      lhsvar(1)=1
      
c     
      end

