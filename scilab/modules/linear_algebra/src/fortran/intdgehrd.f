c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$

			subroutine intdgehrd(fname)

c     [H,Q]=hess(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ZERO
      parameter ( ZERO=0.0D0 )

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
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lQ)) return
            lhsvar(1)=1
            lhsvar(2)=2
            return
         endif 
      elseif(N.eq.-1) then
        if(lhs.eq.1) then
           lhsvar(1) = 1
        else if(lhs.eq.2) then
           if(.not.createvar(2,'d', N, N, lQ)) return
           stk(lQ)=stk(lA)
           stk(lA)=1.0d0
           lhsvar(1)=1
           lhsvar(2)=2
        endif 
        return
      endif
      if(.not.createvar(2,'d', N-1, 1, lTAU)) return
      k = 3
      if(lhs.gt.1) then
         if(.not.createvar(3,'d', N, N, lQ)) return
         k = 4
      endif
      LWORKMIN = N
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return
      call DGEHRD( N, 1, N, stk(lA), N, stk(lTAU), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGEHRD( N, 1, N, A, LDA, TAU, WORK, INFO )

      if(lhs.gt.1) call DLACPY( 'F', N, N, stk(lA), N, stk(lQ), N )
c     SUBROUTINE DLACPY( UPLO, N, N, A, LDA, B, LDB )
      if(N.gt.2) then
         do 20 j = 1, N-2
            do 10 i = j+2, N
               ij = i+(j-1)*N
               stk(lA+ij-1) = ZERO
 10         continue
 20      continue
      endif
      
      if(lhs.gt.1) then
         call DORGHR( N, 1, N, stk(lQ), N, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE DORGHR( N, 1, N, A, LDA, TAU, WORK, INFO ) 
      endif

      if(lhs.eq.1) then
         lhsvar(1) = 1
      else
         lhsvar(1)=3
         lhsvar(2)=1
      endif 
c     
      end

