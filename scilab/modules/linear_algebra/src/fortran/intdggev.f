      subroutine intdggev(fname)

c     [al,be,V]=gspec(A,B)
c     [al,be] = gspec(A,B)

      include 'stack.h'
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


