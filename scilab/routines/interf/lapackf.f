      subroutine intdgeqpf3(fname)

c     [Q,R]=dgeqpf(A)
c     [Q,R,E]=dgeqpf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c
       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lQ)) return
         if(.not.createvar(3,'d', 0, 0, lR)) return
         if(lhs.gt.2) then
           if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1) = 2
         lhsvar(2) = 3
         if(lhs.gt.2) then
           lhsvar(3) = 4
         endif
         return
       endif
       if(.not.createvar(2,'d', M, M, lQ)) return
       if(.not.createvar(3,'d', M, N, lR)) return
       k=4
       if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
       endif
       if(.not.createvar(k,'d', 1, min(M,N), lTAU)) return
       if(lhs.le.2) then
         LWORKMIN = MAX(1,N)
       else
         LWORKMIN = 3*N
       endif
       LWORK=maxvol(k+1,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgeqpf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      if(lhs.le.2) then
         call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c        SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO ) 
      else
         do 5 i = 1, N
   5     istk(lJPVT+i-1) = 0     
         call DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU),
     $        stk(lDWORK), INFO )
c        SUBROUTINE DGEQPF( M, N, A, LDA, JPVT, TAU, WORK, INFO )
      endif
      if(info.ne.0) then
         call errorinfo("dgeqpf",info)
         return
      endif
      call DLACPY( 'U', M, N, stk(lA), M, stk(lR), M )
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         if(M.gt.N) then
         do 20 j = 1, N
            do 10 i = j+1, M
               ij = i+(j-1)*M
               stk(lR+ij-1) = ZERO
  10        continue
  20     continue
         else
         do 40 j = 1, M-1
            do 30 i = j+1, M
               ij = i+(j-1)*M
               stk(lR+ij-1) = ZERO
  30        continue
  40     continue 
         endif
      endif
   
      if(M.gt.N) then
      call DLACPY( 'F', M, N, stk(lA), M, stk(lQ), M ) 
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
        do 60 j = N+1, M
           do 50 i = 1, M
              ij = i+(j-1)*M
              stk(lQ+ij-1) = ZERO
  50        continue
  60     continue
      else
        call DLACPY( 'F', M, M, stk(lA), M, stk(lQ), M ) 
c       SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      endif
      call DORGQR( M, M, min(M,N), stk(lQ), M, stk(lTAU), stk(lDWORK),
     $            LWORK, INFO )
c     SUBROUTINE DORGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )
      
      if(lhs.gt.2) then
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lE), N ) 
c        SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
      endif
 
      if(lhs.le.2) then
        lhsvar(1)=2
        lhsvar(2)=3
      else
        lhsvar(1)=2
        lhsvar(2)=3
        lhsvar(3)=4
      endif
c
      end




      subroutine intdgeqpf4(fname)

c     [Q,R]=dgeqpf(A,'e')
c     [Q,R,E]=dgeqpf(A,'e')

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )
c
       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lQ)) return
         if(.not.createvar(3,'d', 0, 0, lR)) return
         if(lhs.gt.2) then
           if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1)=2
         lhsvar(2)=3
         if(lhs.gt.2) then
           lhsvar(3)=4
         endif
         return
      endif   
       if(.not.createvar(2,'d', M, min(M,N), lQ)) return
       if(.not.createvar(3,'d', min(M,N), N, lR)) return
       k=4
       if(lhs.gt.2) then
          if(.not.createvar(4,'d', N, N, lE)) return
          if(.not.createvar(5,'i', 1, N, lJPVT)) return
          k=6
       endif
       if(.not.createvar(k,'d', 1, min(M,N), lTAU)) return
       if(lhs.le.2) then
         LWORKMIN = MAX(1,N)
       else 
         LWORKMIN = 3*N
       endif
       LWORK=maxvol(k+1,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgeqpf'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return
      if(lhs.le.2) then
         call DGEQRF( M, N, stk(lA), M, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c        SUBROUTINE DGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )
      else
         do 5 i = 1, N
   5     istk(lJPVT+i-1) = 0     
         call DGEQPF( M, N, stk(lA), M, istk(lJPVT), stk(lTAU),
     $        stk(lDWORK), INFO )
c        SUBROUTINE DGEQPF( M, N, A, LDA, JPVT, TAU, WORK, INFO )
      endif
      if(info.ne.0) then
         call errorinfo("dgeqpf",info)
         return
      endif
      call DLACPY( 'U', min(M,N), N, stk(lA), M, stk(lR), min(M,N) )
c     SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
         do 20 j = 1, N-1
         if(j+1.le.min(M,N)) then
            do 10 i = j+1, min(M,N)
               ij = i+(j-1)*min(M,N)
               stk(lR+ij-1) = ZERO
  10        continue
         endif
  20     continue
      endif
   
      call DLACPY( 'F', M, min(M,N), stk(lA), M, stk(lQ), M ) 
c      SUBROUTINE DLACPY( UPLO, M, N, A, B, LDB )
      call DORGQR( M, min(M,N), min(M,N), stk(lQ), M, stk(lTAU),
     $     stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DORGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )
 
      if(lhs.gt.2) then      
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lE), N ) 
c        SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 30 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 30      continue    
      endif

      if(lhs.le.2) then
        lhsvar(1)=2
        lhsvar(2)=3
      else
        lhsvar(1)=2
        lhsvar(2)=3
        lhsvar(3)=4
      endif
c
      end



      subroutine intzgeqpf3(fname)

c     [Q,R]=zgeqpf(A)
c     [Q,R,E]=zgeqpf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ONE, DZERO
      parameter ( ONE=1.0D0, DZERO=0.0D0)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )


       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.eq.0 .or.N.eq.0) then
         if(.not.createvar(2,'z', 0, 0, lQ)) return
         if(.not.createvar(3,'z', 0, 0, lR)) return
         if(lhs.gt.2) then
           if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1) = 2
         lhsvar(2) = 3
         if(lhs.gt.2) then
           lhsvar(3) = 4
         endif
         return
       endif
       if(.not.createvar(2,'z', M, M, lQ)) return
       if(.not.createvar(3,'z', M, N, lR)) return
       k=4
       if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
       endif       
       if(.not.createvar(k,'z', 1, min(M,N), lTAU)) return
       if(.not.createvar(k+1,'d', 1, 2*N, lRWORK)) return
       LWORKMIN = N
       LWORK=maxvol(k+2,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgeqpf'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(k+2,'z', 1, LWORK, lDWORK)) return

      if(lhs.le.2) then
        call ZGEQRF( M, N, zstk(lA), M, zstk(lTAU), zstk(lDWORK),
     $       LWORK, INFO )
c       SUBROUTINE ZGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )
      else
        do 5 i = 1, N
   5    istk(lJPVT+i-1) = 0     
        call ZGEQPF( M, N, zstk(lA), M, istk(lJPVT), zstk(lTAU),
     $       zstk(lDWORK), stk(lRWORK), INFO )
