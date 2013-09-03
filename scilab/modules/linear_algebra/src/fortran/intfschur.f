c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine intfschur(fname)

c     [VS,dim,T]=fschur(A,"function")
c     [VS,dim]=fschur(A,"function")

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4  JOBVS, SORT
      integer schsel
      external schsel
      
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
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return
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
      if(.not.getrhsvar(2,'c', mr, mc, lc)) return
      call setschsel(mr*mc, cstk(lc:lc+mr*mc),irep)
      if ( irep.eq.1) then 
         buf = cstk(lc:lc+mr*mc)
         call error(50)
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
      LWORK=maxvol(k+2,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+2,'d',1,LWORKMIN,lDWORK)) return

      if(lhs.eq.1) then
         JOBVS = 'N'
         lVS = lDWORK
      else
         JOBVS = 'V'
      endif 
      SORT = 'S'

      call DGEES( JOBVS, SORT, schsel, N, stk(lA), N, istk(lSDIM),
     $     stk(lWR), stk(lWI), stk(lVS), N, stk(lDWORK), LWORK,
     $     istk(lBWORK), INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(2,info)
         elseif(info.eq.N+1) then
            buf='eigenvalues could not be reordered (the problem '//
     $           'is very ill-conditioned'
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
      
