c     =======================================
c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c     =======================================
      subroutine scirpem(fname)
c      
      include 'stack.h'
      character*(*) fname
      double precision adelp,adels,fact,zzi(16),zzr(16),zpi(16),zpr(16)
      double precision alpha,beta,u,y,eps,eps1,v
      double precision kappa,lambda,mu,nu
      integer ordre,lw,lt,li,lr,lo,lf,lg
      integer i,top2
      logical vect,arma
      integer iadr,sadr
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1      

      arma=.false.
      top2=top-rhs+1
      il=iadr(lstk(top2))
      if(istk(il).ne.15) then
         err=1
         call error(56)
         return
      endif
      if(istk(il+1).ne.5) then
         err=1
         call error(89)
         return
      endif
      il7=il+7
      ilt=il7+istk(il+2)
      lt=sadr(ilt+4)
      lp=lt+istk(il+3)-istk(il+2)
      ilp=sadr(lp)-4
      ll=lp+istk(il+4)-istk(il+3)
      ill=sadr(ll)-4
      lphi=ll+istk(il+5)-istk(il+4)
      ilphi=sadr(lphi)-4
      lpsi=lphi+istk(il+6)-istk(il+5)
      ilpsi=sadr(ilphi)-4
      top2=top2+1
      ilu=iadr(lstk(top2))
      mnu=istk(ilu+1)*istk(ilu+2)
      if(mnu.le.1)  arma=.true.
      lu=sadr(ilu+4)
      top2=top2+1
      ily=iadr(lstk(top2))
      ly=sadr(ily+4)
      mny=istk(ily+1)*istk(ily+2)
      if(mnu.ne.mny) then
         call error(60)
         return
      endif
      if(top2.eq.top) then
         llam=lstk(top+1)
         stk(llam)=0.950d+0
         stk(llam+1)=0.990d+0
         stk(llam+2)=0.010d+0
         lk=llam+3
         stk(lk)=0.00d+0
         stk(lk+1)=0. 980d+0
         stk(lk+2)=0.020d+0
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      illam=iadr(lstk(top2))
      llam=sadr(illam+4)
      if(top2.eq.top) then
         lk=lstk(top+1)
         stk(lk)=0.00d+0
         stk(lk+1)=0.980d+0
         stk(lk+2)=0.020d+0
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      ilk=iadr(lstk(top2))
      lk=sadr(ilk+4)
      if(top2.eq.top) then
         lc=lk+3
         stk(lc)=1000.0d+0
         goto 55
      endif
      top2=top2+1
      ilc=iadr(lstk(top2))
      lc=sadr(ilc+4)
      ilk=iadr(lstk(top2-1))
      lk=sadr(ilk+4)
      illam=iadr(lstk(top2-2))
      llam=sadr(illam+4)
 55   continue
      mnt=istk(ilt+1)*istk(ilt+2)
      ordre=mnt/3
      lf=lc+1+2*ordre
      lg=lf+mnt
      lsta=lg+mnt
      lwork=lsta+1+ordre
      lmax=lwork+1
      err=lmax-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      lambda=stk(llam)
      alpha=stk(llam+1)
      beta=stk(llam+2)
      kappa=stk(lk)
      mu=stk(lk+1)
      nu=stk(lk+2)
      do 52 k=1,mny-1
         y=stk(ly+k)
         u=stk(lu+k-1)
         call rpem(stk(lt),stk(lp),ordre,u,y,lambda,
     1        kappa,stk(lc),ista,v,eps,eps1,mnt,
     2        stk(lphi),stk(lpsi),stk(lsta),stk(lwork),
     3        stk(lf),stk(lg),stk(ll))
         lambda=alpha*lambda+beta
         kappa=mu*kappa+nu
 52   continue
      if (lhs.eq.1) then
         top=top-rhs+1
         return
      endif
      if(lhs.eq.2) then
         top=top-rhs+2
         istk(ilu)=1
         istk(ilu+1)=1
         istk(ilu+2)=1
         istk(ilu+3)=0
         stk(lu)=v
         lstk(top+1)=lu+1
         return
      endif
      return
      end
c     =======================================
      