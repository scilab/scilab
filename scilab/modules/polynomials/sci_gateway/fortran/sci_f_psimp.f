c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine intpsimp(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vola,volb
      logical chkvar,vcopyobj
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c

c     simp(num,den)
c
      if(lhs.ne.2) then
         call error(41)
         return
      endif
c
      ila=iadr(lstk(top+1-rhs))

      il=ila
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il)
         return
      endif
c
      ilb=iadr(lstk(top))

      il=ilb
      if(istk(il).lt.0) il=iadr(istk(il+1))
      if(istk(il).gt.2) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',il)
         return
      endif
c
      if(istk(ila).lt.0) then
c     move b and copy value of a in place of its reference
         k=istk(ila+2)
         vola=lstk(k+1)-lstk(k)
         volb=lstk(top+1)-lstk(top)
         err=lstk(top)+vola+volb-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(volb,stk(lstk(top)),-1,stk(lstk(top-1)+vola),-1)
         call unsfdcopy(vola,stk(lstk(k)),1,stk(lstk(top-1)),1)
         lstk(top)=lstk(top-1)+vola
         ilb=iadr(lstk(top))
         lstk(top+1)=lstk(top)+volb
      endif

      if(istk(ilb).lt.0) then
c      copy value of b in place of its reference
         k=istk(ilb+2)
         volb=lstk(k+1)-lstk(k)
         err=lstk(top)+volb-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         call unsfdcopy(volb,stk(lstk(k)),1,stk(lstk(top)),1)
         lstk(top+1)=lstk(top)+volb
      endif
      lw=lstk(top+1)

c
      if(istk(ila+3).ne.0.or.istk(ilb+3).ne.0) then
         fun=-1
         call funnam(ids(1,pt+1),'simp',ilb)
         return
      endif
c
      mna=istk(ila+1)*istk(ila+2)
      id(1)=0
      if(istk(ila).eq.2) then
         ida=ila+8
         la=sadr(ida+mna+1)
         call icopy(4,istk(ila+4),1,id,1)
      else
         la=sadr(ila+4)
         ida=iadr(lw)
         lw=sadr(ida+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 74 i=1,mna+1
            istk(ida+i-1)=i
 74      continue
      endif
c
      mnb=istk(ilb+1)*istk(ilb+2)
      if(istk(ilb).eq.2) then
         idb=ilb+8
         lb=sadr(idb+mnb+1)
         if(id(1).eq.0) then
            call icopy(4,istk(ilb+4),1,id,1)
         else
            if(.not.chkvar(id,istk(ilb+4))) then
               call error(43)
               return
            endif
         endif
      else
         lb=sadr(ilb+4)
         idb=iadr(lw)
         lw=sadr(idb+mna+1)
         err=lw-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         do 75 i=1,mna+1
            istk(idb+i-1)=i
 75      continue
      endif

      if(mnb.ne.mna)then
         call error(60)
         return
      endif
c
c     determine max of the degrees
      na=0
      nb=0
      do 76 i=1,mna
         na=max(na,istk(ida+i)-istk(ida-1+i))
         nb=max(nb,istk(idb+i)-istk(idb-1+i))
 76   continue
c     preserve adress of the beginning of a and b coefficients

      lar=la
      lbr=lb
c     allocate memory for intermediate results
      law=lw
      lbw=law+na+1
      lw=lbw+nb+1

c     simplify
      la1=la
      lb1=lb
      do 79 i=1,mna
         na=istk(ida+i)-istk(ida-1+i)-1
         nb=istk(idb+i)-istk(idb-1+i)-1
         ierr=lstk(bot)-lw
         call  dpsimp(stk(la),na,stk(lb),nb,stk(law),nnum,
     $        stk(lbw),nden,stk(lw),ierr)
         if(ierr.eq.1) then
            call error(27)
            return
         elseif(ierr.eq.2) then
            call msgs(43,i)
         endif
c     .  copy overwrite initial polynomials with simplified ones
         call dcopy(nnum,stk(law),1,stk(la1),1)
         call dcopy(nden,stk(lbw),1,stk(lb1),1)

         la=la+na+1
         lb=lb+nb+1
         la1=la1+nnum
         lb1=lb1+nden
         istk(ida-1+i)=nnum
         istk(idb-1+i)=nden
 79   continue
c
c     form vector of pointers from vector of degrees+1
      ma=1
      mb=1
      do 80 i=1,mna+1
         na=istk(ida-1+i)
         nb=istk(idb-1+i)
         istk(ida-1+i)=ma
         istk(idb-1+i)=mb
         ma=ma+na
         mb=mb+nb
 80   continue
c
c     compute position of the a and b simplified in the result
      lstk(top)=lar+istk(ida+mna)-1
      il=iadr(lstk(top))
c
c     put new b variable in place
      if(istk(ilb).eq.2) then
c     b matrice de polynome
         l=sadr(il+9+mna)
c     .  move b data up
         call icopy(9+mna,istk(ilb),1,istk(il),1)
         call unsfdcopy(istk(il+8+mna),stk(lbr),1,stk(l),1)
         l=l+istk(il+8+mna)-1
      else
c     b matrice de scalaires
         call icopy(4,istk(ilb),1,istk(il),1)
         l=sadr(il+4)
         call unsfdcopy(mna,stk(lbr),1,stk(l),1)
         l=l+mna
      endif
      lstk(top+1)=l
      end

c			=======================================
