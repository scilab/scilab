
      subroutine intdgeev(fname)

c     [V,D]=spec(A)
c     d = spec(A)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBVL, JOBVR
      double precision DZERO
      parameter ( DZERO=0.0D0 )
      complex*16 ZERO
      parameter ( ZERO=(0.0D0,0.0D0) )
      INTRINSIC  DCMPLX

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
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif 
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
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
         err=(LWORK-LWORKMIN)
         call error(17)
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
c     SUBROUTINE DGEEV( JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR,
c     LDVR, 
c     $     WORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.eq.1) then
         call ZLASET( 'F', N, 1, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 5 i = 1, N
            zstk(lD+i-1) = dcmplx(stk(lWR+i-1),stk(lWI+i-1))
 5       continue     
      else 
         call ZLASET( 'F', N, N, ZERO, ZERO, zstk(lD), N ) 
c     SUBROUTINE ZLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 10 i = 1, N
            ii = i+(i-1)*N
            zstk(lD+ii-1) = dcmplx(stk(lWR+i-1),stk(lWI+i-1))
 10      continue   
         
         j = 0
 20      j = j+1
         if(stk(lWI+j-1).eq.DZERO) then
            do 30 i = 1, N
               ij = i+(j-1)*N
               zstk(lV+ij-1) = stk(lVR+ij-1)
 30         continue
         else   
            do 40 i = 1, N
               ij = i+(j-1)*N
               ij1 = i+j*N
               zstk(lV+ij-1) = dcmplx(stk(lVR+ij-1),stk(lVR+ij1-1))
               zstk(lV+ij1-1) = dcmplx(stk(lVR+ij-1),-stk(lVR+ij1-1)) 
 40         continue
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