c       SUBROUTINE ZGEQPF( M, N, A, LDA, JPVT, TAU, WORK, RWORK, INFO )
      endif
      if(info.ne.0) then
        call errorinfo("zgeqpf",info)
        return
       endif

      call ZLACPY( 'U', M, N, zstk(lA), M, zstk(lR), M )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
        if(M.gt.N) then
        do 20 j = 1, N
           do 10 i = j+1, M
              ij = i+(j-1)*M
              zstk(lR+ij-1) = ZERO
  10       continue
  20    continue
        else
        do 40 j = 1, M-1
           do 30 i = j+1, M
              ij = i+(j-1)*M
              zstk(lR+ij-1) = ZERO
  30       continue
  40    continue
        endif 
      endif
   
      if(M.gt.N) then
      call ZLACPY( 'F', M, N, zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
        do 60 j = N+1, M
           do 50 i = 1, M
              ij = i+(j-1)*M 
              zstk(lQ+ij-1) = ZERO
  50        continue
  60     continue
      else
        call ZLACPY( 'F', M, M, zstk(lA), M, zstk(lQ), M ) 
c       SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      endif
      call ZUNGQR( M, M, min(M,N), zstk(lQ), M, zstk(lTAU),
     $     zstk(lDWORK), LWORK, INFO )
c     SUBROUTINE ZUNGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )

      if(lhs.gt.2) then
         call DLASET( 'F', N, N, DZERO, DZERO, stk(lE), N ) 
c        SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
         do 70 j = 1, N
            i = istk(lJPVT+j-1)
            ij = i + (j-1)*N
            stk(lE+ij-1) = ONE
 70      continue    
      endif

      if(lhs.le.2) then
        lhsvar(1)=2
        lhsvar(2)=3
      else
        lhsvar(1)=2
        lhsvar(2)=3
        lhsvar(3)=4
      endif
c
      end
 
      subroutine intzgeqpf4(fname)

c     [Q,R]=zgeqpf(A,'e')
c     [Q,R,E]=zgeqpf(A,'e')

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      double precision ONE, DZERO
      parameter ( ONE=1.0D0, DZERO=0.0D0)
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )


       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(2,'z', 0, 0, lQ)) return
         if(.not.createvar(3,'z', 0, 0, lR)) return
         if(lhs.gt.2) then
           if(.not.createvar(4,'d', 0, 0, lE)) return
         endif
         lhsvar(1)=2
         lhsvar(2)=3
         if(lhs.gt.2) then
           lhsvar(3)=4
         endif
         return
      endif      
       if(.not.createvar(2,'z', M, min(M,N), lQ)) return
       if(.not.createvar(3,'z', min(M,N), N, lR)) return
       k=4
       if(lhs.gt.2) then
         if(.not.createvar(4,'d', N, N, lE)) return
         if(.not.createvar(5,'i', 1, N, lJPVT)) return
         k=6
       endif
       if(.not.createvar(k,'z', 1, min(M,N), lTAU)) return
       if(.not.createvar(k+1,'d', 1, 2*N, lRWORK)) return
       LWORKMIN = N
       LWORK=maxvol(k+2,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgeqpf'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(k+2,'z', 1, LWORK, lDWORK)) return


      if(lhs.le.2) then
        call ZGEQRF( M, N, zstk(lA), M, zstk(lTAU), zstk(lDWORK),
     $       LWORK, INFO )
c       SUBROUTINE ZGEQRF( M, N, A, LDA, TAU, WORK, LWORK, INFO )      
      else
        do 5 i = 1, N
   5    istk(lJPVT+i-1) = 0     
        call ZGEQPF( M, N, zstk(lA), M, istk(lJPVT), zstk(lTAU),
     $       zstk(lDWORK), stk(lRWORK), INFO )
c       SUBROUTINE ZGEQPF( M, N, A, LDA, JPVT, TAU, WORK, RWORK, INFO )
      endif
      if(info.ne.0) then
        call errorinfo("zgeqpf",info)
        return
      endif
 
      call ZLACPY( 'U', min(M,N), N, zstk(lA), M, zstk(lR), min(M,N) )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      if(N.ge.2) then
      do 20 j = 1, N-1
         if(j+1.le.min(M,N)) then
         do 10 i = j+1, min(M,N)
            ij = i+(j-1)*min(M,N)
            zstk(lR+ij-1) = ZERO
  10     continue
         endif
  20  continue
      endif

      call ZLACPY( 'F', M, min(M,N), zstk(lA), M, zstk(lQ), M ) 
c     SUBROUTINE ZLACPY( UPLO, M, N, A, B, LDB )
      call ZUNGQR( M, min(M,N), min(M,N), zstk(lQ), M, zstk(lTAU),
     $     zstk(lDWORK), LWORK, INFO )
c     SUBROUTINE ZUNGQR( M, N, K, A, LDA, TAU, WORK, LWORK, INFO )

      if(lhs.gt.2) then
        call DLASET( 'F', N, N, DZERO, DZERO, stk(lE), N ) 
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
        do 30 j = 1, N
           i = istk(lJPVT+j-1)
           ij = i + (j-1)*N
           stk(lE+ij-1) = ONE
 30     continue    
      endif

      if(lhs.le.2) then
        lhsvar(1)=2
        lhsvar(2)=3
      else
        lhsvar(1)=2
        lhsvar(2)=3
        lhsvar(3)=4
      endif
c
      end
 
      subroutine intdgesvd1(fname)

c     [U,S,V]=dgesvd(A)
c     s = dgesvd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         endif
       endif
       if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
       k = 3              
       if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'d', N, N, lV)) return
         if(.not.createvar(6,'d', N, N, lVT)) return
         k = 7
       endif
       LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
       LWORK=maxvol(k,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgesvd'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
        JOBU = 'A'
        JOBVT = 'A'
      endif
      if(lhs.eq.1) then
        call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $       M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c        SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      else
        call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $       M, stk(lVT), N, stk(lDWORK), LWORK, INFO )
c        SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      endif  
       if(info.ne.0) then
         call errorinfo("dgesvd",info)
         return
       endif

      if(lhs.gt.1) then
        call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, min(M,N)
           ii = i+(i-1)*M
           stk(lS+ii-1) = stk(lSV+i-1)
   10   continue        
        do 30 j = 1, N
           do 20 i = j, N
              ij = i+(j-1)*N
              ji = j+(i-1)*N
              stk(lV+ij-1) = stk(lVT+ji-1)
              stk(lV+ji-1) = stk(lVT+ij-1)
   20      continue
   30   continue     
      endif
     
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else 
        lhsvar(1)=3
        lhsvar(2)=4
        lhsvar(3)=5
      endif
c
       end

      subroutine intdgesvd2(fname)

c     [U,S,V]=dgesvd(A)
c     s = dgesvd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )

       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         endif
       endif
       if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
       k = 3              
       if(lhs.gt.1) then
         if(.not.createvar(3,'d', M, min(M,N), lU)) return
         if(.not.createvar(4,'d', min(M,N), min(M,N), lS)) return
         if(.not.createvar(5,'d', N, min(M,N), lV)) return
         if(.not.createvar(6,'d', min(M,N), N, lVT)) return
         k = 7
       endif
       LWORKMIN = max(3*min(M,N)+max(M,N),5*min(M,N)-4)
       LWORK=maxvol(k,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgesvd'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
        JOBU = 'S'
        JOBVT = 'S'
      endif
      if(lhs.eq.1) then
        call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lA),
     $       M, stk(lA), N, stk(lDWORK), LWORK, INFO )
