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
         err=1
         call error(20)
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
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
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
         err=2*(LWORK-LWORKMIN)
         call error(17)
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
c     SUBROUTINE ZGEES( JOBVS, SORT, SELECT, N, A, LDA, SDIM, W,
c     $    VS, LDVS, WORK, LWORK, RWORK, BWORK, INFO )
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
         err=1
         call error(20)
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
      elseif(N.eq.-1) then
        if(lhs.eq.1) then
           lhsvar(1) = 1
        else if(lhs.eq.2) then
           if(.not.createvar(2,'d', N, N, lQ)) return
           stk(lQ)=stk(lA)
           stk(lA)=1.0d0
           lhsvar(1)=1
           lhsvar(2)=2
        endif 
        return
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
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return
      call DGEHRD( N, 1, N, stk(lA), N, stk(lTAU), stk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE DGEHRD( N, 1, N, A, LDA, TAU, WORK, INFO )

      if(lhs.gt.1) call DLACPY( 'F', N, N, stk(lA), N, stk(lQ), N )
c     SUBROUTINE DLACPY( UPLO, N, N, A, LDA, B, LDB )
      if(N.gt.2) then
         do 20 j = 1, N-2
            do 10 i = j+2, N
               ij = i+(j-1)*N
               stk(lA+ij-1) = ZERO
 10         continue
 20      continue
      endif
      
      if(lhs.gt.1) then
         call DORGHR( N, 1, N, stk(lQ), N, stk(lTAU), stk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE DORGHR( N, 1, N, A, LDA, TAU, WORK, INFO ) 
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
      complex*16 ZERO,ONE
      parameter ( ZERO=(0.0D0,0.0D0), ONE=(1.0D0,0.0D0) )

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
            lhsvar(1) = 1
            return
         else if(lhs.eq.2) then
            if(.not.createvar(2, 'z', N, N, lQ)) return
            lhsvar(1)=1
            lhsvar(2)=2
            return
         endif
      elseif(N.eq.-1) then
        if(lhs.eq.1) then
           lhsvar(1) = 1
        else if(lhs.eq.2) then
           if(.not.createvar(2,'z', N, N, lQ)) return
           zstk(lQ)=zstk(lA)
           zstk(lA)=ONE
           lhsvar(1)=1
           lhsvar(2)=2
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
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'z',1,LWORK,lDWORK)) return
      call ZGEHRD( N, 1, N, zstk(lA), N, zstk(lTAU), zstk(lDWORK),
     $     LWORK, INFO )
c     SUBROUTINE ZGEHRD( N, 1, N, A, LDA, TAU, WORK, INFO )

      if(lhs.gt.1) call ZLACPY( 'F', N, N, zstk(lA), N, zstk(lQ), N )
c     SUBROUTINE ZLACPY( UPLO, N, N, A, LDA, B, LDB )
      if(N.gt.2) then
         do 20 j = 1, N-2
            do 10 i = j+2, N
               ij = i+(j-1)*N
               zstk(lA+ij-1) = ZERO
 10         continue
 20      continue
      endif
      
      if(lhs.gt.1) then
         call ZUNGHR( N, 1, N, zstk(lQ), N, zstk(lTAU), zstk(lDWORK),
     $        LWORK, INFO )
