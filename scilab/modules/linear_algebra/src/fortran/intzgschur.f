c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c$

      subroutine intzgschur(fname)


c     [As,Bs,VSR,dim]=gschur(A,B,'function')
c     [VSR,dim]=gschur(A,B,'function')

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      

      character fname*(*)
      character  JOBVSL, JOBVSR, SORT
      integer gzhsel
      external gzhsel

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
      if(MA.ne.NB) then
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
         else
            if(.not.createvar(3,'d', N, N, lVSR)) return
            if(.not.createvar(4,'d', 1, 1, lSDIM)) return
            stk(lSDIM)=0.0d0
            lhsvar(2)=2
            lhsvar(3)=3
            lhsvar(4)=4
         endif
         return
      elseif(N.eq.-1) then
         err=1
         call error(271)
         return
      endif
      if(.not.getrhsvar(3,'c', mr, mc, lc)) return
      call setgzhsel(mr*mc, cstk(lc:lc+mr*mc),irep)
      if ( irep.eq.1) then 
         buf = cstk(lc:lc+mr*mc)
         call error(50)
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
      LWORK=maxvol(11,'z')
      if(LWORK.le.LWORKMIN) then
         err=2*(LWORK-LWORKMIN)
         call error(17)
         return
      endif
      if(.not.createvar(11,'z',1,LWORK,lDWORK)) return

      JOBVSL = 'V'
      JOBVSR = 'V' 
      SORT = 'S'

      call ZGGES( JOBVSL, JOBVSR, SORT, gzhsel, N, zstk(lA), N,
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

      end



      
      logical function scigshur(alphar,alphai,beta)
      INCLUDE 'stack.h'
      logical scifunction, createvar, createcvar
      common /scigsch/ lf, nx, nf
      integer iadr
      double precision alphar, alphai, beta
c     
      iadr(l) = l+l-1
c     
      scigshur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=alphar
      stk(lx+1)=alphai
      if(.not.createvar(nx+1,'d',1,1,lb)) return
      stk(lb)=beta
      if(.not.scifunction(nx,lf,1,2)) return
c     stk(lx)=fct([alphar,alphai,beta])  evaluated by scilab fct pointed
c     to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scigshur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scigshur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scigchk()
c     checks fct passed to gschur
      INCLUDE 'stack.h'
      logical scifunction, createvar, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scigsch/ lf, nx, nf
      iadr(l) = l+l-1

      scigchk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.createvar(nx+1,'d',1,1,lb)) return
      stk(lb)=1.0d0
      if(.not.scifunction(nx,lf,1,2)) then
c     error into fct passed to gschur (gschur(A,B,tst))
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
      scigchk=.true.
      return
      end