c        SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      else
        call DGESVD( JOBU, JOBVT, M, N, stk(lA), M, stk(lSV), stk(lU),
     $       M, stk(lVT), min(M,N), stk(lDWORK), LWORK, INFO )
c        SUBROUTINE DGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, INFO )
      endif  
       if(info.ne.0) then
         call errorinfo("dgesvd",info)
         return
       endif

      if(lhs.gt.1) then
        call DLASET( 'F', min(M,N), min(M,N), ZERO, ZERO, stk(lS),
     $       min(M,N) )
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, min(M,N)
           ii = i+(i-1)*min(M,N)
           stk(lS+ii-1) = stk(lSV+i-1)
   10   continue        
        do 30 j = 1, min(M,N)
           do 20 i = 1, N
              ij = i+(j-1)*N
              ji = j+(i-1)*min(M,N)
              stk(lV+ij-1) = stk(lVT+ji-1)
   20      continue
   30   continue     
      endif
     
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else 
        lhsvar(1)=3
        lhsvar(2)=4
        lhsvar(3)=5
      endif
c
       end

      subroutine intzgesvd1(fname)

c     [U,S,V]=zgesvd(A)
c     s = zgesvd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
 
      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )
      intrinsic conjg

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         endif
       endif
       if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
       k = 3              
       if(lhs.gt.1) then
         if(.not.createvar(3,'z', M, M, lU)) return
         if(.not.createvar(4,'d', M, N, lS)) return
         if(.not.createvar(5,'z', N, N, lV)) return
         if(.not.createvar(6,'z', N, N, lVT)) return
         k = 7
       endif
       LRWRK = max(3*min(M,N),5*min(M,N)-4)
       if(.not.createvar(k,'d',1,LRWRK,lRWORK)) return
       LWORKMIN = 2*min(M,N)+max(M,N)
       LWORK=maxvol(k+1,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgesvd'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
        JOBU = 'A'
        JOBVT = 'A'
      endif
      if(lhs.eq.1) then
        call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lA),
     $       M, zstk(lA), N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c        SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      else
        call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lU),
     $       M, zstk(lVT), N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c        SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      endif  
       if(info.ne.0) then
         call errorinfo("zgesvd",info)
         return
       endif

      if(lhs.gt.1) then
        call DLASET( 'F', M, N, ZERO, ZERO, stk(lS), M )
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, min(M,N)
           ii = i+(i-1)*M
           stk(lS+ii-1) = stk(lSV+i-1)
   10   continue        
        do 30 j = 1, N
           do 20 i = j, N
              ij = i+(j-1)*N
              ji = j+(i-1)*N
              zstk(lV+ij-1) = conjg(zstk(lVT+ji-1))
              zstk(lV+ji-1) = conjg(zstk(lVT+ij-1))
   20      continue
   30   continue     
      endif
     
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else 
        lhsvar(1)=3
        lhsvar(2)=4
        lhsvar(3)=5
      endif
c
       end

      subroutine intzgesvd2(fname)

c     [U,S,V]=zgesvd(A)
c     s = zgesvd(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
 
      character fname*(*)
      character JOBU, JOBVT
      double precision ZERO
      parameter ( ZERO=0.0D0 )
      intrinsic conjg

       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=3
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.eq.0) then
         if(lhs.eq.1) then
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2,'d', N, N, lS)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         else if(lhs.eq.3) then
            if(.not.createvar(2,'d', M, N, lS)) return
            if(.not.createvar(3,'d', N, N, lV)) return
            lhsvar(1) = 1
            lhsvar(2) = 2
            lhsvar(3) = 3
            return
         endif
       endif
       if(.not.createvar(2,'d', min(M,N), 1, lSV)) return
       k = 3              
       if(lhs.gt.1) then
         if(.not.createvar(3,'z', M, min(M,N), lU)) return
         if(.not.createvar(4,'d', min(M,N), min(M,N), lS)) return
         if(.not.createvar(5,'z', N, min(M,N), lV)) return
         if(.not.createvar(6,'z', min(M,N), N, lVT)) return
         k = 7
       endif
       LRWRK = 5*min(M,N)
       if(.not.createvar(k,'d',1,LRWRK,lRWORK)) return
       LWORKMIN = 2*min(M,N)+max(M,N)
       LWORK=maxvol(k+1,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgesvd'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return

      JOBU = 'N'
      JOBVT = 'N'
      if(lhs.gt.1) then
        JOBU = 'S'
        JOBVT = 'S'
      endif
      if(lhs.eq.1) then
        call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lA),
     $       M, zstk(lA), N, zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c        SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      else
        call ZGESVD( JOBU, JOBVT, M, N, zstk(lA), M, stk(lSV), zstk(lU),
     $       M, zstk(lVT), min(M,N), zstk(lDWORK), LWORK, stk(lRWORK),
     $     INFO )
c        SUBROUTINE ZGESVD( JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT,
C     $      LDVT, WORK, LWORK, RWORK, INFO )
      endif  
       if(info.ne.0) then
         call errorinfo("zgesvd",info)
         return
       endif

      if(lhs.gt.1) then
        call DLASET( 'F', min(M,N), min(M,N), ZERO, ZERO, stk(lS),
     $               min(M,N) )
c       SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, min(M,N)
           ii = i+(i-1)*min(M,N)
           stk(lS+ii-1) = stk(lSV+i-1)
   10   continue        
        do 30 j = 1, min(M,N)
           do 20 i = 1, N
              ij = i+(j-1)*N
              ji = j+(i-1)*min(M,N)
              zstk(lV+ij-1) = conjg(zstk(lVT+ji-1))
   20      continue
   30   continue     
      endif
     
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else 
        lhsvar(1)=3
        lhsvar(2)=4
        lhsvar(3)=5
      endif
c
       end

      subroutine intdgelsy(fname)

c     [X,rank]=dgelsy(A,B,rcond)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision EPS, RCOND
      double precision dlamch
      external dlamch
      intrinsic sqrt
