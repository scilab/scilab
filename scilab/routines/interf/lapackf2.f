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

      subroutine intdgehrd(fname)

c     [H,Q]=hess(A)

      include '../stack.h'
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
         buf='dgehrd'//': the matrix must be square'
         call error(998)
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
         buf='dgehrd'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return
      call DGEHRD( N, 1, N, stk(lA), N, stk(lTAU), stk(lDWORK),
     $     LWORK, INFO )
c      SUBROUTINE DGEHRD( N, 1, N, A, LDA, TAU, WORK, INFO )

      if(lhs.gt.1) call DLACPY( 'F', N, N, stk(lA), N, stk(lQ), N )
c      SUBROUTINE DLACPY( UPLO, N, N, A, LDA, B, LDB )
       if(N.gt.2) then
         do 20 j = 1, N-2
            do 10 i = j+2, N
               ij = i+(j-1)*N
               stk(lA+ij-1) = ZERO
   10       continue
   20    continue
       endif
      
       if(lhs.gt.1) then
         call DORGHR( N, 1, N, stk(lQ), N, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c         SUBROUTINE DORGHR( N, 1, N, A, LDA, TAU, WORK, INFO ) 
       endif

      if(lhs.eq.1) then
        lhsvar(1) = 1
      else
        lhsvar(1)=3
        lhsvar(2)=1
      endif 
c
       end


      subroutine intzgehrd(fname)

c     [Q,H]=hess(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.ne.N) then
         buf='zgehrd'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           lhsvar(1) = 1
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2, 'z', N, N, lQ)) return
           lhsvar(1)=1
           lhsvar(2)=2
           return
         endif
       endif
       if(.not.createvar(2,'z', N-1, 1, lTAU)) return
       k = 3
       if(lhs.gt.1) then
         if(.not.createvar(3,'z', N, N, lQ)) return
         k = 4
       endif
       LWORKMIN = N
       LWORK=maxvol(k,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgehrd'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
      if(.not.createvar(k,'z',1,LWORK,lDWORK)) return
      call ZGEHRD( N, 1, N, zstk(lA), N, zstk(lTAU), zstk(lDWORK),
     $     LWORK, INFO )
c      SUBROUTINE ZGEHRD( N, 1, N, A, LDA, TAU, WORK, INFO )

      if(lhs.gt.1) call ZLACPY( 'F', N, N, zstk(lA), N, zstk(lQ), N )
c      SUBROUTINE ZLACPY( UPLO, N, N, A, LDA, B, LDB )
       if(N.gt.2) then
         do 20 j = 1, N-2
            do 10 i = j+2, N
               ij = i+(j-1)*N
               zstk(lA+ij-1) = ZERO
   10       continue
   20    continue
       endif
      
       if(lhs.gt.1) then
         call ZUNGHR( N, 1, N, zstk(lQ), N, zstk(lTAU), zstk(lDWORK),
     $        LWORK, INFO )
c         SUBROUTINE ZUNGHR( N, 1, N, A, LDA, TAU, WORK, INFO ) 
       endif

      if(lhs.eq.1) then
        lhsvar(1) = 1
      else
        lhsvar(1)=3
        lhsvar(2)=1
      endif
c
       end


      subroutine intdggev(fname)

c     [al,be,V]=gspec(A,B)
c     [al,be] = gspec(A,B)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
   
      character fname*(*)
      character JOBVL, JOBVR
      double precision DZERO
      parameter ( DZERO=0.0D0 )
      complex*16 complex

       minrhs=2
       maxrhs=2
       minlhs=2
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(MA.ne.NA) then
         buf='dggev'//': the matrix A must be square'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(MB.ne.NB) then
         buf='dggev'//': the matrix B must be square'
         call error(998)
         return
       endif
       if(MA.ne.MB) then
         buf='dggev'//':
     $        the matrices A and B must be of the same order'
         call error(998)
         return
       endif
       N = MA
       if(N.eq.0) then
         if(.not.createvar(3,'z', N, 1, lALPHA)) return
         if(.not.createvar(4,'d', N, 1, lBETA)) return
         lhsvar(1)=3
         lhsvar(2)=4
         if(lhs.eq.3) then
           if(.not.createvar(5,'z', N, N, lV)) return
           lhsvar(3)=5
         endif
         return
       endif
       if(.not.createvar(3,'z', N, 1, lALPHA)) return
       k = 4              
       if(lhs.eq.3) then
         if(.not.createvar(4,'d', N, N, lVR)) return
         if(.not.createvar(5,'z', N, N, lV)) return
         k = 6
       endif
       if(.not.createvar(k,'d', N, 1, lALPHAR)) return
       if(.not.createvar(k+1,'d', N, 1, lALPHAI)) return
       if(.not.createvar(k+2,'d', N, 1, lBETA)) return
       LWORKMIN = max(1,8*N)
       LWORK=maxvol(k+3,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dggev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+3,'d',1,LWORK,lDWORK)) return

      JOBVL = 'N'
      if(lhs.eq.2) then
        JOBVR = 'N'
        lVR = lDWORK
      else
        JOBVR = 'V'
      endif 
      call DGGEV( JOBVL, JOBVR, N, stk(lA), N, stk(lB), N, stk(lALPHAR),
     $     stk(lALPHAI), stk(lBETA), stk(lDWORK), N, stk(lVR), N,
     $     stk(lDWORK), LWORK, INFO )
