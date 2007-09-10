      subroutine intlsqrsolve(fname)
c     ----------------------------
c     Scilab lsqrsolve function 
c
c     Copyright INRIA. 
c     Author : Serge Steer INRIA
c     ---------------------------
      character*(*) fname
c      implicit undefined (a-z)
      include '../stack.h'
      integer topk,kres,kjac,kx,m1,n1,lr1,lr,lw,gettype
      logical checklhs,checkrhs,getrmat,getexternal,cremat,jac
      logical type,getscalar
      double precision ftol,xtol,gtol,epsfcn,factor
      integer maxfev,nprint
      external blsqrsolv,bjlsqrsolv,setlsqrsolvf,setlsqrsolvj
      character*(nlgh+1) namef,namej
      common/clsqrsolve/namef,namej
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      topk = top
      if (.not.checkrhs(fname,3,6)) return
      if (.not.checklhs(fname,1,3)) return

c     checking variable x (number 1)
      kx=top-rhs+1
      if(.not.getrmat(fname,topk,kx,m1,n1,lr1))return
      n=m1*n1

c     checking variable fcn (number 2)
      kres=top-rhs+2
      if (.not.getexternal(fname,topk,kres,namef,type,
     $     setlsqrsolvf)) return
c
c     checking variable m (number 3)
      km=top-rhs+3
      if(.not.getscalar(fname,topk,km,lr3))return
      m=stk(lr3)


c     checking variable jac (number 4)
      jac=.false.
      kjac=0
      iskip=0
      if (rhs.ge.4) then 
         itype=gettype(top-rhs+4) 
         if (itype.eq.13.or.itype.eq.10.or.itype.eq.11.or.
     $        itype.eq.15) then 
            jac=.true.
            kjac=top-rhs+4
            if (.not.getexternal(fname,topk,kjac,namej,
     $           type,setlsqrsolvj)) return
         else
            iskip=1
         endif
      endif

c     checking variable tol (number 5)
      if(rhs.ge.5-iskip) then
         if(.not.getrmat(fname,topk,top-rhs+5-iskip,m5,n5,lr5))return
         if(m5*n5.ne.6) then 
            call error(60)
            return
         endif
         ftol=stk(lr5)
         xtol=stk(lr5+1)
         gtol=stk(lr5+2)
         maxfev=stk(lr5+3)
         epsfcn=stk(lr5+4)
         factor=stk(lr5+5)
         nprint=0
      else
         ftol=1.d-8
         xtol=1.d-8
         gtol=1.d-5
         maxfev=1000
         epsfcn=0
         factor=100.0d0
         nprint=0
      endif

c     checking variable diag (number 6)
      if(rhs.ge.6-iskip) then
         if(.not.getrmat(fname,topk,top-rhs+5-iskip,m6,n6,lr6))return
         ldiag=lr6
c     test m6*n6=n
         mode=2
      else
         mode=1
      endif
      top=top+1
      if (.not.cremat(fname,top,0,m,n,lfjac,lc)) return

c     
c     to keep track of externals 
C     --------------------------
      top=top+1
      lw = lstk(top)
      ilext=iadr(lw)
      istk(ilext)=1
      istk(ilext+1)=ilext+3
      istk(ilext+2)=ilext+6
      istk(ilext+3)=kres
      istk(ilext+4)=kx
      istk(ilext+5)=km
      istk(ilext+6)=kjac
      istk(ilext+7)=kx
      istk(ilext+8)=km
      ilw=ilext+9
      
c     Working areas 
C     -------------------------------------------
      ilpvt=ilw
      lw=sadr(ilpvt+n)
      lqtf=lw
      lw=lqtf+n
      lwa1=lw
      lw=lwa1+n
      lwa2=lw
      lw=lwa2+n
      lwa3=lw
      lw=lwa3+n
      lwa4=lw
      lw=lwa4+m
      lfvec=lw
      lw=lfvec+m
      if(mode.eq.1) then
         ldiag=lw
         lw=lw+n
      endif

      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      lstk(top+1)=lw
      if(jac) then
         call lmder(bjlsqrsolv,m,n,stk(lr1),stk(lfvec),stk(lfjac),m,ftol
     $        ,xtol,gtol,maxfev,stk(ldiag),mode,factor,nprint,info,nfev
     $        ,njev,istk(ilpvt),stk(lqtf),stk(lwa1),stk(lwa2),stk(lwa3)
     $        ,stk(lwa4))

      else
         call lmdif(blsqrsolv,m,n,stk(lr1),stk(lfvec),ftol,xtol,gtol
     $        ,maxfev,epsfcn,stk(ldiag),mode,factor,nprint,info,nfev
     $        ,stk(lfjac),m,istk(ilpvt),stk(lqtf),stk(lwa1),stk(lwa2)
     $        ,stk(lwa3),stk(lwa4))

      endif 
      if(err.gt.0.or.err1.gt.0) return
c     
      top=top-2
      if(lhs.eq.1) then
         top=top-rhs+1
         goto 999
      endif
      top=top-rhs+2
      if (.not.cremat(fname,top,0,m,1,lr,lc)) return
      call unsfdcopy(m,stk(lfvec),1,stk(lr),1)
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


