c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c
      subroutine scisolv(fname)
c     ----------------------------
c     Scilab fsolve function 
c     ---------------------------
      character*(*) fname
c      implicit undefined (a-z)
      include 'stack.h'
      integer topk,kres,kjac,kx,m1,n1,lr1,lr,lw,gettype
      logical checklhs,checkrhs,getrmat,getexternal,cremat,jac
      logical type,getscalar
      double precision tol
      external bsolv,bjsolv,setfsolvf,setfsolvj
      character*(nlgh+1) namef,namej
      common/csolve/namef,namej
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      topk = top
      if (.not.checkrhs(fname,2,4)) return
      if (.not.checklhs(fname,1,3)) return
c     checking variable x (number 1)
c     
      kx=top-rhs+1
      if(.not.getrmat(fname,topk,kx,m1,n1,lr1))return
      mn1=m1*n1
c     checking variable fcn (number 2)
c     
      kres=top-rhs+2
      if (.not.getexternal(fname,topk,kres,namef,type,
     $     setfsolvf)) return
c     
c     checking variable jac (number 3)
c     
      jac=.false.
      kjac=0
      iskip=0
      if (rhs.ge.3) then 
         itype=gettype(top-rhs+3) 
         if (itype.eq.13.or.itype.eq.10.or.itype.eq.11.or.
     $        itype.eq.15) then 
            jac=.true.
            kjac=top-rhs+3
            if (.not.getexternal(fname,topk,kjac,namej,
     $           type,setfsolvj)) return
         else
            iskip=1
         endif
      endif
c     checking variable tol (number 4)
c     
      if(rhs.eq.4-iskip) then
         if(.not.getscalar(fname,topk,top,lr4))return
         tol=stk(lr4)
      else
         tol=1.d-10
      endif
c     
c     An Ugly feature to keep track of externals 
C     -------------------------------------------
      top=top+1
      lw = lstk(top)
      ilext=iadr(lw)
      istk(ilext)=2
      istk(ilext+1)=ilext+4
      istk(ilext+2)=ilext+7
      istk(ilext+3)=ilext+10
      istk(ilext+4)=kres
      istk(ilext+5)=mn1
      istk(ilext+6)=kx
      istk(ilext+7)=kjac
      istk(ilext+8)=mn1
      istk(ilext+9)=kx
c     Working areas 
C     -------------------------------------------
      lw=sadr(ilext+10)
      lw4=lw
      lw=lw4+mn1
      lw6=lw
      lw=lw+1
      if(jac) then
         nn7=(mn1*(mn1+13))/2+mn1*mn1
      else
         nn7=(mn1*(3*mn1+13))/2
      endif
      lw7=lw
      lw=lw+nn7
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      lstk(top+1)=lw
      if(jac) then
         call hybrj1(bjsolv,mn1,stk(lr1),stk(lw4),stk(lw7),
     $        mn1,tol,info,stk(lw7+mn1*mn1),nn7)
      else
         call hybrd1(bsolv,mn1,stk(lr1),stk(lw4),tol,info,
     $        stk(lw7),nn7)
      endif 
      if(err.gt.0.or.err1.gt.0) return
c     
      top=top-1
      if(lhs.eq.1) then
         top=top-rhs+1
         goto 999
      endif
      top=top-rhs+2
      if (.not.cremat(fname,top,0,m1,n1,lr,lc)) return
      call unsfdcopy(mn1,stk(lw4),1,stk(lr),1)
      if(lhs.eq.3) then
c     info = 0   improper input parameters.
c     info = 1   algorithm estimates that the relative error
c     between x and the solution is at most tol.
c     info = 2   number of calls to fcn with iflag = 1 has
c     reached 100*(n+1).
c     info = 3   tol is too small. no further improvement in
c     the approximate solution x is possible.
c     info = 4   iteration is not making good progress.
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)=info
      endif
      goto 999
      
c     
 999  continue
      return
      end
      