c      SUBROUTINE DGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHAR,
c    $     ALPHAI, BETA, VL, LDVL, VR, LDVR, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dggev",info)
         return
       endif

       do 10 i = 1, N
           zstk(lALPHA+i-1) = complex(stk(lALPHAR+i-1),stk(lALPHAI+i-1))
 10    continue      
      
      if(lhs.eq.3) then
        j = 0
 20     j = j+1
           if(stk(lALPHAI+j-1).eq.DZERO) then
              do 30 i = 1, N
                 ij = i+(j-1)*N
                 zstk(lV+ij-1) = stk(lVR+ij-1)
 30           continue
           else   
              do 40 i = 1, N
                 ij = i+(j-1)*N
                 ij1 = i+j*N
                 zstk(lV+ij-1) = complex(stk(lVR+ij-1),stk(lVR+ij1-1))
                 zstk(lV+ij1-1) = complex(stk(lVR+ij-1),-stk(lVR+ij1-1)) 
 40           continue
              j = j+1
           endif
        if(j.lt.N) go to 20
      endif
     
      if(lhs.eq.2) then
        lhsvar(1)=3
        lhsvar(2)=6
      else 
        lhsvar(1)=3
        lhsvar(2)=8
        lhsvar(3)=5
      endif
c
       end



      subroutine intzggev(fname)