c
       minrhs=2
       maxrhs=3
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, N, lA)) return
       if(.not.getrhsvar(2,'d', MB, NRHS, lB)) return
       if(MA .ne. MB) then
         buf='dgelsy'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       M = MA
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'d', 0, 0, lX)) return
         if(.not.createvar(4,'i', 0, 0, lRANK)) return
         lhsvar(1) = 3
         lhsvar(2) = 4
         return
       endif
       if(rhs.eq.2) then
         eps = dlamch('eps')
         rcond = sqrt(eps)
         MR = 1
         NR = 1
         if(.not.createvar(3,'d', MR, NR, lRCOND)) return
         stk(lRCOND) = RCOND
       else
         if(.not.getrhsvar(3,'d', MR, NR, lRCOND)) return
       endif
       if(MR.ne.1 .or. NR.ne.1 ) then
         buf='dgelsy'//': RCOND must be a scalar'
         call error(998)
         return
       endif
       if(.not.createvar(4,'d', N, NRHS, lX)) return
       if(.not.createvar(5,'d', max(M,N), NRHS, lXB)) return
       if(.not.createvar(6,'i', 1, 1, lRANK)) return
       if(.not.createvar(7,'i', 1, N, lJPVT)) return

       LWORKMIN = max( min(M,N)+3*N+1, 2*min(M,N)+NRHS )
       LWORK=maxvol(8,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgelsy'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(8,'d',1,LWORK,lDWORK)) return

      call DLACPY( 'F', M, N, stk(lB), M, stk(lXB), max(M,N) )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
   10 istk(lJPVT+i-1) = 0
      call DGELSY( M, N, NRHS, stk(lA), M, stk(lXB), max(M,N),
     $     istk(lJPVT), stk(lRCOND), istk(lRANK), stk(lDWORK),
     $     LWORK, INFO )
c      SUBROUTINE DGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $     RANK, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dgelsy",info)
         return
       endif

      call DLACPY( 'F', N, NRHS, stk(lXB), max(M,N), stk(lX), N )
c     SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )

      if(lhs.eq.1) then
        lhsvar(1)=4
      else
        lhsvar(1)=4
        lhsvar(2)=6
      endif
c
       end

      subroutine intzgelsy(fname)

c     [X,rank]=zgelsy(A,B,rcond)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision EPS, RCOND
      double precision dlamch
      external dlamch
      intrinsic sqrt

       minrhs=2
       maxrhs=3
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return

       if(.not.getrhsvar(1,'z', MA, N, lA)) return
       if(.not.getrhsvar(2,'z', MB, NRHS, lB)) return

       if(MA .ne. MB) then
         buf='zgelsy'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       M = MA
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         if(.not.createvar(4,'i', 0, 0, lRANK)) return     
         lhsvar(1) = 3
         lhsvar(2) = 4
         return
       endif
       if(rhs.eq.2) then
         eps = dlamch('eps')
         rcond = sqrt(eps)
         MR = 1
         NR = 1
         if(.not.createvar(3,'d', MR, NR, lRCOND)) return
         stk(lRCOND) = RCOND
       else
         if(.not.getrhsvar(3,'d', MR, NR, lRCOND)) return
       endif
       if(MR.ne.1 .or. NR.ne.1 ) then
          buf='zgelsy'//': RCOND must be a scalar'
          call error(998)
          return
       endif
       if(.not.createvar(4,'z', N, NRHS, lX)) return
       if(.not.createvar(5,'z', max(M,N), NRHS, lXB)) return
       if(.not.createvar(6,'i', 1, 1, lRANK)) return
       if(.not.createvar(7,'i', 1, N, lJPVT)) return
       if(.not.createvar(8,'d', 1, 2*N, lRWORK)) return
       LWORKMIN = min(M,N) +  max( 2*min(M,N), N+1, min(M,N)+NRHS )
       LWORK=maxvol(9,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgelsy'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(9,'z',1,LWORK,lDWORK)) return

      call ZLACPY( 'F', M, N, zstk(lB), M, zstk(lXB), max(M,N) )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
      do 10 i = 1, N
   10 istk(lJPVT+i-1) = 0
      call ZGELSY( M, N, NRHS, zstk(lA), M, zstk(lXB), max(M,N),
     $     istk(lJPVT), stk(lRCOND), istk(lRANK), zstk(lDWORK),
     $     LWORK, stk(lRWORK), INFO )
c      SUBROUTINE ZGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $     RANK, WORK, LWORK, RWORK, INFO )
       if(info.ne.0) then
         call errorinfo("zgelsy",info)
         return
       endif
      call ZLACPY( 'F', N, NRHS, zstk(lXB), max(M,N), zstk(lX), N )
c     SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )

       lhsvar(1)=4
       lhsvar(2)=6
c
       end

      subroutine intdgeev(fname)

c     [V,D]=dgeev(A)
c     d = dgeev(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character JOBVL, JOBVR
      double precision DZERO
      parameter ( DZERO=0.0D0 )
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )
      complex*16 complex

       minrhs=1
       maxrhs=1
       minlhs=1
       maxlhs=2
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return 

       if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.ne.N) then
         buf='dgeev'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           if(.not.createvar(2,'z', 0, 0, lD)) return
           lhsvar(1) = 2
           return
         else if(lhs.eq.2) then
           if(.not.createvar(2,'z', 0, 0, lD)) return
           if(.not.createvar(3,'d', 0, 0, lV)) return
           lhsvar(1) = 2
           lhsvar(2) = 3
           return
         endif
       endif  
       if(lhs.eq.1) then
         if(.not.createvar(2,'z', N, 1, lD)) return
         k = 3              
       else if(lhs.eq.2) then
         if(.not.createvar(2,'z', N, N, lD)) return
         if(.not.createvar(3,'d', N, N, lVR)) return
         if(.not.createvar(4,'z', N, N, lV)) return
         k = 5
       endif
       if(.not.createvar(k,'d', N, 1, lWR)) return
       if(.not.createvar(k+1,'d', N, 1, lWI)) return
       LWORKMIN = 3*N
       if(lhs.gt.1) LWORKMIN = 4*N
       LWORK=maxvol(k+2,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgeev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+2,'d',1,LWORK,lDWORK)) return
      JOBVL = 'N'
      if(lhs.eq.1) then
        JOBVR = 'N'
      else
        JOBVR = 'V'
      endif 
      call DGEEV( JOBVL, JOBVR, N, stk(lA), N, stk(lWR), stk(lWI),
     $     stk(lDWORK), N, stk(lVR), N, stk(lDWORK), LWORK, INFO )
c      SUBROUTINE DGEEV( JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR, 
c     $     WORK, INFO )
       if(info.ne.0) then
         call errorinfo("dgeev",info)
         return
       endif

      if(lhs.eq.1) then
        call ZLASET( 'F', N, 1, ZERO, ZERO, zstk(lD), N ) 
c       SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 5 i = 1, N
           zstk(lD+i-1) = complex(stk(lWR+i-1),stk(lWI+i-1))
  5     continue     
      else 
        call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c       SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, N
           ii = i+(i-1)*N
           zstk(lD+ii-1) = complex(stk(lWR+i-1),stk(lWI+i-1))
 10     continue   
      
        j = 0
 20     j = j+1
           if(stk(lWI+j-1).eq.DZERO) then
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
     
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else 
        lhsvar(1)=4
        lhsvar(2)=2
      endif
c
       end

      subroutine intzgeev(fname)

