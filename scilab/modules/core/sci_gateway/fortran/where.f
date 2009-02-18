c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c Revised and corrected by Francois VOGEL, July/August 2004
c     (bugs 908, 922 and 911 are fixed by this version)
c
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
      subroutine where
c     extrait l'arbre d'appel de l'instruction courante
c     cette routine est issue de la fin du sous programme error
      include 'stack.h'
      integer iadr,sadr

      integer p,lpts(6),lcts,r,vol,rios
      logical first

      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

c     preserve datas
      call icopy(6,lpt,1,lpts,1)
      lcts=lct(8)
      p=pt
      rios=rio

c     initialize
      
      top=top+1
      il=iadr(lstk(top))
      
      first=.true.
      il0=il
      nn=0
      ll=0

c     depilement de l'environnement
      p=p+1
 1001 p=p-1
      if(p.eq.0) goto 1010
      r=rstk(p)
      goto(1002,1002,1004) r-500
      goto 1001
c
c     on depile une macro
 1002 k=lpt(1)-(13+nsiz)
      lpt(1)=lin(k+1)
      lpt(2)=lin(k+2)
      lpt(6)=k
c
c     recherche du nom de la macro correspondant a ce niveau
      lk=sadr(lin(k+6))
      if(lk.le.lstk(top+1)) then
         km=0
      else
         km=lin(k+5)-1
      endif
 1003 km=km+1
      if(km.gt.isiz)goto 1013
      if(lstk(km).ne.lk) goto 1003
c
 1013 continue
      ilk=lin(k+6)
      if(istk(ilk).ne.10) then
         if(first) then
            first=.false.
            if (istk(ilk).eq.13) then 
c     .        compiled macro
               nlc=0
            else
c     .        uncompiled macro
               nlc=1
            endif
         else
            call whatln(lpt(1),lpt(2),lpt(6),nlc,l1,ifin)
         endif
         err=sadr(il+2+nlgh)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(il)=lct(8)-nlc
         il=il+1
         if (km.le.isiz) then
            call namstr(idstk(1,km),istk(il+1),istk(il),1)
            ll=ll+istk(il)
            il=il+1+istk(il)
         else
            istk(il)=0
            il=il+1
         endif
         nn=nn+1
      else
         istk(il)=0
         istk(il+1)=7
         call cvstr(7,istk(il+2),'execstr',0)
         ll=ll+7
         il=il+9
         nn=nn+1
      endif

      lct(8)=lin(k+12+nsiz)
c
      goto 1001
c
c     on depile un exec ou une pause
 1004 if(rio.ne.rte) then
c     exec
         k=lpt(1)-(13+nsiz)
         lpt(1)=lin(k+1)
         lpt(2)=lin(k+4)
         lpt(6)=k
c
         if(first) then
            first=.false.
            nlc=0
         endif
         err=sadr(il+2+nlgh)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(il)=lct(8)-nlc
         istk(il+1)=4
         call cvstr(4,istk(il+2),'exec',0)
         ll=ll+4
         il=il+6
         nn=nn+1
         lct(8)=lin(k+12+nsiz)
c
 1005    p=p-1
         if(rstk(p).ne.902) goto 1005
         rio=pstk(p)
         goto 1001
      else
c     pause
         k = lpt(1) - (13+nsiz)
         lpt(1) = lin(k+1)
         lpt(2) = lin(k+2)
         lpt(3) = lin(k+3)
         lpt(4) = lin(k+4)
         lpt(6) = k
         if(first) first=.false.
         err=sadr(il+2+nlgh)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(il)=0
         istk(il+1)=5
         call cvstr(5,istk(il+2),'pause',0)
         ll=ll+5
         il=il+7
         nn=nn+1
         lct(8)=lin(k+12+nsiz)
         goto 1001
      endif
c     restaure datas
 1010 call icopy(6,lpts,1,lpt,1)
      lct(8)=lcts
      rio=rios
c     create return variables
      ill=il0
      ll=sadr(ill+4)
      ilm=iadr(ll+nn)
      if(nn.eq.0) then
         err=sadr(ill+8)-lstk(bot)
         if(err.gt.0) then
            call error(17)
            return
         endif
         istk(ill)=1
         istk(ill+1)=0
         istk(ill+2)=0
         istk(ill+3)=0
         lstk(top+1)=ll
         top=top+1
         istk(ilm)=1
         istk(ilm+1)=0
         istk(ilm+2)=0
         istk(ilm+3)=0
         lstk(top+1)=sadr(ilm+4)
         return
      endif
c     compute total size of names strings
      vol=0
      il1=il0
      do 05 i=1,nn
         n1=istk(il1+1)
         vol=vol+n1
         il1=il1+2+n1
 05   continue  
c     check memory
      ilw=ilm+4+nn+1+vol
      err=sadr(ilw+il-il0)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call icopy(il-il0,istk(il0),-1,istk(ilw),-1)
      
      istk(ill)=1
      istk(ill+1)=nn
      istk(ill+2)=1
      istk(ill+3)=0
c
      istk(ilm)=10
      istk(ilm+1)=nn
      istk(ilm+2)=1
      istk(ilm+3)=0
      istk(ilm+4)=1
c
      il=ilw
      ln=1
      do 10 i=1,nn
         stk(ll-1+i)=istk(il)
         n=istk(il+1)
         istk(ilm+4+i)=ln+n
         il=il+2
         if(n.gt.0) then
            call icopy(n,istk(il),1,istk(ilm+4+nn+ln),1)
            il=il+n
         endif
         ln=ln+n
 10   continue
      lstk(top+1)=ll+nn
      top=top+1
      lstk(top+1)=sadr(ilm+4+nn+ln)
      return
      end
      
