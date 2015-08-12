c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$

      subroutine intddet(fname)

c     det(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE, TEN
      double precision D, E
      parameter ( ZERO = 0.0D0, ONE = 1.0D0, TEN = 10.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=2
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
      if(M.ne.N) then
         err=1
         call error(20)
         return
      endif
      if(N.eq.0) then
         lhsvar(1) = 2
         if(.not.createvar(2,'d',1,1,lL)) return
         stk(lL)=1.0d0
         if (lhs.eq.2) then
            if(.not.createvar(3,'d',1,1,lL)) return
            stk(lL)=0.0d0
            lhsvar(2) = 3
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif

      if(.not.createvar(2,'i',1,M,lIPIV)) return


      call DGETRF( M, N, stk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.lt.0) then
         return
      endif
      if (lhs.eq.1) then
         D=one
         do 10 i=0,N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D=D*stk(lA+i*(M+1))
 10      continue
         if(.not.createvar(1,'d',1,1,lD)) return
         stk(lD)=D
         lhsvar(1)=1
      else 
c     .  [e,m]=det(A)
         D = ONE
         E = ZERO
         do 40 i = 0, N-1
            if(istk(lIPIV+i).ne.i+1 ) D=-D
            D = D*stk(lA+i*(M+1))
            if (D .eq. ZERO) go to 50
 15         if (abs(D) .ge. ONE) go to 20
            D = ten*D
            E = E - ONE
            go to 15
 20         continue
 25         if (abs(D) .lt. ten) go to 30
            D = D/ten
            E = E + ONE
            go to 25
 30         continue
 40      continue
 50      continue
         if(.not.createvar(1,'d',1,1,le)) return
         stk(le) = E
         if(.not.createvar(2,'d',1,1,lm)) return
         stk(lm) = D
         lhsvar(1) = 1
         lhsvar(2) = 2
      endif
c     
      return     
      end