c     [V,D]=zgeev(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
  
      character fname*(*)
      character JOBVL, JOBVR
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
         buf='zgeev'//': the matrix must be square'
         call error(998)
         return
       endif
       if(N.eq.0) then
         if(lhs.eq.1) then
           if(.not.createvar(2,'z',0,0,lD)) return
           lhsvar(1) = 2
           return
         else
           if(.not.createvar(2,'z', 0, 0, lD)) return
           if(.not.createvar(3,'d', 0, 0, lV)) return
           lhsvar(1) = 2
           lhsvar(2) = 3
           return
         endif
       endif  
       if(lhs.eq.1) then
         if(.not.createvar(2,'z', N, 1, lD)) return
         k = 3              
       else if(lhs.eq.2) then
         if(.not.createvar(2,'z', N, N, lD)) return
         if(.not.createvar(3,'z', N, N, lVR)) return
         k = 4
       endif
       if(.not.createvar(k,'z', N, 1, lW)) return
       if(.not.createvar(k+1,'d', 2*N, 1, lRWORK)) return
       LWORKMIN = 2*N
       LWORK=maxvol(k+2,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgeev'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(k+2,'z',1,LWORK,lDWORK)) return

      JOBVL = 'N'
      if(lhs.eq.1) then
        JOBVR = 'N'
      else
        JOBVR = 'V'
      endif 
      call ZGEEV( JOBVL, JOBVR, N, zstk(lA), N, zstk(lW), 
     $     zstk(lDWORK), N, zstk(lVR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), INFO )
c      SUBROUTINE ZGEEV( JOBVL, JOBVR, N, A, LDA, W, VL, LDVL,
c    $     VR, LDVR, WORK, LWORK, RWORK, INFO )
       if(info.ne.0) then
         call errorinfo("zgeev",info)
         return
       endif

      if(lhs.gt.1) then
        call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c       SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

        do 10 i = 1, N
           ii = i+(i-1)*N
           zstk(lD+ii-1) = zstk(lW+i-1)
 10     continue   
      endif
      
      if(lhs.eq.1) then
        lhsvar(1) = 2
      else
        lhsvar(1)=3
        lhsvar(2)=2
      endif
c
       end

      subroutine intdgetri(fname)

c     B = dgetri(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
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
         buf='dgetri'//': the matrix must be square'
         call error(998)
         return
      endif
      if(n.eq.0) then
        lhsvar(1) = 1
        return
      endif
      if(.not.createvar(2,'i',1,N,lIWORK)) return
      LWORKMIN = MAX(1,N)
      LWORK=maxvol(3,'d')
      if(LWORK.le.LWORKMIN) then
         buf='dgetri'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(3,'d',1,LWORK,lDWORK)) return

      call DGETRF( N, N, stk(lA), N, istk(lIWORK), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.ne.0) then
         call errorinfo("dgetrf",info)
         return
      endif
      call DGETRI( N, stk(lA), N, istk(lIWORK), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end

      subroutine intzgetri(fname)

c     B = zgetri(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
c     
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(m.ne.n) then
         buf='zgetri'//': the matrix must be square'
         call error(998)
         return
      endif
      if(n.eq.0) then
        lhsvar(1) = 1
        return
      endif
      if(.not.createvar(2,'i',1,N,lIWORK)) return
      LWORKMIN = MAX(1,N)
      LWORK=maxvol(3,'z')
      if(LWORK.le.LWORKMIN) then
         buf='zgetri'//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(3,'z',1,LWORK,lDWORK)) return

      call ZGETRF( N, N, zstk(lA), N, istk(lIWORK), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.ne.0) then
         call errorinfo("zgetrf",info)
         return
      endif
      call ZGETRI( N, zstk(lA), N, istk(lIWORK), zstk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE ZGETRI( N, A, LDA, IPIV, WORK, LWORK, INFO )

      lhsvar(1)=1
      
c     
      end


      subroutine intdgecon(fname)

c     rcond

      include '../stack.h'
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
         buf='dgecon'//': matrix must be square'
         call error(998)
         return
       endif
       N = MA

       if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 2
         return
       endif

       if(.not.createvar(2,'d', 1, 1, lRCOND)) return
       if(.not.createvar(3,'i', 1, N, lIPIV)) return
       if(.not.createvar(4,'i', 1, N, lIWORK)) return
       LWORKMIN =  4*N
       LWORK=maxvol(5,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgecon'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(5,'d',1,LWORK,lDWORK)) return
       ANORM = dlange( '1', N, N, stk(lA), N, stk(lDWORK) )
c      DOUBLE PRECISION FUNCTION DLANGE( NORM, M, N, A, LDA, WORK )
       call DGETRF( N, N, stk(lA), N, istk(lIPIV), INFO )         
c       SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
       stk(lRCOND) = 0.0d0
       if(INFO.eq.0) then
         call DGECON( '1', N, stk(lA), N, ANORM, stk(lRCOND),
     $                stk(lDWORK), istk(lIWORK), INFO )
c        SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     IWORK, INFO )
       endif

       lhsvar(1) = 2
       return
c
       end

      subroutine intzgecon(fname)

c     rcond

      include '../stack.h'
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
         buf='zgecon'//': matrix must be square'
         call error(998)
         return
       endif
       N = MA

       if(N.eq.0) then
         if(.not.createvar(2,'d', 0, 0, lRCOND)) return
         lhsvar(1) = 2
         return
       endif

       if(.not.createvar(2,'d', 1, 1, lRCOND)) return
       if(.not.createvar(3,'i', 1, N, lIPIV)) return
       if(.not.createvar(4,'i', 1, N, lIWORK)) return
       if(.not.createvar(5,'d', 1, 2*N, lRWORK)) return
       LWORKMIN =  2*N
       LWORK=maxvol(6,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgecon'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(6,'z',1,LWORK,lDWORK)) return
       ANORM = zlange( '1', N, N, zstk(lA), N, zstk(lDWORK) )
c      DOUBLE PRECISION FUNCTION ZLANGE( NORM, M, N, A, LDA, WORK )
       call ZGETRF( N, N, zstk(lA), N, istk(lIPIV), INFO )         
c       SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
       stk(lRCOND) = 0.0d0
       if(INFO.eq.0) then
         call ZGECON( '1', N, zstk(lA), N, ANORM, stk(lRCOND),
     $                zstk(lDWORK), istk(lIWORK), stk(lRWORK), INFO )
c        SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                     IWORK, RWORK, INFO )
       endif

       lhsvar(1) = 2
       return
c
       end

      subroutine intdpotrf(fname)

c     U = dpotrf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
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
         buf='dgetri'//': the matrix must be square'
         call error(998)
         return
      endif

      call DPOTRF( 'U', N, stk(lA), N, INFO )
c     SUBROUTINE DPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         call errorinfo("dpotrf",info)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               stk(lA+ij-1)=0.0d0    
 10         continue
 20      continue
      endif
   
      lhsvar(1)=1
      
c     
      end


      subroutine intzpotrf(fname)

c     U = zpotrf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      complex*16 ZERO
      parameter ( ZERO = (0.0d0,0.0d0) )
      character fname*(*)
  
      minrhs=1
      maxrhs=1
      minlhs=1
      maxlhs=1
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
      if(m.ne.n) then
         buf='zpotrf'//': the matrix must be square'
         call error(998)
         return
      endif

      call ZPOTRF( 'U', N, zstk(lA), N, INFO )
c     SUBROUTINE ZPOTRF( 'U', N, A, LDA, INFO )
      if(info.ne.0) then
         call errorinfo("zpotrf",info)
         return
      endif

      if( N.gt.1 ) then
         do 20 j=1,N
            do 10 i=j+1,N
               ij = i+(j-1)*N
               zstk(lA+ij-1)=ZERO    
 10         continue
 20      continue
      endif
   
      lhsvar(1)=1
      
c     
      end

      subroutine intdgetrf(fname)

c     [L,U,E] = dgetrf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO = 0.0D0, ONE = 1.0D0 )
c     
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'d', M, N, lA)) return
       if(M.eq.0 .or.N.eq.0) then
        lhsvar(1) = 1
        return
      endif
      if(.not.createvar(2,'d',M,min(M,N),lL)) return
      if(.not.createvar(3,'d',min(M,N),N,lU)) return
      if(.not.createvar(4,'i',1,min(M,N),lIPIV)) return
      if(.not.createvar(5,'d',M,M,lE)) return


      call DGETRF( M, N, stk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE DGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.ne.0) then
         call errorinfo("dgetrf",info)
         return
      endif
 
      do 20 j=1,min(M,N)
         do 10 i=1,M
            ij = i+(j-1)*M
            if(i.eq.j) then
              stk(lL+ij-1)=ONE
            else if(i.gt.j) then
              stk(lL+ij-1)=stk(lA+ij-1)
            else if(i.lt.j) then
              stk(lL+ij-1)=ZERO
            endif
 10      continue
 20   continue     
      do 40 j=1,N
         do 30 i=1,min(M,N)
            ij1 = i+(j-1)*min(M,N)
            ij2 = i+(j-1)*M
            if(i.gt.j) then
              stk(lU+ij1-1)=ZERO
            else if(i.le.j) then
              stk(lU+ij1-1)=stk(lA+ij2-1)
            endif
 30      continue
 40   continue
      if(lhs.gt.2) then
        call DLASET( 'F', M, M, ZERO, ONE, stk(lE), M ) 
c        SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
        call DLASWP( M, stk(lE), M, 1, min(M,N), istk(lIPIV), 1 )
c        SUBROUTINE DLASWP( N, A, LDA, K1, K2, IPIV, INCX )   
      endif   


      lhsvar(1)=2
      lhsvar(2)=3
      lhsvar(3)=5
c
      return     
      end


      subroutine intzgetrf(fname)

c     [L,U,E] = zgetrf(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ZERO, ONE
      parameter ( ZERO = 0.0D0, ONE = 1.0D0 )
      complex*16 CZERO, CONE
      parameter ( CZERO = (0.0D0,0.0D0), CONE = (1.0D0,0.0D0) )
c     
      minrhs=1
      maxrhs=1
      minlhs=2
      maxlhs=3
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return

      if(.not.getrhsvar(1,'z', M, N, lA)) return
       if(M.eq.0 .or.N.eq.0) then
        lhsvar(1) = 1
        return
      endif
      if(.not.createvar(2,'z',M,min(M,N),lL)) return
      if(.not.createvar(3,'z',min(M,N),N,lU)) return
      if(.not.createvar(4,'i',1,min(M,N),lIPIV)) return
      if(.not.createvar(5,'d',M,M,lE)) return


      call ZGETRF( M, N, zstk(lA), M, istk(lIPIV), INFO )
c     SUBROUTINE ZGETRF( M, N, A, LDA, IPIV, INFO )
      if(info.ne.0) then
         call errorinfo("zgetrf",info)
         return
      endif
 
      do 20 j=1,min(M,N)
         do 10 i=1,M
            ij = i+(j-1)*M
            if(i.eq.j) then
              zstk(lL+ij-1) = CONE
            else if(i.gt.j) then
              zstk(lL+ij-1) = zstk(lA+ij-1)
            else if(i.lt.j) then
              zstk(lL+ij-1) = CZERO
            endif
 10      continue
 20   continue     
      do 40 j=1,N
         do 30 i=1,min(M,N)
            ij1 = i+(j-1)*min(M,N)
            ij2 = i+(j-1)*M
            if(i.gt.j) then
              zstk(lU+ij1-1) = CZERO
            else if(i.le.j) then
              zstk(lU+ij1-1) = zstk(lA+ij2-1)
            endif
 30      continue
 40   continue
      if(lhs.gt.2) then
        call DLASET( 'F', M, M, ZERO, ONE, stk(lE), M ) 
c        SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )
        call DLASWP( M, stk(lE), M, 1, min(M,N), istk(lIPIV), 1 )
c        SUBROUTINE DLASWP( N, A, LDA, K1, K2, IPIV, INCX )   
      endif   


      lhsvar(1)=2
      lhsvar(2)=3
      lhsvar(3)=5
c
      return     
      end

      subroutine intzgesv3(fname)

c     a\b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      external dlamch, zlange
      intrinsic sqrt
c
       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=1
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'z', MA, NA, lA)) return
       if(.not.getrhsvar(2,'z', MB, NRHS, lB)) return
       if(MA .ne. MB) then
         buf='zgesv'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       M = MA
       N = NA
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         lhsvar(1) = 3
         return
       endif

       if(.not.createvar(3,'z', M, N, lAF)) return
       if(.not.createvar(4,'z', N, NRHS, lX)) return
       if(.not.createvar(5,'z', max(M,N), NRHS, lXB)) return
       if(.not.createvar(6,'i', 1, 1, lRANK)) return
       if(.not.createvar(7,'i', 1, N, lIPIV)) return
       if(.not.createvar(8,'i', 1, N, lJPVT)) return
       if(.not.createvar(9,'i', 1, N, lIWORK)) return 
       if(.not.createvar(10,'d',1,2*N,lRWORK)) return       
       LWORKMIN = max( 2*N, min(M,N)+max( 2*min(M,N), N+1,
     $                 min(M,N)+NRHS ) )
       LWORK=maxvol(11,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgesv'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(11,'z',1,LWORK,lDWORK)) return
      
       EPS = dlamch('eps')
       ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )
