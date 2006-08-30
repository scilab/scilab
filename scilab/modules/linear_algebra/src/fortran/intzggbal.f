      subroutine intzggbal(fname)
c     [Ab,Bb,X,Y]=balanc(A,B)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
c     integer vfinite
      character JOB

      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )

      minrhs=2
      maxrhs=2
      minlhs=4
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'z', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'z', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.NB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         if(.not.createvar(3,'d', N, N, lX)) return
         if(.not.createvar(4,'d', N, N, lY)) return
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=3
         lhsvar(4)=4
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(3,'d', N, N, lX)) return
      if(.not.createvar(4,'d', N, N, lY)) return
      if(.not.createvar(5,'d', 1, N, lLSCALE)) return
      if(.not.createvar(6,'d', 1, N, lRSCALE)) return
      if(.not.createvar(7,'d', 1, N*6, lWORK)) return


c     SUBROUTINE ZGGBAL( JOB, N, A, LDA, B, LDB, ILO, IHI, LSCALE,
c     $                   RSCALE, WORK, INFO )
      JOB='B'
      call ZGGBAL(JOB, N, zstk(lA), N, zstk(lB), N, ILO, IHI, 
     $     stk(lLSCALE), stk(lRSCALE), stk(lWORK), INFO )

c     
      call DLASET( 'F', N, N, ZERO, ONE, stk(lX), N ) 
      call DLASET( 'F', N, N, ZERO, ONE, stk(lY), N ) 

      call DGGBAK(JOB, 'L', N, ILO, IHI, stk(lLSCALE), stk(lRSCALE), N,
     $     stk(lX), N, INFO )
      call DGGBAK(JOB, 'R', N, ILO, IHI, stk(lLSCALE), stk(lRSCALE), N,
     $     stk(lY), N, INFO )


      lhsvar(1)=1
      lhsvar(2)=2
      lhsvar(3)=3
      lhsvar(4)=4
c     
      end
