c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c$
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
c$
      subroutine intzschur(fname)

c     [VS,dim]=zschur(A,function)
c     [VS,dim,T]=zschur(A,function)

      include 'stack.h'
      logical getrhsvar,createvar
      logical checklhs,checkrhs
      
      character fname*(*)
      character*4  JOBVS, SORT
      logical SCIZSCHUR,scizchk
      external SCIZSCHUR,scizchk
      common /scisch/ lf, nx, nf

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
            if(.not.createvar(2,'d', 0, 0, lSDIM)) return 
            stk(lSDIM)=0.0d0
            if(.not.createvar(3,'z', N, N, lVS)) return
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

      if(.not.getrhsvar(2,'f', mlhs, mrhs, lf)) return
      if(mlhs.ne.1 .or. mrhs.ne.1) then
         err=2
         call error(80)
         return
      endif
      if(.not.createvar(3,'z', N, 1, lW)) return
      k = 4
      if(lhs.gt.1) then
         if(.not.createvar(4,'z', N, N, lVS)) return
         k = 5
      endif
      if(.not.createvar(k,'i', 1, 1, lSDIM)) return 
      if(.not.createvar(k+1,'i',N, 1, lBWORK)) return
      if(.not.createvar(k+2,'d', N, 1, lRWORK)) return
      LWORKMIN = 3*N
      LWORK = LWORKMIN
      if(.not.createvar(k+3,'z',1,LWORKMIN,lDWORK)) return
      nx=k+4

      if(lhs.eq.1) then
         JOBVS = 'N'
         lVS = lDWORK
      else
         JOBVS = 'V'
      endif 
      SORT = 'S'

      if(.not.scizchk()) return
      call ZGEES( JOBVS, SORT, SCIZSCHUR, N, zstk(lA), N, istk(lSDIM),
     $     zstk(lW), zstk(lVS), N, zstk(lDWORK), LWORK, stk(lRWORK),
     $     istk(lBWORK), INFO )
      if(info.gt.0) then
         if(info.le.N) then
            call msgs(2,info)
         elseif(info.eq.N+1) then
            buf='eigenvalues could not be reordered (the problem '//
     $              'is very ill-conditioned'
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
      
      logical function scizschur(w)
      INCLUDE 'stack.h'
      logical scifunction, createcvar
      common /scisch/ lf, nx, nf
      integer iadr
      complex*16 w
      intrinsic dreal, dimag
c     
      iadr(l) = l+l-1
c     
      scizschur=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=dreal(w)
      stk(lx+1)=dimag(w)
      if(.not.scifunction(nx,lf,1,1)) return
c     stk(lx)=fct([re,im])  evaluated by scilab fct pointed to by lf
      ilx=iadr(lx-2)
      if(istk(ilx).eq.1) then
         scizschur=(stk(lx).ne.0.0d0)
      elseif (istk(ilx).eq.4) then
         scizschur=(istk(ilx+3).ne.0)
      endif
      return
      end

      logical function scizchk()
c     checks fct passed to schur
      INCLUDE 'stack.h'
      logical scifunction, createcvar
c     
      integer iadr
      common/ierinv/iero
      common /scisch/ lf, nx, nf
      iadr(l) = l+l-1
      scizchk=.false.
      if(.not.createcvar(nx,'d',1,1,1,lx,lc)) return
      stk(lx)=1.0d0
      stk(lx+1)=1.0d0
      if(.not.scifunction(nx,lf,1,1)) then
c     error into fct passed to schur (schur(A,tst))
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
      scizchk=.true.
      return
      end

      logical function voiddummy(ar,ai)

      voiddummy = .false.
      return
      end

