      subroutine intfschur(fname)

c     [VS,dim,T]=fschur(A,"function")
c     [VS,dim]=fschur(A,"function")

      include '../stack.h'
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
         buf='dgees'//': the matrix must be square'
         call error(998)
         return
      endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           lhsvar(1) = 1
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2,'d', 0, 0, lSDIM)) return
           lhsvar(1)=1
           lhsvar(2)=2
           return
         else if(lhs.eq.3) then
           if(.not.createvar(2,'d', N, N, lVS)) return
           if(.not.createvar(3,'i', 0, 0, lSDIM)) return 
           lhsvar(1)=2
           lhsvar(2)=3
           lhsvar(3)=1
           return
         endif
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
        buf='dgees'//': not enough memory (use stacksize)'
        call error(998)
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

      if(info.ne.0) then
         call errorinfo("dgees",info)
         return
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
      
      subroutine intzfschur(fname)

c     T=zfschur(A,'function')
c     [VS,dim]=zfschur(A,'function')
c     [VS,dim,T]=zfschur(A,'function')


      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character*4 JOBVS, SORT
      integer zchsel
      external zchsel

       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.ne.N) then
         buf='zgees'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           lhsvar(1) = 1
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2,'z', 0, 0, lSDIM)) return
           lhsvar(1)=1
           lhsvar(2)=2
           return
         else if(lhs.eq.3) then
           if(.not.createvar(2,'z', N, N, lVS)) return
           if(.not.createvar(3,'i', 0, 0, lSDIM)) return 
           lhsvar(1)=2
           lhsvar(2)=3
           lhsvar(3)=1
           return
         endif
       endif

       if(.not.getrhsvar(2,'c', mr, mc, lc)) return
       call setzchsel(mr*mc,cstk(lc:lc+mr*mc),irep)
       if ( irep.eq.1) then 
          buf = cstk(lc:lc+mr*mc)
          call error(50)
          return
       endif

       if(.not.createvar(3,'z', N, 1, lW)) return
       k = 4
       if(lhs.gt.1) then
          if(.not.createvar(4,'z', N, N, lVS)) return
          k = 5
       endif
       if(.not.createvar(k,'i', 1, 1, lSDIM)) return 
       if(.not.createvar(k+1,'d', N, 1, lRWORK)) return
       if(.not.createvar(k+2,'i', 2*N, 1, lBWORK)) return
       LWORKMIN = 2*N
       LWORK=maxvol(k+3,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgees'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(k+3,'z',1,LWORK,lDWORK)) return     

      if(lhs.eq.1) then
        JOBVS = 'N'
        lVS = lDWORK
      else
        JOBVS = 'V'
      endif 
      SORT = 'S'

      call ZGEES( JOBVS, SORT, ZCHSEL, N, zstk(lA), N, istk(lSDIM),
     $     zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $     istk(lBWORK), INFO )
c        SUBROUTINE ZGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, W,
c     $    VS, LDVS, WORK, LWORK, RWORK, BWORK, INFO )
      if(info.ne.0) then
        call errorinfo("zgees",info)
        return
      endif
    
      if(lhs.eq.1) then
        lhsvar(1) = 1
      else if(lhs.eq.2) then
        lhsvar(1)=4
        lhsvar(2)=5
      else if(lhs.eq.3) then
        lhsvar(1)=4
        lhsvar(2)=5
        lhsvar(3)=1
      endif
c
       end

