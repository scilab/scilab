      subroutine intzggev(fname)

c     [al,be,V]=gspec(A,B)
c     [al,be]=gspec(A,B)

      include 'stack.h'
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


