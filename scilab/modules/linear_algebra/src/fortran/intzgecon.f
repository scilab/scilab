      subroutine intzgecon(fname)

c     rcond

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM
      double precision zlange
      external zlange
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      
      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
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
      if(.not.createvar(4,'d', 1, 2*N, lRWORK)) return
      LWORKMIN =  2*N
      LWORK=maxvol(5,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(5,'z',1,LWORK,lDWORK)) return
      ANORM = zlange( '1', N, N, zstk(lA), N, zstk(lDWORK) )
c     DOUBLE PRECISION FUNCTION ZLANGE( NORM, M, N, A, LDA, WORK )
      call ZGETRF( N, N, zstk(lA), N, istk(lIPIV), INFO )         
c     SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
      stk(lRCOND) = 0.0d0
      if(INFO.eq.0) then
         call ZGECON( '1', N, zstk(lA), N, ANORM, stk(lRCOND),
     $        zstk(lDWORK),  stk(lRWORK), INFO )
c     SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     RWORK, INFO )
      endif

      lhsvar(1) = 2
      return
c     
      end