c
       if(M.eq.N) then
c
c        M = N
c
         call ZLACPY( 'F', N, N, zstk(lA), N, zstk(lAF), N )
c         SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZLACPY( 'F', N, NRHS, zstk(lB), N, zstk(lXB), N )
c         SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZGETRF( N, N, zstk(lAF), N, istk(lIPIV), INFO )         
c         SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
         if(INFO.eq.0) then
           call ZGECON( '1', N, zstk(lAF), N, ANORM, RCOND,
     $             zstk(lDWORK), istk(lIWORK), stk(lRWORK), INFO )
c           SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                       IWORK, RWORK, INFO )
           if(RCOND.gt.sqrt(EPS)) then
              call ZGETRS( 'N', N, NRHS, zstk(lAF), N, istk(lIPIV),
     $                     zstk(lXB), N, INFO ) 
c              SUBROUTINE ZGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c                                 B, LDB, INFO )
              call ZLACPY( 'F', N, NRHS, zstk(lXB), N, zstk(lX), N )
c              SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
              lhsvar(1) = 4
              return
           else
              write(buf(1:13),'(1pd13.4)') RCOND
              call basout(io, wte,
     $          'warning')
              call basout(io, wte,
     $          'matrix is close to singular or badly scaled.')
              call basout(io, wte,
     $          'rcond = '//buf(1:13))
              call basout(io, wte,
     $          'computing least squares solution.')               
           endif
         endif
       endif
c
c      M.ne.N or A  singular
c
       RCOND = sqrt(EPS)
       call ZLACPY( 'F', M, NRHS, zstk(lB), M, zstk(lXB), max(M,N) )
c       SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
       do 10 i = 1, N
   10  istk(lJPVT+i-1) = 0
       call ZGELSY( M, N, NRHS, zstk(lA), M, zstk(lXB), max(M,N),
     $   istk(lJPVT), RCOND, istk(lRANK), zstk(lDWORK),
     $   LWORK, stk(lRWORK), INFO )
c       SUBROUTINE ZGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                    RANK, WORK, LWORK, RWORK, INFO )
       if(info.ne.0) then
         call errorinfo("zgelsy",info)
         return
       endif

       call ZLACPY( 'F', N, NRHS, zstk(lXB), max(M,N), zstk(lX), N )
