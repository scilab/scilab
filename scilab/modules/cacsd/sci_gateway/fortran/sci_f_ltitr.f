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
      subroutine sciltitr(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      double precision eps
      integer p,ind,ido,n
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
c
c verification des quantites de parametres d'entree et de sortie
c
      if(rhs.le.0 .or. rhs.gt.4 .or. lhs.gt.2) then
         call error(39)
         return
      endif
      i1=iadr(lstk(top+1-rhs))
c
c systeme defini par sa representation d'etat
c
c     adresses des variables
      lw=lstk(top+1)
      if(rhs.eq.4) then
         ix=iadr(lstk(top))
         if(istk(ix).ne.1) then
            err=4
            call error(53)
            return
         endif
c
         if(istk(ix+3).ne.0) then
            err=4
            call error(52)
            return
         endif
         top=top-1
      endif
c
      ifr = iadr(lstk(top))
      if(istk(ifr).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(istk(ifr+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      top=top-1
c
      ib = iadr(lstk(top))
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
      if(istk(ifr+1).ne.istk(ib+2)) then
         err=3
         call error(89)
         return
      endif
      top=top-1
c
      ia = iadr(lstk(top))
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
      n = istk(ia+1)
      if (n.ne.istk(ia+2)) then
         err=1
         call error(20)
         return
      endif
c
      p = istk(ib+2)
      ido = istk(ifr+2)
      if (n.ne.istk(ib+1)) then
         call error(60)
         return
      endif
      if(rhs.eq.3) goto 51
c
      if(istk(ix+1).ne.istk(ia+1).or.istk(ix+2).ne.1)  then
         call error(60)
         return
      endif
c
   51 lwgr = lw + n
      err = lwgr + n*ido -  lstk(bot)
      if (err.gt. 0) then
         call error(17)
         return
      endif
      la = sadr(ia+4)
      lb = sadr(ib+4)
      lfr = sadr(ifr+4)
      if(rhs.eq.4) then
                   lx=sadr(ix+4)
                   call unsfdcopy(n,stk(lx),1,stk(lwgr),1)
                   else
                   call dset(n,0.0d+0,stk(lwgr),1)
      endif
      do 52 ind = 1,ido
      ig = ind*n
      lf = lfr+(ind-1)*p
      call dmmul(stk(la),n,stk(lwgr+ig-n),n,stk(lw),n,n,n,1)
      call dmmul(stk(lb),n,stk(lf),p,stk(lwgr+ig),n,n,p,1)
      call dadd(n,stk(lw),1,stk(lwgr+ig),1)
   52 continue
      if(lhs.eq.1) goto 53
      call unsfdcopy(n,stk(lwgr+ig),1,stk(la),1)
      lstk(top+1)=la+n
      istk(ia+1)=n
      istk(ia+2)=1
      top=top+1
   53 iy=iadr(lstk(top))
      istk(iy)=1
      istk(iy+1)=n
      istk(iy+2)=ido
      istk(iy+3)=0
      ly=sadr(iy+4)
      call unsfdcopy(ido*n,stk(lwgr),1,stk(ly),1)
      lstk(top+1)=ly+n*ido
      goto 99      
      
   99    return
      end
c     ----------------------------