c     [al,be,V]=gspec(A,B)
c     [al,be]=gspec(A,B)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
   

      character fname*(*)
      character JOBVL, JOBVR

       minrhs=2
       maxrhs=2
       minlhs=2
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', MA, NA, lA)) return
       if(MA.ne.NA) then
         buf='zggev'//': the matrix must be square'
         call error(998)
         return
       endif
       if(.not.getrhsvar(2,'z', MB, NB, lB)) return
       if(MB.ne.NB) then
         buf='zggev'//': the matrix must be square'
         call error(998)
         return
       endif
       if(MA.ne.NB) then
         buf='zgeev'//':
     $        the matrices A and B must be of the same order'
         call error(998)
         return
       endif
       N = MA
       if(N.eq.0) then
         if(.not.createvar(3,'z', N, 1, lALPHA)) return
         if(.not.createvar(4,'z', N, 1, lBETA)) return
         lhsvar(1)=3
         lhsvar(2)=4
         if(lhs.eq.3) then
           if(.not.createvar(5,'z', N, N, lVR)) return
           lhsvar(3)=5
         endif
         return
       endif
       if(.not.createvar(3,'z', N, 1, lALPHA)) return
       if(.not.createvar(4,'z', N, 1, lBETA)) return
       K = 5
       if(lhs.eq.3) then
         if(.not.createvar(5,'z', N, N, lVR)) return
         k = 6
       endif
       if(.not.createvar(k,'d', 8*N, 1, lRWORK)) return
       LWORKMIN = 2*N
       LWORK=maxvol(k+1,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zggev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return

      JOBVL = 'N'
      if(lhs.eq.2) then
        JOBVR = 'N'
        lVR = lDWORK
      else
        JOBVR = 'V'
      endif 
      call ZGGEV( JOBVL, JOBVR, N, zstk(lA), N, zstk(lB),N,
     $     zstk(lALPHA), zstk(lBETA), zstk(lDWORK), N, zstk(lVR), N,
     $     zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c      SUBROUTINE ZGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHA, BETA,
c     $     VL, LDVL, VR, LDVR, WORK, LWORK, RWORK, INFO )
       if(info.ne.0) then
         call errorinfo("zggev",info)
         return
       endif


      if(lhs.eq.2) then
        lhsvar(1)=3
        lhsvar(2)=4
      else
        lhsvar(1)=3
        lhsvar(2)=4
        lhsvar(3)=5
      endif
c
       end


      subroutine intdsyev(fname)

c     [D,V]=spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBZ, UPLO
      double precision ZERO
      parameter ( ZERO=0.0D0 )
c
       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.ne.N) then
         buf='dsyev'//': the matrix must be square'
         call error(998)
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
       if(lhs.eq.1) then
         if(.not.createvar(2,'d', N, 1, lD)) return              
       else
         if(.not.createvar(2,'d', N, N, lD)) return
       endif
       if(.not.createvar(3,'d', N, 1, lW)) return
       LWORKMIN = 3*N - 1
       LWORK=maxvol(4,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dsyev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(4,'d',1,LWORK,lDWORK)) return
      if(lhs.eq.1) then
        JOBZ = 'N'
      else
        JOBZ = 'V'
      endif 
      UPLO = 'U'
      call DSYEV( JOBZ, UPLO, N, stk(lA), N, stk(lW),
     $     stk(lDWORK), LWORK, INFO )
c      SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dsyev",info)
         return
       endif

      if(lhs.eq.1) then
        do 10 i = 1, N
           stk(lD+i-1) = stk(lW+i-1)
  10    continue     
      else 
        call DLASET( 'F', N, N, ZERO, ZERO, stk(lD), N ) 
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 20 i = 1, N
           ii = i+(i-1)*N
           stk(lD+ii-1) = stk(lW+i-1)
 20     continue   
      endif 
      lhsvar(1)=2
      lhsvar(2)=1
c
       end

      subroutine intzheev(fname)

c     [D,V]=spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBZ, UPLO
      double precision ZERO
      parameter ( ZERO=0.0D0 )

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.ne.N) then
         buf='zheev'//': the matrix must be square'
         call error(998)
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
       if(lhs.eq.1) then
         if(.not.createvar(2,'d', N, 1, lD)) return              
       else
         if(.not.createvar(2,'d', N, N, lD)) return
       endif
       if(.not.createvar(3,'d', N, 1, lW)) return
       if(.not.createvar(4,'d', 3*N-2, 1, lRWORK)) return
       LWORKMIN = 2*N - 1
       LWORK=maxvol(5,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zheev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(5,'z',1,LWORK,lDWORK)) return

      if(lhs.eq.1) then
        JOBZ = 'N'
      else
        JOBZ = 'V'
      endif 
      UPLO = 'U'
      call ZHEEV( JOBZ, UPLO, N, zstk(lA), N, stk(lW),
     $     zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c      SUBROUTINE ZHEEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK,
c     $    stk(lROWK), INFO )
       if(info.ne.0) then
         call errorinfo("zheev",info)
         return
       endif

      if(lhs.eq.1) then
        do 10 i = 1, N
           stk(lD+i-1) = stk(lW+i-1)
  10    continue     
      else 
        call DLASET( 'F', N, N, ZERO, ZERO, stk(lD), N ) 
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 20 i = 1, N
           ii = i+(i-1)*N
           stk(lD+ii-1) = stk(lW+i-1)
 20     continue   
      endif 
      lhsvar(1)=2
      lhsvar(2)=1
c
       end


























