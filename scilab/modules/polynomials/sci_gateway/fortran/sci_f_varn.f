c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) ????-2008 - INRIA
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at
c http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
      subroutine intvarn(id)
      INCLUDE 'stack.h'
      integer iadr, sadr
      integer id(nsiz)
      integer vol,blank
      logical ref
      data blank/40/
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if(lhs.ne.1) then
         call error(41)
         return
      endif
c
      il1=iadr(lstk(top+1-rhs))
      ilr=il1
      if(istk(il1).lt.0) il1=iadr(istk(il1+1))
      ref=ilr.ne.il1
      if(istk(il1).ne.2) then
         if(rhs.eq.2.and.istk(il1).eq.1) then
            top=top-1
            if (ref) then
               k=istk(ilr+2)
               vol=lstk(k+1)-lstk(k)
               err=lstk(top)+vol-lstk(bot)
               if(err.gt.0) then
                  call error(17)
                  return
               endif
               call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
               lstk(top+1)=lstk(top)+vol
            endif
            return
         endif
         fun=-1
         call funnam(ids(1,pt+1),'varn',il1)
         return
      endif
      call icopy(4,istk(il1+4),1,id,1)

      if(rhs.eq.1) then
c     get formal variable name
         if(ref) then
            err=sadr(ilr+10)-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
         endif
c     extraction du nom de la variable muette
         istk(ilr)=10
         istk(ilr+1)=1
         istk(ilr+2)=1
         istk(ilr+3)=0
         do 10 ii=4,1,-1
            if(istk(il1+3+ii).ne.blank) then
               nv=ii
               goto 11
            endif
 10      continue
 11      call icopy(nv,istk(il1+4),-1,istk(ilr+6),-1)
         istk(ilr+4)=1
         istk(ilr+5)=nv+1
         lstk(top+1)=sadr(ilr+10)
      elseif(rhs.eq.2) then
c     change formal variable name
c     .  get new variable name
         il=iadr(lstk(top))
         if(istk(il).lt.0) il=iadr(istk(il+1))
         if(istk(il).ne.10) then
            err=2
            call error(55)
            return
         endif
         if(istk(il+1)*istk(il+2).ne.1) then
            err=2
            call error(36)
            return
         endif
         nc=istk(il+5)-1
         if(istk(il1).eq.1) goto 106
         call icopy(max(4,nc),istk(il+6),1,id,1)
         if(nc.lt.4) call iset(4-nc,blank,id(nc+1),1)
 106     top=top-1
         if(ref) then
c     .     variable is passed by reference copy it on the top of the
c           stack
            k=istk(ilr+2)
            vol=lstk(k+1)-lstk(k)
            err=lstk(top)+vol-lstk(bot)
            if(err.gt.0) then
               call error(17)
               return
            endif
            call unsfdcopy(vol,stk(lstk(k)),1,stk(lstk(top)),1)
            lstk(top+1)=lstk(top)+vol
         endif
         ilr=iadr(lstk(top))
         call icopy(4,id,1,istk(ilr+4),1)
      else
         call error(42)
         return
      endif
      return
      end
c			=======================================