c       SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )

       lhsvar(1)=4
       return
c
       end


      subroutine intzgesv4(fname)

c     a/b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, zlange
      external dlamch, zlange
      intrinsic conjg, sqrt
c
       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=1
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'z', MA, NA, lA)) return
       if(.not.getrhsvar(2,'z', MB, NB, lB)) return
       if(NA .ne. NB) then
         buf='zgesv'//': A and B must have equal number of columns'
         call error(998)
         return
       endif
       M = MA
       N = NA
       K = MB
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'z', 0, 0, lX)) return
         lhsvar(1) = 3
         return
       endif

       if(.not.createvar(3,'z', N, M, lAF)) return
       if(.not.createvar(4,'z', K, M, lX)) return
       if(.not.createvar(5,'z', N, M, lAT)) return
       if(.not.createvar(6,'z', max(M,N), K, lBT)) return
       if(.not.createvar(7,'i', 1, 1, lRANK)) return
       if(.not.createvar(8,'i', 1, N, lIPIV)) return
       if(.not.createvar(9,'i', 1, M, lJPVT)) return
       if(.not.createvar(10,'i', 1, N, lIWORK)) return
       if(.not.createvar(11,'d',1,2*M,lRWORK)) return    
       LWORKMIN = max( 2*N, min(M,N)+max( 2*min(M,N), M+1,
     $                 min(M,N)+K ))
       LWORK=maxvol(12,'z')
       if(LWORK.le.LWORKMIN) then
         buf='zgelsy'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(12,'z',1,LWORK,lDWORK)) return
       EPS = dlamch('eps')
       ANORM = zlange( '1', M, N, zstk(lA), M, zstk(lDWORK) )
c
c        Transpose A and B
c
         do 20 j = 1, M
            do 10 i = 1, N
               ij = i+(j-1)*N
               ji = j+(i-1)*M
               zstk(lAT+ij-1) = conjg( zstk(lA+ji-1) )
 10         continue
 20      continue 
         do 40 j = 1, K
            do 30 i = 1, N
               ij = i+(j-1)*max(M,N)
               ji = j+(i-1)*K
               zstk(lBT+ij-1) = conjg( zstk(lB+ji-1) )
 30         continue
 40      continue 
c
       if(M.eq.N) then
c
c        M = N
c
         call ZLACPY( 'F', N, N, zstk(lAT), N, zstk(lAF), N )
c         SUBROUTINE ZLACPY( UPLO, M, N, A, LDA, B, LDB )
         call ZGETRF( N, N, zstk(lAF), N, istk(lIPIV), INFO )         
c         SUBROUTINE ZGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
           call ZGECON( '1', N, zstk(lAF), N, ANORM, RCOND,
     $             zstk(lDWORK), istk(lIWORK), stk(lRWORK), INFO )
c           SUBROUTINE ZGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        IWORK, RWORK, INFO )
           if(RCOND.gt.sqrt(EPS)) then
              call ZGETRS( 'N', N, K, zstk(lAF), N, istk(lIPIV),
     $                     zstk(lBT), N, INFO ) 
c              SUBROUTINE ZGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c                                 B, LDB, INFO )
c
c             Back transpose X
c
              do 60 j = 1, N
                 do 50 i = 1, K
                    ij = i+(j-1)*K
                    ji = j+(i-1)*N
                    zstk(lX+ij-1) = conjg( zstk(lBT+ji-1) )
 50              continue
 60           continue
              lhsvar(1) = 4
              return
           endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
         call basout(io, wte,
     $     'warning')
         call basout(io, wte,
     $     'matrix is close to singular or badly scaled.')
         call basout(io, wte,
     $     'rcond = '//buf(1:13))
         call basout(io, wte,
     $     'computing least squares solution.')               
       endif
c
c      M.ne.N or A singular
c
       RCOND = sqrt(eps)
       do 70 i = 1, M
   70  istk(lJPVT+i-1) = 0
       call ZGELSY( N, M, K, zstk(lAT), N, zstk(lBT), max(M,N),
     $              istk(lJPVT), RCOND, istk(lRANK), zstk(lDWORK),
     $              LWORK, stk(lRWORK), INFO )
c        SUBROUTINE ZGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                     RANK, WORK, LWORK, RWORK, INFO )
       if(info.ne.0) then
         call errorinfo("zgelsy",info)
         return
       endif
       write(buf(1:5),'(1pI5)') istk(lRANK)
       if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call basout(io, wte,
     $     'rank defficient.')
         call basout(io, wte,
     $     'rank = '//buf(1:5))
       endif
c
c      Back transpose X
c
       do 90 j = 1, M
          do 80 i = 1, K
             ij = i+(j-1)*K
             ji = j+(i-1)*max(M,N)
             zstk(lX+ij-1) = conjg( zstk(lBT+ji-1) )
 80       continue
 90    continue 

       lhsvar(1)=4
       return
c
       end



      subroutine intdgesv4(fname)

c     a/b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, dlange
      external dlamch, dlange
      intrinsic sqrt
c
       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=1
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(.not.getrhsvar(2,'d', MB, NB, lB)) return
       if(NA .ne. NB) then
         buf='dgesv'//': A and B must have equal number of columns'
         call error(998)
         return
       endif
       M = MA
       N = NA
       K = MB
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'d', 0, 0, lX)) return
         lhsvar(1) = 3
         return
       endif

       if(.not.createvar(3,'d', N, M, lAF)) return
       if(.not.createvar(4,'d', K, M, lX)) return
       if(.not.createvar(5,'d', N, M, lAT)) return
       if(.not.createvar(6,'d', max(M,N), K, lBT)) return
       if(.not.createvar(7,'i', 1, 1, lRANK)) return
       if(.not.createvar(8,'i', 1, N, lIPIV)) return
       if(.not.createvar(9,'i', 1, M, lJPVT)) return
       if(.not.createvar(10,'i', 1, N, lIWORK)) return
       LWORKMIN = max( 4*N, min(M,N)+3*M+1, 2*min(M,N)+K )
       LWORK=maxvol(11,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgelsy'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(11,'d',1,LWORK,lDWORK)) return
       EPS = dlamch('eps')
       ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
c
c        Transpose A and B
c
         do 20 j = 1, M
            do 10 i = 1, N
               ij = i+(j-1)*N
               ji = j+(i-1)*M
               stk(lAT+ij-1) = stk(lA+ji-1)
 10         continue
 20      continue 
         do 40 j = 1, K
            do 30 i = 1, N
               ij = i+(j-1)*max(M,N)
               ji = j+(i-1)*K
               stk(lBT+ij-1) = stk(lB+ji-1)
 30         continue
 40      continue 
c
       if(M.eq.N) then
c
c        M = N
c
         call DLACPY( 'F', N, N, stk(lAT), N, stk(lAF), N )
c         SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
         call DGETRF( N, N, stk(lAF), N, istk(lIPIV), INFO )         
c         SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
           call DGECON( '1', N, stk(lAF), N, ANORM, RCOND, stk(lDWORK),
     $                  istk(lIWORK), INFO )
c           SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        IWORK, INFO )
           if(RCOND.gt.sqrt(EPS)) then
              call DGETRS( 'N', N, K, stk(lAF), N, istk(lIPIV),
     $                     stk(lBT), N, INFO ) 
