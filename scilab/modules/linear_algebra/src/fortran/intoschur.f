      subroutine intoschur(fname)

c     [VS,dim]=schur(A,function)
c     [VS,dim,T]=oschur(A,function)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4  JOBVS, SORT
      logical SCISCHUR,scichk
      external SCISCHUR,scichk
      common /scisch/ lf, nx, nf

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=3
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
            if(.not.createvar(2,'d',1,1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(1)=1
            lhsvar(2)=2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return 
            stk(lSDIM)=0.0d0
            if(.not.createvar(3,'d', N, N, lVS)) return
            lhsvar(1)=1
            lhsvar(2)=2
            lhsvar(3)=3
            return
         endif
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(2,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.1) then
         err=2
         call error(80)
         return
      endif
      if(.not.createvar(3,'d', N, 1, lWR)) return
      if(.not.createvar(4,'d', N, 1, lWI)) return
      k = 5
      if(lhs.gt.1) then
         if(.not.createvar(5,'d', N, N, lVS)) return
         k = 6
      endif
      if(.not.createvar(k,'i', 1, 1, lSDIM)) return 
      if(.not.createvar(k+1,'i', N, 1, lBWORK)) return
      LWORKMIN = 3*N
      LWORK = LWORKMIN
      if(.not.createvar(k+2,'d',1,LWORKMIN,lDWORK)) return
      nx=k+3

      if(lhs.eq.1) then
         JOBVS = 'N'
         lVS = lDWORK
      else
         JOBVS = 'V'
      endif 
      SORT = 'S'
      if(.not.scichk()) return
      call DGEES( JOBVS, SORT, SCISCHUR, N, stk(lA), N, istk(lSDIM),
     $     stk(lWR), stk(lWI), stk(lVS), N, stk(lDWORK), LWORK,
     $     istk(lBWORK), INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(2,info)
         elseif(info.eq.N+1) then
            buf='eigenvalues could not be reordered (the problem '//
     $              'is very ill-conditioned'
            call error(1002)
         elseif(info.eq.N+2) then   
            call msgs(103,0) 
         endif
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 1
      else if(lhs.eq.2) then
         lhsvar(1)=5
         lhsvar(2)=6
      else if(lhs.eq.3) then 
         lhsvar(1)=5
         lhsvar(2)=6
         lhsvar(3)=1
      endif
c     
      end