c     SUBROUTINE ZUNGHR( N, 1, N, A, LDA, TAU, WORK, INFO ) 
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
      logical getrhsvar,createvar,createcvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOBVL, JOBVR
      double precision DZERO
      complex*16 complex
      parameter ( DZERO=0.0D0 )

      minrhs=2
      maxrhs=2
      minlhs=1
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         lhsvar(2)=2
         if(lhs.ge.3) then
            if(.not.createvar(3,'d', N, N, lVR   )) return
            lhsvar(3)=3
         endif
         if(lhs.eq.4) then
            if(.not.createvar(4,'d', N, N, lVL)) return
            lhsvar(4)=4
         endif
         return
      endif
      if(vfinite(N*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif  
      if(vfinite(N*N,stk(lB)).eq.0) then
         err=2
         call error(264)
         return
      endif  

      if(.not.createcvar(3,'d',1, N, 1, lALPHAR,lALPHAI)) return
      if(.not.createvar(4,'d', N, 1, lBETA)) return
      k=5
      if(lhs.ge.3) then
         if(.not.createcvar(5,'d',1, N, N, lVRR, lVRI)) return
         k = 6            
      endif
      if(lhs.eq.4) then
         if(.not.createcvar(6,'d',1, N, N, lVLR, lVLI)) return
         k = 7           
      endif

      LWORKMIN = max(1,8*N)
      LWORK=maxvol(k,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k,'d',1,LWORK,lDWORK)) return
      if(lhs.eq.1.or.lhs.eq.2) then
         JOBVL = 'N'
         JOBVR = 'N'
         lVR = lDWORK
         lVL = lDWORK
      elseif(lhs.eq.3) then
         JOBVR = 'V'
         JOBVL = 'N'
         lVL = lDWORK
      else
         JOBVR = 'V'
         JOBVL = 'V'
      endif 
      call DGGEV( JOBVL, JOBVR, N, stk(lA), N, stk(lB), N, stk(lALPHAR),
     $     stk(lALPHAI), stk(lBETA), stk(lVLR), N, stk(lVRR), N,
     $     stk(lDWORK), LWORK, INFO )
c     SUBROUTINE DGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHAR,
c     $     ALPHAI, BETA, VL, LDVL, VR, LDVR, WORK, LWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         endif
      endif

      if(lhs.eq.1) then
         do 15 i = 1, N
            stk(lALPHAR-1+i)=stk(lALPHAR-1+i)/stk(lBETA-1+i)
            stk(lALPHAI-1+i)=stk(lALPHAI-1+i)/stk(lBETA-1+i)
 15      continue
         lhsvar(1)=3
         return
      endif
      if(lhs.eq.2) then
         lhsvar(1)=3
         lhsvar(2)=4
         return
      endif
      if(lhs.ge.3) then
         call dset(N*N,0.0d0,stk(lVRI),1)
         j = 0
 20      j = j+1
         if(stk(lALPHAI+j-1).ne.DZERO) then
            do 30 i = 1, N
               ij = i+(j-1)*N
               ij1 = i+j*N
               stk(lVRI+ij-1) = stk(lVRR+ij1-1)
               stk(lVRI+ij1-1) = -stk(lVRR+ij1-1)
               stk(lVRR+ij1-1) = stk(lVRR+ij-1)
 30         continue
            j = j+1
         endif
         if(j.lt.N) go to 20
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
      endif
      if(lhs.eq.4) then
         call dset(N*N,0.0d0,stk(lVLI),1)
         j = 0
 35      j = j+1
         if(stk(lALPHAI+j-1).ne.DZERO) then
            do 40 i = 1, N
               ij = i+(j-1)*N
               ij1 = i+j*N
               stk(lVLI+ij-1) = stk(lVLR+ij1-1)
               stk(lVLI+ij1-1) = -stk(lVLR+ij1-1)
               stk(lVLR+ij1-1) = stk(lVLR+ij-1)
 40         continue
            j = j+1
         endif
         if(j.lt.N) go to 35
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=6
         lhsvar(4)=5
      endif
c     
      end



      subroutine intzggev(fname)

c     [al,be,V]=gspec(A,B)
c     [al,be]=gspec(A,B)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite
      

      character fname*(*)
      character JOBVL, JOBVR

      minrhs=2
      maxrhs=2
      minlhs=1
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
         if(.not.createvar(3,'z', N, 1, lALPHA)) return
         if(.not.createvar(4,'z', N, 1, lBETA)) return
         lhsvar(1)=3
         lhsvar(2)=4
         if(lhs.ge.3) then
            if(.not.createvar(5,'z', N, N, lVR)) return
            lhsvar(3)=5
         endif
         if(lhs.eq.4) then
            if(.not.createvar(6,'z', N, N, lVL)) return
            lhsvar(4)=6
         endif
         return
      endif
      if(vfinite(2*N*N,zstk(lA)).eq.0) then
         err=1
         call error(264)
         return
      endif  
      if(vfinite(2*N*N,zstk(lB)).eq.0) then
         err=2
         call error(264)
         return
      endif
      if(.not.createvar(3,'z', N, 1, lALPHA)) return
      if(.not.createvar(4,'z', N, 1, lBETA)) return
      k = 5
      if(lhs.ge.3) then
         if(.not.createvar(5,'z', N, N, lVR)) return
         k = 6
      endif
      if(lhs.eq.4) then
         if(.not.createvar(6,'z', N, N, lVL)) return
         k = 7
      endif
      if(.not.createvar(k,'d', 8*N, 1, lRWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+1,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return
      if(lhs.eq.1.or.lhs.eq.2) then
         JOBVL = 'N'
         JOBVR = 'N'
         lVR = lDWORK
         lVL = lDWORK
      elseif(lhs.eq.3) then
         JOBVR = 'V'
         JOBVL = 'N'
         lVL = lDWORK
      else
         JOBVR = 'V'
         JOBVL = 'V'
      endif 
      call ZGGEV( JOBVL, JOBVR, N, zstk(lA), N, zstk(lB),N,
     $     zstk(lALPHA), zstk(lBETA), zstk(lVL), N, zstk(lVR), N,
     $     zstk(lDWORK), LWORK, stk(lRWORK), INFO )
c     SUBROUTINE ZGGEV( JOBVL, JOBVR, N, A, LDA, B, LDB, ALPHA, BETA,
c     $     VL, LDVL, VR, LDVR, WORK, LWORK, RWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
         endif
      endif
      if(lhs.eq.1) then
         do 15 i = 1, N
            zstk(lALPHA-1+i)=zstk(lALPHA-1+i)/zstk(lBETA-1+i)
 15      continue
         lhsvar(1)=3
      elseif(lhs.eq.2) then
         lhsvar(1)=3
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=5
      else
         lhsvar(1)=3
         lhsvar(2)=4
         lhsvar(3)=6
         lhsvar(4)=5
      endif
c     
      end


      subroutine intdsyev(fname)

c     [D,V]=spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite

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
      if(vfinite(M*N,stk(lA)).eq.0) then
         err=1
         call error(264)
         return
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
         err=(LWORK-LWORKMIN)
         call error(17)
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
c     SUBROUTINE DSYEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK, INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.eq.1) then
         do 10 i = 1, N
            stk(lD+i-1) = stk(lW+i-1)
 10      continue   
         lhsvar(1)=2  
      else 
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lD), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 20 i = 1, N
            ii = i+(i-1)*N
            stk(lD+ii-1) = stk(lW+i-1)
 20      continue   
         lhsvar(1)=1
         lhsvar(2)=2

      endif 
c     
      end

      subroutine intzheev(fname)

c     [D,V]=spec(A)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      integer vfinite 

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
            lhsvar(1) = 1
            lhsvar(2) = 2
            return
         endif
      endif
      if(vfinite(2*M*N,zstk(lA)).eq.0) then
         err=1
         call error(264)
         return
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
         err=2*(LWORK-LWORKMIN)
         call error(17)
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
c     SUBROUTINE ZHEEV( JOBZ, UPLO, N, A, LDA, W, WORK, LWORK,
c     $    stk(lROWK), INFO )
      if(info.ne.0) then
         call error(24)
         return
      endif

      if(lhs.eq.1) then
         do 10 i = 1, N
            stk(lD+i-1) = stk(lW+i-1)
 10      continue  
         lhsvar(1)=2   
      else 
         call DLASET( 'F', N, N, ZERO, ZERO, stk(lD), N ) 
c     SUBROUTINE DLASET( UPLO, M, N, ALPHA, BETA, A, LDA )

         do 20 i = 1, N
            ii = i+(i-1)*N
            stk(lD+ii-1) = stk(lW+i-1)
 20      continue   
         lhsvar(1)=1
         lhsvar(2)=2
      endif 
c     
      end


      subroutine intdgges(fname)

c     [AS,BS,VSL,VSR]=gschur(A,B)
c     [AS,BS] = gschur(A,B)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      
      character fname*(*)
      character JOBVSL, JOBVSR, SORT
      logical voiddummy
      external voiddummy

      minrhs=2
      maxrhs=2
      minlhs=2
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         lhsvar(2)=2
         if(lhs.eq.4) then
            if(.not.createvar(3,'d', N, N, lVSL)) return
            if(.not.createvar(4,'d', N, N, lVSR)) return
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(3,'i', 1, 1, lSDIM)) return
      if(.not.createvar(4,'d', N, 1, lALPHAR)) return
      if(.not.createvar(5,'d', N, 1, lALPHAI)) return
      if(.not.createvar(6,'d', N, 1, lBETA)) return
      k = 7              
      if(lhs.eq.4) then
         if(.not.createvar(7,'d', N, N, lVSL)) return
         if(.not.createvar(8,'d', N, N, lVSR)) return
         k = 9
      endif
      if(.not.createvar(k,'i', 2*N, 1, lBWORK)) return

      LWORKMIN = 8*N+16
      LWORK=maxvol(k+1,'d')
      if(LWORK.le.LWORKMIN) then
         err=(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'d',1,LWORK,lDWORK)) return

      if(lhs.eq.2) then
         JOBVSL = 'N'       
         JOBVSR = 'N'
         lVSL = lDWORK
         lVSR = lDWORK
      else
         JOBVSL = 'V'
         JOBVSR = 'V'
      endif
      SORT = 'N' 
      call DGGES( JOBVSL, JOBVSR, SORT, voiddummy, N, stk(lA), N, 
     $     stk(lB),
     $     N, istk(lSDIM), stk(lALPHAR), stk(lALPHAI), stk(lBETA),
     $     stk(lVSL), N, stk(lVSR), N, stk(lDWORK), LWORK, istk(lBWORK),
     $     INFO )
