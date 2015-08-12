c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

      subroutine intspget(id)
      include 'stack.h'
      double precision tv
      integer id(nsiz),top0
      integer iadr, sadr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      rhs = max(0,rhs)
      top0=top+1-rhs

      lw = lstk(top+1)
      if (rhs .ne.1) then
         call error(39)
         return
      endif
      if (lhs .gt. 3) then
         call error(41)
         return
      endif 
      il=iadr(lstk(top))
      ityp=istk(il)
      if(ityp.ne.5.and.ityp.ne.6) then
         call funnam(ids(1,pt+1),'spget',iadr(lstk(top)))
         fun=-1
         return
      endif
      nel=istk(il+4)
      m=istk(il+1)
      n=istk(il+2)
      it=istk(il+3)
      ilr=il+5
      ilc=ilr+m
      l=sadr(ilc+nel)

      if(nel.eq.0) then
         istk(il)=1
         istk(il+1)=0
         istk(il+2)=0
         istk(il+3)=0
         lstk(top+1)=sadr(il+4)
         if(lhs.ge.2) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=0
            istk(il+2)=0
            istk(il+3)=0
            lstk(top+1)=sadr(il+4)
         endif
         if(lhs.eq.3) then
            top=top+1
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=1
            istk(il+2)=2
            istk(il+3)=0
            l=sadr(il+4)
            stk(l)=m
            stk(l+1)=n
            lstk(top+1)=l+2
         endif
         return
      endif
      lij=sadr(il+4)
      ilv=iadr(lij+2*nel)
      if(ityp.eq.5) then
         lv=sadr(ilv+4)
         ilrs=iadr(max(lw,lv+nel*(it+1)))
      else
         ilrs=iadr(max(lw,lij+2*nel))
      endif
      lw=sadr(ilrs+m+nel)
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     sauvegarde
      call icopy(m+nel,istk(ilr),1,istk(ilrs),1)
c     constitution des variables de sortie
      if(ityp.eq.5) then
         if(l.ge.lv) then
            call unsfdcopy(nel*(it+1),stk(l),1,stk(lv),1)
         else
            call unsfdcopy(nel*(it+1),stk(l),-1,stk(lv),-1)
         endif
      endif
      call int2db(nel,istk(ilrs+m),1,stk(lij+nel),1)
      i1=0
      do 30 i=1,m
         if(istk(ilrs-1+i).ne.0) then
            tv=i
            call dset(istk(ilrs-1+i),tv,stk(lij+i1),1)
            i1=i1+istk(ilrs-1+i)
         endif
 30   continue
      istk(il)=1
      istk(il+1)=nel
      istk(il+2)=2
      istk(il+3)=0
      lstk(top+1)=lij+2*nel
      if(lhs.ge.2) then
         top=top+1
         if(ityp.eq.5) then
            il=iadr(lstk(top))
            istk(il)=1
            istk(il+1)=nel
            istk(il+2)=1
            istk(il+3)=it
            lstk(top+1)=lv+nel*(it+1)
         else
            il=iadr(lstk(top))
            istk(il)=4
            istk(il+1)=nel
            istk(il+2)=1
            call iset(nel,1,istk(il+3),1)
            lstk(top+1)=sadr(il+3+nel)
         endif
      endif
      if(lhs.eq.3) then
         top=top+1
         il=iadr(lstk(top))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=2
         istk(il+3)=0
         l=sadr(il+4)
         stk(l)=m
         stk(l+1)=n
         lstk(top+1)=l+2
      endif
      return
      end
c			======================================
