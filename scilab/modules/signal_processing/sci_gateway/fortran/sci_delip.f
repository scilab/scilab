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
      subroutine scidelip(fname)
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
      
            if(rhs.ne.2.or.lhs.ne.1) then
         call error(39)
         return
      endif
      il1=iadr(lstk(top))
      itv1=istk(il1)
      if(itv1.ne.1) then
         err=2
         call error(53)
         return
      endif
      m1=istk(il1+1)
      n1=istk(il1+2)
      if(m1*n1.ne.1) then
         err=2
         call error(89)
         return
      endif
      it1=istk(il1+3)
      l1=sadr(il1+4)
      il2=iadr(lstk(top-1))
      itv2=istk(il2)
      if(itv2.ne.1) then
         err=1
         call error(53)
         return
      endif
      m2=istk(il2+1)
      n2=istk(il2+2)
      length=m2*n2
      it2=istk(il2+3)
      l2=sadr(il2+4)
      lw=lstk(top+1)
      err=lw+2*length-lstk(bot)
      if(err.gt.0)then
         call error(17)
         return
      endif
      call delip(length,stk(lw),stk(lw+length),stk(l2),stk(l1))
      top=top-1
      istk(il2)=1
      istk(il2+1)=m2
      istk(il2+2)=n2
      istk(il2+3)=1
      call unsfdcopy(length,stk(lw),1,stk(l2),1)
      call unsfdcopy(length,stk(lw+length),1,stk(l2+length),1)
      lstk(top+1)=l2+2*length
      return

      end
c     =======================================
      