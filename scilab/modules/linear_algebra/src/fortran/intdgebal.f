   
      subroutine intdgebal(fname)

c     [V,D]=balanc(A)
c     d = balanc(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
c      integer vfinite

      character fname*(*)
      character  JOB
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )

      minrhs=1
      maxrhs=1
      minlhs=2
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
         if(.not.createvar(2,'d', 0, 0, lD)) return
         lhsvar(1) = 1
         lhsvar(2) = 2
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif 

      if(.not.createvar(2,'d', N, N, lZ)) return
      if(.not.createvar(3,'d', 1, N, lSCALE)) return
      JOB='B'
c     SUBROUTINE DGEBAL( JOB, N, A, LDA, ILO, IHI, SCALE, INFO )
      call  DGEBAL(JOB, N, stk(lA), N, ILO, IHI,stk(lSCALE),INF0)

      call DLASET( 'F', N, N, ZERO, ONE, stk(lZ), N ) 
c     SUBROUTINE DGEBAK( JOB, SIDE, N, ILO, IHI, SCALE, M, V, LDV, INFO )
      call DGEBAK(JOB, 'R', N, ILO, IHI, stk(lSCALE), N,
     $     stk(lZ), N, INFO )
 
      lhsvar(1)=1
      lhsvar(2)=2
c     
      end