c     SUBROUTINE DGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $     SDIM, ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR, WORK,
c     $     LWORK, BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         endif
      endif

      
      if(lhs.eq.2) then
         lhsvar(1)=1
         lhsvar(2)=2
      else 
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=7
         lhsvar(4)=8
      endif
c     
      end


      subroutine intzgges(fname)

c     [AS,BS,VSL,VSR]=gschur(A,B)
c     [AS,BS]=gschur(A,B)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      

      character fname*(*)
      character JOBVSL, JOBVSR, SORT
      logical voiddummy
      external voiddummy

      minrhs=2
      maxrhs=2
      minlhs=2
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
         lhsvar(1)=1
         lhsvar(2)=2
         if(lhs.eq.4) then
            if(.not.createvar(3,'d', N, N, lVSL)) return
            if(.not.createvar(4,'d', N, N, lVSR)) return
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.createvar(3,'i', 1, 1, lSDIM)) return
      if(.not.createvar(4,'z', N, 1, lALPHA)) return
      if(.not.createvar(5,'z', N, 1, lBETA)) return
      K = 6
      if(lhs.eq.4) then
         if(.not.createvar(6,'z', N, N, lVSL)) return
         if(.not.createvar(7,'z', N, N, lVSR)) return
         k = 8
      endif
      if(.not.createvar(k,'d', 8*N, 1, lRWORK)) return
      k=k+1
      if(.not.createvar(k,'i', 2*N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=maxvol(k+1,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(k+1,'z',1,LWORK,lDWORK)) return


      if(lhs.eq.2) then
         JOBVSL = 'N'
         JOBVSR = 'N'
         lVSL = lDWORK
         lVSR = lDWORK
      else
         JOBVSL = 'V'
         JOBVSR = 'V'
      endif
      SORT = 'N'
      call ZGGES( JOBVSL, JOBVSR, SORT, voiddummy, N, 
     $     zstk(lA), N,
     $     zstk(lB), N, istk(lSDIM), zstk(lALPHA), zstk(lBETA),
     $     zstk(lVSL), N, zstk(lVSR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), istk(lBWORK), INFO )
c     SUBROUTINE ZGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $    SDIM, ALPHA, BETA, VSL, LDVSL, VSR, LDVSR, WORK, LWORK, RWORK
c     ,
c     $    BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         endif
      endif

      if(lhs.eq.2) then
         lhsvar(1)=1
         lhsvar(2)=2
      else
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=6
         lhsvar(4)=7
      endif
c     
      end



      subroutine intozgschur(fname)

c     [As,Bs,VSR,dim]=gshur(A,B,function)
c     [VS,dim]=gshur(A,B,function)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      
      character fname*(*)
      character  JOBVSL, JOBVSR, SORT
      logical SCIZGSHR,scizgchk
      external SCIZGSHR,scizgchk
      common /scizgsch/ lf, nfree, nf

      minrhs=3
      maxrhs=3
      minlhs=1
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
      N = MA 
      if(N.eq.0) then
         lhsvar(1)=1
         lhsvar(2)=2
         if(lhs.eq.4) then
            if(.not.createvar(3,'z', N, N, lVSR)) return
            if(.not.createvar(4,'d', N, N, lSDIM)) return
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      nf=3
      if(.not.getrhsvar(nf,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.2) then
         err=nf
         call error(80)
         return
      endif

      if(.not.createvar(4,'i', 1, 1, lSDIM)) return
      if(.not.createvar(5,'z', N, 1, lALPHA)) return
      if(.not.createvar(6,'z', N, 1, lBETA)) return
      if(.not.createvar(7,'z', N, N, lVSL)) return
      if(.not.createvar(8,'z', N, N, lVSR)) return
      if(.not.createvar(9,'d', 8*N, 1, lRWORK)) return
      if(.not.createvar(10,'i', 2*N, 1, lBWORK)) return
      LWORKMIN = 2*N
      LWORK=LWORKMIN
      if(.not.createvar(11,'z',1,LWORK,lDWORK)) return
      nfree = 12
      
      JOBVSL = 'V'
      JOBVSR = 'V' 
      SORT = 'S'

      
      if(.not.scizgchk()) return
      call ZGGES( JOBVSL, JOBVSR, SORT, SCIZGSHR, N, zstk(lA), N,
     $     zstk(lB), N, istk(lSDIM), zstk(lALPHA), zstk(lBETA),
     $     zstk(lVSL), N, zstk(lVSR), N, zstk(lDWORK), LWORK,
     $     stk(lRWORK), istk(lBWORK), INFO )
c     SUBROUTINE ZGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $    SDIM, ALPHA, BETA, VSL, LDVSL, VSR, LDVSR, WORK, LWORK, RWORK
c     ,
c     $    BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         elseif(info.eq.N+2) then 
            call msgs(103,0)
         elseif(info.eq.N+3) then 
            buf='reordering failed'
            call error(1002)
            return
         endif
      endif

      if(lhs.eq.1) then
         lhsvar(1)=4
      elseif(lhs.eq.2) then
         lhsvar(1)=8
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=7
         lhsvar(2)=8
         lhsvar(3)=4
      else if(lhs.eq.4) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=8
         lhsvar(4)=4
      endif
c     
      end

      
      logical function scizgshr(alpha,beta)
      INCLUDE '../stack.h'
      logical scifunction, createcvar
      common /scizgsch/ lf, nx, nf
      integer iadr
      complex*16 alpha, beta
      intrinsic dreal, dimag

      iadr(l) = l+l-1
c     
      scizgshr=.false.
      if(.not.createcvar(nx,'d',1,1,1,lar,lai)) return
      if(.not.createcvar(nx+1,'d',1,1,1,lbr,lbi)) return
      lx=lar
      stk(lar)=dreal(alpha)
      stk(lai)=dimag(alpha)
      stk(lbr)=dreal(beta)
      stk(lbi)=dimag(beta)

      if(.not.scifunction(nx,lf,1,2)) return
c     stk(lx)=fct([alpha,beta])  evaluated by scilab fct pointed to by
c     lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scizgshr=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scizgshr=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scizgchk()
c     checks fct passed to zgshur
      INCLUDE '../stack.h'
      logical scifunction, createcvar
      
      integer iadr
      common/ierinv/iero
      common /scizgsch/ lf, nx, nf
      iadr(l) = l+l-1

      scizgchk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lar,lai)) return
      if(.not.createcvar(nx+1,'d',1,1,1,lbr,lbi)) return
      lx=lar
      stk(lar)=1.0D0
      stk(lai)=0.0d0
      stk(lbr)=1.0D0
      stk(lbi)=0.0d0
      if(.not.scifunction(nx,lf,1,1)) then
c     error into fct passed to schur (zgschur(A,B,tst))
         return
      endif
c     check return value of fct
      ilx=iadr(lx-2)
      if(istk(ilx).ne.1 .and. istk(ilx).ne.4) then
         err=nf
         call error(268)
         return
      endif
      if(istk(ilx+1)*istk(ilx+2).ne.1) then
         err=nf
         call error(268)
         return
      endif
      scizgchk=.true.
      return
      end

      subroutine intogschur(fname)
c     [dim]=gschur(A,B,function)
c     [VSR,dim]=gschur(A,B,function)
c     [VSL,VSR,dim]=gschur(A,B,function)
c     [As,Bs,VSR,dim]=gschur(A,B,function)
c     [VSR,dim]=gschur(A,B,function)

      include '../stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs

      character fname*(*)
      character  JOBVSL, JOBVSR, SORT
      logical SCIGSHUR,scigchk
      external SCIGSHUR,scigchk
      common /scigsch/ lf, nfree, nf

      minrhs=3
      maxrhs=3
      minlhs=1
      maxlhs=5
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif

      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
         call error(267)
         return
      endif
      N = MA
      if(N.eq.0) then
         lhsvar(1)=1
         if(lhs.eq.2) then
            if(.not.createvar(2,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
         elseif(lhs.eq.3) then
            if(.not.createvar(3,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
         elseif(lhs.eq.4) then
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
         else
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', N, N, lVSL)) return
            if(.not.createvar(5,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
            lhsvar(5)=5
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      nf=3
      if(.not.getrhsvar(nf,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.2) then
         err=nf
         call error(80)
         return
      endif

      if(.not.createvar(4,'i', 1, 1, lSDIM)) return
      if(.not.createvar(5,'d', N, 1, lALPHAR)) return
      if(.not.createvar(6,'d', N, 1, lALPHAI)) return
      if(.not.createvar(7,'d', N, 1, lBETA)) return              
      if(.not.createvar(8,'d', N, N, lVSL)) return
      if(.not.createvar(9,'d', N, N, lVSR)) return
      if(.not.createvar(10,'i', 2*N, 1, lBWORK)) return

      LWORKMIN = 7*(N+1)+16
      LWORK=LWORKMIN
      if(.not.createvar(11,'d',1,LWORK,lDWORK)) return
      nfree = 12

      JOBVSL = 'V'
      JOBVSR = 'V'
      SORT = 'S'
c     
      if(.not.scigchk()) return
      call DGGES( JOBVSL, JOBVSR, SORT, SCIGSHUR, N, stk(lA), N,
     $     stk(lB), N, istk(lSDIM), stk(lALPHAR), stk(lALPHAI),
     $     stk(lBETA), stk(lVSL), N, stk(lVSR), N, stk(lDWORK), LWORK,
     $     istk(lBWORK),INFO )


c     SUBROUTINE DGGES( JOBVSL, JOBVSR, SORT, DELCTG, N, A, LDA, B, LDB,
c     $     SDIM, ALPHAR, ALPHAI, BETA, VSL, LDVSL, VSR, LDVSR, WORK,
c     $     LWORK, BWORK, INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(1,info)
         elseif(info.eq.N+1) then
            call error(24)
            return
         elseif(info.eq.N+2) then 
            call msgs(103,0)
         elseif(info.eq.N+3) then 
            buf='reordering failed'
            call error(1002)
            return
         endif
      endif

      if(lhs.eq.1) then
         lhsvar(1)=4
      elseif(lhs.eq.2) then
         lhsvar(1)=9
         lhsvar(2)=4
      elseif(lhs.eq.3) then
         lhsvar(1)=8
         lhsvar(2)=9
         lhsvar(3)=4
      else if(lhs.eq.4) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=9
         lhsvar(4)=4
      else if(lhs.eq.5) then
         lhsvar(1)=1
         lhsvar(2)=2
         lhsvar(3)=8
         lhsvar(4)=9
         lhsvar(5)=4
      endif
      end

      subroutine intdggbal(fname)
c     [Ab,Bb,X,Y]=balanc(A,B)

      include '../stack.h'
      logical getrhsvar,createvar,createcvar
      logical checklhs,checkrhs
      integer vfinite

      character fname*(*)
      character JOB
      double precision ZERO, ONE
      parameter ( ZERO=0.0D0, ONE=1.0D0 )

      minrhs=2
      maxrhs=2
      minlhs=4
      maxlhs=4
c     
      if(.not.checkrhs(fname,minrhs,maxrhs)) return
      if(.not.checklhs(fname,minlhs,maxlhs)) return 

      if(.not.getrhsvar(1,'d', MA, NA, lA)) return
      if(MA.ne.NA) then
         err=1
         call error(20)
         return
      endif
      if(.not.getrhsvar(2,'d', MB, NB, lB)) return
      if(MB.ne.NB) then
         err=2
         call error(20)
         return
      endif
      if(MA.ne.MB) then
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


c     SUBROUTINE DGGBAL( JOB, N, A, LDA, B, LDB, ILO, IHI, LSCALE,
c     $                   RSCALE, WORK, INFO )

      JOB='B'
      call DGGBAL(JOB, N, stk(lA), N, stk(lB), N, ILO, IHI,stk(lLSCALE),
     $     stk(lRSCALE), stk(lWORK), INFO )

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
      end



      subroutine intzggbal(fname)
c     [Ab,Bb,X,Y]=balanc(A,B)

      include '../stack.h'
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
