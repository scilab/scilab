c     --------------------------


c     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c     Copyright (C) INRIA
c     
c     This file must be used under the terms of the CeCILL.
c     This source file is licensed as described in the file COPYING,
c     which
c     you should have received as part of this distribution.  The terms
c     are also available at    
c     http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

c     --------------------------
c     
      subroutine scifreq(fname)
c     ----------------------------
      character*(*) fname
      include 'stack.h'
      double precision eps,rcond,ur,ui,vr,vi,xr,xi
      integer p,ind,q,job,ido,n,top1
      integer iadr,sadr
      logical first
      character bufstr*(4096)

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
c     verification des quantites de parametres d'entree et de sortie
c     
      if(rhs.le.0) then
         call error(39)
         return
      endif
      i1=iadr(lstk(top+1-rhs))
      if(rhs.eq.3) goto 45
c     
c     systeme defini par sa representation d'etat
c     
      if(rhs.gt.5.or.rhs.lt.3) then
         call error(39)
         return
      endif
c     addresses des variables
      lw=lstk(top+1)
      top1=top
      ifr = iadr(lstk(top))
      if(istk(ifr).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      itfr=istk(ifr+3)
      top=top-1
      if(rhs.eq.5) then
         id=iadr(lstk(top))
         if(istk(id).ne.1) then
            err=rhs-1
            call error(53)
            return
         endif
         if(istk(id+3).ne.0) then
            err=top1+1-top
            call error(52)
            return
         endif
         top=top-1
      endif
c     
      ic = iadr(lstk(top))
      if(istk(ic).ne.1) then
         err=top1+1-top
         call error(53)
         return
      endif
      if(istk(ic+3).ne.0) then
         err=top1+1-top
         call error(52)
         return
      endif
c     
      top=top-1
      ib = iadr(lstk(top))
      if(istk(ib).ne.1) then
         err=top1+1-top
         call error(53)
         return
      endif
      if(istk(ib+3).ne.0) then
         err=top1+1-top
         call error(52)
         return
      endif
c
      top=top-1
      ia = iadr(lstk(top))
      if(istk(ia).ne.1) then
         err=top1+1-top
         call error(53)
         return
      endif
      if(istk(ia+3).ne.0) then
         err=top1+1-top
         call error(52)
         return
      endif
      if(istk(ia+1).ne.istk(ia+2)) then
         err=top1+1-top
         call error(20)
         return
      endif
c     
      n = istk(ia+1)
      p = istk(ib+2)
      q = istk(ic+1)
      ido = istk(ifr+1)*istk(ifr+2)
c     
      if (n.ne.istk(ib+1).or.n.ne.istk(ic+2)) then
         call  error(60)
         return
      endif
      if (rhs.ne.5) goto 41
      if(istk(id+1).ne.istk(ic+1).or.istk(id+2).ne.istk(ib+2)) then
         call error(60)
         return
      endif
 41   lw1 = lw + 2*n*n + 2*n
      lwgr = lw1 + n
      lwgi = lwgr + p*ido*q
      err = lwgi + p*q*ido - lstk(bot)
      if (err.gt. 0) then
         call error(17)
         return
      endif
      la = sadr(ia+4)
      lb = sadr(ib+4)
      lc = sadr(ic+4)
      lfr = sadr(ifr+4)
      if (rhs.eq.5) ld=sadr(id+4)
      job = 0
      first=.true.
      do 42 ind = 1,ido

         ig = (ind-1)*p*q
         lf = lfr+ind-1
         if (itfr.eq.1) then
            call dfrmg(job,n,n,q,q,p,n,stk(la),stk(lb),stk(lc),stk(lf),
     &           stk(lf+ido),stk(lwgr+ig),stk(lwgi+ig),rcond,stk(lw),
     $           stk(lw1))
         else
            call dfrmg(job,n,n,q,q,p,n,stk(la),stk(lb),stk(lc),stk(lf),
     &           0.0d0,stk(lwgr+ig),stk(lwgi+ig),rcond,stk(lw),stk(lw1))
         endif
         if ((1.0d+0+rcond.eq.1.0d+0).and.first) then
            buf=' '
            write(bufstr(1:13),'(1pd13.4)') rcond
            buf = bufstr
            call msgs(5,0)
            first=.false.
         endif
         if(rhs.eq.5) call dadd(p*q,stk(ld),1,stk(lwgr+ig),1)
 42   continue
      igran = (itfr+1)*p*q*ido
      call unsfdcopy(igran,stk(lwgr),1,stk(la),1)
      istk(ia) = 1
      istk(ia+1) = q
      istk(ia+2) = p*ido
      istk(ia+3) = itfr
      lstk(top+1)=la+igran
      goto 99
c     
c     systeme defini par sa representation rationnelle n./d
c     
 45   continue
      lw=lstk(top+1)
      ilf=iadr(lstk(top))
      if(istk(ilf).ne.1) then
         err=rhs
         call error(53)
         return
      endif
      nf=istk(ilf+1)*istk(ilf+2)
      itf=istk(ilf+3)
      lf=sadr(ilf+4)
c     
      top=top-1
      ild=iadr(lstk(top))
      if(istk(ild).gt.2) then
         err=2
         call error(54)
         return
      endif
      md=istk(ild+1)
      nd=istk(ild+2)
      if(istk(ild+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      ld=sadr(ild+4)
      if(istk(ild).eq.2) ld=sadr(ild+8+md*nd+1)
c     
      top=top-1
      iln=iadr(lstk(top))
      if(istk(iln).gt.2) then
         err=1
         call error(54)
         return
      endif
      if(istk(iln+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      ln=sadr(iln+4)
      if(istk(iln).eq.2) ln=sadr(iln+8+md*nd+1)
c     
      if(istk(iln+1).ne.md.or.istk(iln+2).ne.nd) then
         call error(60)
         return
      endif
c     
      lr=lw
      if(itf.eq.1) then
         li=lr+md*nd*nf
      else
         li=lr
      endif
      err=li+md*nd*nf-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      do 48 i=1,nf
         xr=stk(lf-1+i)
         xi=0.0d+0
         if(itf.eq.1) xi=stk(lf+nf-1+i)
         lnj=ln
         ldj=ld
         nnj=0
         ndj=0
         do 47 j=1,md*nd
            if(istk(iln).eq.2) nnj=istk(iln+8+j)-istk(iln+j+7)-1
            if(istk(ild).eq.2) ndj=istk(ild+8+j)-istk(ild+j+7)-1
            call horner(stk(lnj),nnj,xr,xi,vr,vi)
            call horner(stk(ldj),ndj,xr,xi,ur,ui)
            if(ur*ur+ui*ui.eq.0.0d+0) then
               call error(27)
               return
            endif
            if(itf.eq.1) then
               call wdiv(vr,vi,ur,ui,stk(lr),stk(li))
            else
               stk(lr)=vr/ur
            endif
            lr=lr+1
            li=li+1
            lnj=lnj+nnj+1
            ldj=ldj+ndj+1
 47      continue
 48   continue
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=md
      istk(il+2)=nd*nf
      istk(il+3)=itf
      l=sadr(il+4)
      call unsfdcopy(md*nd*nf*(itf+1),stk(lw),1,stk(l),1)
      lstk(top+1)=l+md*nd*nf*(itf+1)
      goto 99
      
 99   return
      end
c     ----------------------------
