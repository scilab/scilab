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

      include 'stack.h'
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

      include 'stack.h'
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

      include 'stack.h'
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

      include 'stack.h'
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



























