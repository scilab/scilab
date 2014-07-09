c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intbez(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,blank,ipb(6)
      double precision er

      logical refa,refb
      data blank/40/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

      if(rhs.ne.2) then
         call error(42)
         return
      endif
      if(lhs.ne.2.and.lhs.ne.3) then
         call error(41)
         return
      endif
      lw =lstk(top+1)
c
      ilb=iadr(lstk(top))
      ilbr=ilb
      if(istk(ilb).lt.0) ilb=iadr(istk(ilb+1))
      if(istk(ilb).eq.1.or.istk(ilb).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',ilb)
         return
      endif
      refb=ilbr.ne.ilb
      itb=istk(ilb+3)
      if(itb.ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',ilb)
         return
      endif
      if(istk(ilb+1)*istk(ilb+2).ne.1)then
         err=2
         call error(89)
         return
      endif
      if(istk(ilb).eq.1) then
         nb=0
         lb=sadr(ilb+4)
         id(1)=0
      else
         nb=istk(ilb+9)-2
         lb=sadr(ilb+9)
         call icopy(4,istk(ilb+4),1,id,1)
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      if(istk(il1).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',il1)
         return
      endif
      refa=ilr.ne.il1
      it1=istk(il1+3)
      if(it1.ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'bezout',il1)
         return
      endif
      if(istk(il1+1)*istk(il1+2).ne.1) then
         err=1
         call error(89)
         return
      endif
      mn1=1
      if(istk(il1).eq.2) then
         id1=il1+8
         l1=sadr(id1+mn1+1)
         vol=istk(id1+mn1)-1
         na=vol-1
         if(id(1).eq.0) call icopy(4,istk(il1+4),1,id,1)
      else
         l1=sadr(il1+4)
         na=0
      endif
      if(id(1).eq.0) call iset(4,blank,id,1)
c
      lf=lstk(top+1)+na+9
      if(refa) lf=lw+na+9
c
      lw=lf+2*(na+nb)+min(na,nb)+3
      n0=max(na,nb)+1
      err=lw+10*n0+3*n0*n0-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call recbez(stk(l1),na,stk(lb),nb,stk(lf),ipb,stk(lw),er)
c     pgcd
      istk(ilr)=2
      istk(ilr+1)=1
      istk(ilr+2)=1
      istk(ilr+3)=0
      call icopy(4,id,1,istk(ilr+4),1)
      l1=sadr(ilr+8+mn1+1)
      istk(ilr+8)=1
      np=ipb(2)-ipb(1)-1
      istk(ilr+9)=2+np
      call unsfdcopy(np+1,stk(lf+ipb(1)-1),1,stk(l1),1)
      lstk(top)=l1+np+1

c     matrice q
      il=iadr(lstk(top))
      istk(il)=2
      istk(il+1)=2
      istk(il+2)=2
      istk(il+3)=0
      call icopy(4,id,1,istk(il+4),1)
      il=il+8
      istk(il)=1
      l=sadr(il+5)
      do 71 i=1,4
         ii=i+1
         istk(il+1)=istk(il)+ipb(ii+1)-ipb(ii)
         call unsfdcopy(istk(il+1)-istk(il),stk(lf+ipb(ii)-1),1,
     $        stk(l),1)
         l=l+istk(il+1)-istk(il)
         il=il+1
 71   continue
      lstk(top+1)=l
      if(lhs.eq.3) then
c     retour de l'erreur
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=1
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=er
         lstk(top+1)=l+1
      endif
      goto 999
 999  return
      end

c			=======================================