c              SUBROUTINE DGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c                                 B, LDB, INFO )
c
c             Back transpose X
c
              do 60 j = 1, N
                 do 50 i = 1, K
                    ij = i+(j-1)*K
                    ji = j+(i-1)*N
                    stk(lX+ij-1) = stk(lBT+ji-1)
 50              continue
 60           continue
              lhsvar(1) = 4
              return
           endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
         call basout(io, wte,
     $     'warning')
         call basout(io, wte,
     $     'matrix is close to singular or badly scaled.')
         call basout(io, wte,
     $     'rcond = '//buf(1:13))
         call basout(io, wte,
     $     'computing least squares solution.')               
       endif
c
c      M.ne.N or A singular
c
       RCOND = sqrt(eps)
       do 70 i = 1, M
   70  istk(lJPVT+i-1) = 0
       call DGELSY( N, M, K, stk(lAT), N, stk(lBT), max(M,N),
     $              istk(lJPVT), RCOND, istk(lRANK), stk(lDWORK),
     $              LWORK, INFO )
c        SUBROUTINE DGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                     RANK, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dgelsy",info)
         return
       endif
       write(buf(1:5),'(1pI5)') istk(lRANK)
       if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call basout(io, wte,
     $     'rank defficient.')
         call basout(io, wte,
     $     'rank = '//buf(1:5))
       endif
c
c      Back transpose X
c
       do 90 j = 1, M
          do 80 i = 1, K
             ij = i+(j-1)*K
             ji = j+(i-1)*max(M,N)
             stk(lX+ij-1) = stk(lBT+ji-1)
 80       continue
 90    continue 

       lhsvar(1)=4
       return
c
       end


      subroutine intdgesv3(fname)

c     a\b

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      character fname*(*)
      double precision ANORM, EPS, RCOND
      double precision dlamch, dlange
      external dlamch, dlange
      intrinsic sqrt
c
       minrhs=2
       maxrhs=2
       minlhs=1
       maxlhs=1
c
       if(.not.checkrhs(fname,minrhs,maxrhs)) return
       if(.not.checklhs(fname,minlhs,maxlhs)) return
        
       if(.not.getrhsvar(1,'d', MA, NA, lA)) return
       if(.not.getrhsvar(2,'d', MB, NRHS, lB)) return
       if(MA .ne. MB) then
         buf='dgesv'//': A and B must have equal number of rows'
         call error(998)
         return
       endif
       M = MA
       N = NA
       if(M.eq.0 .or. N.eq.0) then
         if(.not.createvar(3,'d', 0, 0, lX)) return
         lhsvar(1) = 3
         return
       endif

       if(.not.createvar(3,'d', M, N, lAF)) return
       if(.not.createvar(4,'d', N, NRHS, lX)) return
       if(.not.createvar(5,'d', max(M,N), NRHS, lXB)) return
       if(.not.createvar(6,'i', 1, 1, lRANK)) return
       if(.not.createvar(7,'i', 1, N, lIPIV)) return
       if(.not.createvar(8,'i', 1, N, lJPVT)) return
       if(.not.createvar(9,'i', 1, N, lIWORK)) return       
       LWORKMIN = max( 4*N, min(M,N)+3*N+1, 2*min(M,N)+NRHS )
       LWORK=maxvol(10,'d')
       if(LWORK.le.LWORKMIN) then
         buf='dgesv'//': not enough memory (use stacksize)'
         call error(998)
         return
       endif
       if(.not.createvar(10,'d',1,LWORK,lDWORK)) return
      
       EPS = dlamch('eps')
       ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
c
       if(M.eq.N) then
c
c        M = N
c
         call DLACPY( 'F', N, N, stk(lA), N, stk(lAF), N )
c         SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
         call DGETRF( N, N, stk(lAF), N, istk(lIPIV), INFO )         
c         SUBROUTINE DGETRF( N, N, A, LDA, IPIV, INFO )
         RCOND = 0.0d0
         if(INFO.eq.0) then
           call DGECON( '1', N, stk(lAF), N, ANORM, RCOND, stk(lDWORK),
     $                  istk(lIWORK), INFO )
c           SUBROUTINE DGECON( NORM, N, A, LDA, ANORM, RCOND, WORK,
c     $                        IWORK, INFO )
           if(RCOND.gt.sqrt(EPS)) then
              call DGETRS( 'N', N, NRHS, stk(lAF), N, istk(lIPIV),
     $                     stk(lB), N, INFO ) 
c              SUBROUTINE DGETRS( TRANS, N, NRHS, A, LDA, IPIV,
c                                 B, LDB, INFO )
              call DLACPY( 'F', N, NRHS, stk(lB), N, stk(lX), N )
c              SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
              lhsvar(1) = 4
              return
           endif
         endif
         write(buf(1:13),'(1pd13.4)') RCOND
         call basout(io, wte,
     $     'warning')
         call basout(io, wte,
     $     'matrix is close to singular or badly scaled.')
         call basout(io, wte,
     $     'rcond = '//buf(1:13))
         call basout(io, wte,
     $     'computing least squares solution.')               
       endif
c
c      M.ne.N or A singular
c
       RCOND = sqrt(EPS)
       call DLACPY( 'F', M, NRHS, stk(lB), M, stk(lXB), max(M,N) )
c       SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )
       do 10 i = 1, N
   10  istk(lJPVT+i-1) = 0
       call DGELSY( M, N, NRHS, stk(lA), M, stk(lXB), max(M,N),
     $   istk(lJPVT), RCOND, istk(lRANK), stk(lDWORK),
     $   LWORK, INFO )
c       SUBROUTINE DGELSY( M, N, NRHS, A, LDA, B, LDB, JPVT, RCOND,
c     $                    RANK, WORK, LWORK, INFO )
       if(info.ne.0) then
         call errorinfo("dgelsy",info)
         return
       endif
       write(buf(1:5),'(1pI5)') istk(lRANK)
       if( M.ne.N .and. istk(lRANK).lt.min(M,N) )then
         call basout(io, wte,
     $     'rank defficient.')
         call basout(io, wte,
     $     'rank = '//buf(1:5))
       endif
       call DLACPY( 'F', N, NRHS, stk(lXB), max(M,N), stk(lX), N )
c       SUBROUTINE DLACPY( UPLO, M, N, A, LDA, B, LDB )

       lhsvar(1)=4
       return
c
       end
































































