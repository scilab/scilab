			subroutine intdgees0(fname)

c     [VS,T]=schur(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs 

      character fname*(*)
      character*4 JOBVS, SORT
      integer SDIM
      logical voiddummy
      external voiddummy

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
            if(.not.createvar(2,'d', N, N, lV)) return
            lhsvar(1) = 2
            lhsvar(2) = 1
            return
         endif
      endif

      if(.not.createvar(2,'d', N, 1, lWR)) return
      if(.not.createvar(3,'d', N, 1, lWI)) return
      k = 4
      if(lhs.gt.1) then
         if(.not.createvar(4,'d', N, N, lVS)) return
         k = 5
      endif
      if(.not.createvar(k,'i', N, 1, lBWORK)) return
      LWORKMIN = 3*N
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return


      if(lhs.eq.1) then
         JOBVS = 'N'
      else
         JOBVS = 'V'
      endif 
      SORT = 'N'

      call DGEES( JOBVS, SORT, voiddummy, N, stk(lA), N, SDIM,
     $     stk(lWR), stk(lWI), stk(lVS), N, stk(lDWORK), LWORK,
     $     istk(lBWORK), INFO )
c     SUBROUTINE DGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, WR, WI,
c     $    VS, LDVS, WORK, LWORK, INFO )
      if(info.gt.0) then
         call msgs(2,info)
      endif

      if(lhs.eq.1) then
         lhsvar(1) = 1
      else
         lhsvar(1)=4
         lhsvar(2)=1
      endif
c     
      end


