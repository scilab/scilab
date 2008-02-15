c     --------------------------
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
c
      subroutine scippol(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      double precision dnrm2,tol,atol,eps,gi,rcond,ur,ui,vr,vi,xr,xi
      integer p,ind,q,job,ido,n,top1
      integer iadr,sadr

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if(top+1-rhs+lhs.ge.bot) then
         call error(18)
         return
      endif
      eps=0.1*sqrt(stk(leps))
c
      err=0

      if(lhs.ne.1.or.rhs.ne.3) then
         call error(39)
         return
      endif
c
      ip=iadr(lstk(top))
      if(istk(ip).ne.1) then
         err=3
         call error(53)
         return
      endif
      n=istk(ip+1)*istk(ip+2)
      itp=istk(ip+3)
      lpr=sadr(ip+4)
      lpi=lpr+n
      lz=lpi+n
c
      top=top-1
      ib=iadr(lstk(top))
      if(istk(ib).ne.1) then
         err=2
         call error(53)
         return
      endif
      if(istk(ib+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      m=istk(ib+2)
      lb=sadr(ib+4)
c
      top=top-1
      ia=iadr(lstk(top))
      if(istk(ia).ne.1) then
         err=1
         call error(53)
         return
      endif
      if(istk(ia+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      if(istk(ia+1).ne.istk(ia+2)) then
         err=1
         call error(20)
         return
      endif
      la=sadr(ia+4)
c
      if(istk(ia+1).ne.n.or.istk(ib+1).ne.n) then
         call error(60)
         return
      endif
      if(n.eq.0) then
         istk(ia+1)=0
         istk(ia+2)=0
         istk(ia+3)=0
         lstk(top+1)=sadr(ia+4)
         return
      endif
c
      lg=lz+n*n
      ilw=iadr(lg+m*n)
      lw=sadr(ilw+n)
      err=lw+max(n*m+3*m,m*m+m*max(2,m)+3*m+2*n)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      if(itp.eq.0) call dset(n,0.0d+0,stk(lpi),1)
      l=lpi-1
   21 l=l+1
      if(l.ge.lz) goto 22
      if(stk(l).eq.0.0d+0) goto 21
      l=l+1
      if(stk(l).eq.-stk(l-1)) goto 21
      err=3
      call error(44)
      return
c
c calcul de la forme canonique orthogonale
   22 call ssxmc(n,m,stk(la),n,stk(lb),idc,inc,istk(ilw),stk(lz),
     1  stk(lw+m),stk(lw+m+n*m),stk(lw+2*m+n*m),stk(lw),
     2    eps,1)
      if(idc.lt.n) goto 91
c placement de poles
      lw1=lw+m+m*m
      lw2=lw1+m*max(2,m)
      call polmc(n,m,n,m,stk(la),stk(lb),stk(lg),stk(lpr),stk(lpi),
     1           stk(lz),inc,istk(ilw),ierr,stk(lw),stk(lw+m),
     2    stk(lw1),stk(lw2),stk(lw2+n),stk(lw2+2*n),stk(lw2+2*n+m))
      if(ierr.ne.0) goto 91
      istk(ia+1)=m
      call unsfdcopy(n*m,stk(lg),1,stk(la),1)
      lstk(top+1)=la+n*m
      goto 99
c
   91 call error(86)
      return
      
   99    return
      end
c     ----------------------------