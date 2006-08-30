      subroutine intdgecon(fname)

c     rcond

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM
      double precision dlange
      external dlange
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA .ne. NA) then
         err=1
         call error(20)
         return
      endif
      N = MA

      if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 2
         return
      elseif(n.eq.-1) then
         if(.not.createvar(2,'d', 1, 1, lRCOND)) return
         stk(lRCOND)=1.0d0
         lhsvar(1) = 1
         return
      endif

      if(.not.createvar(2,'d', 1, 1, lRCOND)) return
      if(.not.createvar(3,'i', 1, N, lIPIV)) return
      if(.not.createvar(4,'i', 1, N, lIWORK)) return
      LWORKMIN =  4*N
      LWORK=maxvol(5,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(5,'d',1,LWORK,lDWORK)) return
      ANORM = dlange( '1', N, N, stk(lA), N, stk(lDWORK) )
c     DOUBLE PRECISION FUNCTION DLANGE( NORM, M, N, A, LDA, WORK )
      call DGETRF( N, N, stk(lA), N, istk(lIPIV), INFO )         
c     SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
      stk(lRCOND) = 0.0d0
      if(INFO.eq.0) then
         call DGECON( '1', N, stk(lA), N, ANORM, stk(lRCOND),
     $        stk(lDWORK), istk(lIWORK), INFO )
c     SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     IWORK, INFO )
      endif

      lhsvar(1) = 2
      return
c     
      end
