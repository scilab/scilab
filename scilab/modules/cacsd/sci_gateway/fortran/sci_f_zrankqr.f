c     =======================================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     =======================================================
      subroutine intzb03od(fname)
      INCLUDE 'stack.h'
      character fname*(*)
      logical createvar, getrhsvar, checklhs, checkrhs
c     
      integer A,RCOND,RANK,SVAL,JPVT,TAU,R,Q,RWORK,DWORK
      integer ptrA,ptrRCOND,ptrJPVT,ptrSVAL,ptrTAU,ptrRANK
      integer ptrDWORK,ptrR,ptrQ,ptrRWORK
      double precision RCONDdef,SLVMAX
      character*(1) JOBQR
      complex*16 ZERO
      parameter( ZERO=(0.0D0,0.0D0) )

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      minrhs=1
      maxrhs=3
      minlhs=1
      maxlhs=5
      if(.not.checklhs(fname,minlhs,maxlhs)) return
      if(.not.checkrhs(fname,minrhs,maxrhs)) return

      JOBQR='Q'
      RCONDdef=0.0d0
      SLVMAX=0.0d0

      A=1
      RCOND=2
      JPVT=3
      SVAL=4
      TAU=5
      RANK=6
      R=7
      Q=8
      RWORK=9
      DWORK=10
 
      if(.not.getrhsvar(A,'z',M,N,ptrA)) return

      if(rhs.eq.1) then
c                 rankqr(A)
c     create RCOND=0.0,JPVT=[0,..,0]
         if(.not.createvar(RCOND,'d',1,1,ptrRCOND)) return
         stk(ptrRCOND)=RCONDdef
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.2) then
c                 rankqr(A,RCOND)
c     get RCOND, create JPVT=[0,..,0]
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.createvar(JPVT,'i',1,N,ptrJPVT)) return 
         call icopy(N,0,0,istk(ptrJPVT),1)
      elseif(rhs.eq.3) then
c                 rankqr(A,RCOND,JPVT)
c     get  RCOND, JPVT
         if(.not.getrhsvar(RCOND,'d',mR,nR,ptrRCOND)) return
         if(mR*nR.ne.1) then 
            buf=fname//': 2nd parameter (RCOND) has wrong dimension'
            call error(998)
            return
         endif
         if(.not.getrhsvar(JPVT,'i',mJ,nJ,ptrJPVT)) return
         if(mJ*nJ.ne.N) then
            buf=fname//': JPVT must have same column dim. as A'
            call error(998)
            return
         endif
      endif



c    Creating  SVAL,RANK,R,Q,RWORK,DWORK
      
      if(.not.createvar(SVAL,'d',1,3,ptrSVAL)) return
      if(.not.createvar(TAU,'z',1,min(M,N),ptrTAU)) return
      if(.not.createvar(RANK,'i',1,1,ptrRANK)) return
      if(.not.createvar(R,'z',M,N,ptrR)) return
      if(.not.createvar(Q,'z',M,M,ptrQ)) return
      if(.not.createvar(RWORK,'d',1,2*N,ptrRWORK)) return

      LDWORKMIN=max(2*min(M,N),N+1)
      LDWORK=maxvol(DWORK,'z')
      if(LDWORK.le.LDWORKMIN) then
         buf=fname//': not enough memory (use stacksize)'
         call error(998)
         return
      endif
      if(.not.createvar(DWORK,'z',1,LDWORK,ptrDWORK)) return

      LDA=max(1,M)
      call ZB03OD(JOBQR, M, N, zstk(ptrA), LDA, istk(ptrJPVT),
     &  stk(ptrRCOND), SLVMAX, zstk(ptrTAU), istk(ptrRANK),
     &  stk(ptrSVAL), zstk(ptrDWORK), LDWORK, stk(ptrRWORK), INFO)
      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     Save R (=current A)
      call zcopy(M*N,zstk(ptrA),1,zstk(ptrR),1)
c     Set to zero lower trapezoidal part of R:
      IRANK=istk(ptrRANK)
      IR=IRANK-M
      do 10 K=1,N
         II=II+1
         NB=M-IRANK
         IR=IR+M
         NB1=NB
         IR1=IR
         if(K.lt.IRANK) then
           IR1=IR-(IRANK-K)
           NB1=NB+(IRANK-K)
         endif
         call zcopy(NB1,ZERO,0,zstk(ptrR+IR1),1)
 10   continue

c     Make Q
      if(M.le.N) then
c     A is fat
      CALL ZUNGQR( M, M, M, zstk(ptrA), LDA, zstk(ptrTAU), 
     &   zstk(ptrDWORK), LDWORK, INFO )
      call zcopy(M*M,zstk(ptrA),1,zstk(ptrQ),1)
      else
c     A is tall => Q=[A,0]
      call zcopy(M*N,zstk(ptrA),1,zstk(ptrQ),1)
      call zcopy(M*(M-N),ZERO,0,zstk(ptrQ+M*N),1)
      CALL ZUNGQR( M, M, min(M,N), zstk(ptrQ), LDA, zstk(ptrTAU), 
     &   zstk(ptrDWORK), LDWORK, INFO )
      endif

      if(INFO.ne.0) then
         call errorinfo(fname,INFO)
         return
      endif

c     [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])

      lhsvar(1)=Q
      lhsvar(2)=R
      lhsvar(3)=JPVT
      lhsvar(4)=RANK
      lhsvar(5)=SVAL

      end
c     =======================================================

