      subroutine intozgschur(fname)

c     [As,Bs,VSR,dim]=gshur(A,B,function)
c     [VS,dim]=gshur(A,B,function)

      include 'stack.h'
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
      INCLUDE 'stack.h'
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
      INCLUDE 'stack.h'
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

