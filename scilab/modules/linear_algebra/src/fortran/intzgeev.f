       subroutine intzgeev(fname)

c     [V,D]=spec(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite
      
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
         err=1
         call error(20)
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
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif  
      if(vfinite(2*M*N,zstk(lA)).eq.0) then
         err=1
         call error(264)
         return
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
         err=2*(LWORK-LWORKMIN)
         call error(17)
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
c     SUBROUTINE ZGEEV( JOBVL, JOBVR, N, A, LDA, W, VL, LDVL,
c     $     VR, LDVR, WORK, LWORK, RWORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.gt.1) then
         call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, N
            ii = i+(i-1)*N
            zstk(lD+ii-1) = zstk(lW+i-1)
 10      continue 
      else
         do 11 i = 1, N
            zstk(lD+i-1) = zstk(lW+i-1)
 11      continue
      endif
      
      if(lhs.eq.1) then
         lhsvar(1) = 2
      else
         lhsvar(1)=3
         lhsvar(2)=2
      endif
c     
      end
