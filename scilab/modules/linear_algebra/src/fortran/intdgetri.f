      subroutine intdgetri(fname)

c     B = inv(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision anorm,rcond
      double precision dlange,dlamch
      external dlange,dlamch

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
      if(n.eq.0) then
         lhsvar(1) = 1
         return
      elseif(n.eq.-1) then
         stk(lA)=1.0d0/stk(lA)
         lhsvar(1) = 1
         return
      endif
      if(.not.createvar(2,'i',1,N,lIPIV)) return
      if(.not.createvar(3,'i',1,N,lIWORK)) return
      LWORKMIN = MAX(1,N)
      LWORK=maxvol(4,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(4,'d',1,LWORK,lDWORK)) return

      ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )

      call DGETRF( N, N, stk(lA), N, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.gt.0) then
         call error(19)
         return
      elseif (info.lt.0) then
         return
      endif
      RCOND = 0.0d0
      call DGECON( '1', N, stk(lA), N, ANORM, RCOND, stk(lDWORK),
     $     istk(lIWORK), INFO )
      if(RCOND.le.sqrt(dlamch('eps'))) then
         call writebufdgetri(buf,RCOND)
c     .  ill conditionned problem
         call msgs(5,0)
      endif


      call DGETRI( N, stk(lA), N, istk(lIPIV), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end
