      subroutine matnew
c ====================================================================
c
c             
c    Frequency response, time response
c
c ====================================================================
c
c     Copyright INRIA
      include '../stack.h'
c
      double precision dnrm2,tol,atol,eps,gi,rcond,ur,ui,vr,vi,xr,xi
      integer p,ind,q,job,ido,n,top1
      integer iadr,sadr
c
c         fin    1     2      3      4       5     6
c              xxxx   ppol  tzer    freq  ltitr  rtitr
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matnew '//buf(1:4))
      endif
c
      if(top+1-rhs+lhs.ge.bot) then
         call error(18)
         return
      endif
      eps=0.1*sqrt(stk(leps))
c
      err=0
      goto (10,20,30,40,50,60) fin
 10   continue
      goto 99
c
c  ppol
c
   20 continue
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
c
   30 continue
c     calcule de zeros multivariables
      if(rhs.ne.4) then
         call error(42)
         return
      endif
      if(lhs.ne.2.and.lhs.ne.3) then
         call error(41)
         return
      endif
c   matrice d
      id=iadr(lstk(top))
      if(istk(id).ne.1) then
         err=4
         call error(53)
         return
      endif
      if(istk(id+3).ne.0) then
         err=4
         call error(52)
         return
      endif
      md=istk(id+1)
      nd=istk(id+2)
      ld=sadr(id+4)
c    matrice c
      top=top-1
      ic=iadr(lstk(top))
      if(istk(ic).ne.1) then
         err=3
         call error(53)
         return
      endif
      if(istk(ic+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      mc=istk(ic+1)
      nc=istk(ic+2)
      lc=sadr(ic+4)
c      matrice b
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
      mb=istk(ib+1)
      nb=istk(ib+2)
      lb=sadr(ib+4)
c      matrice a
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
      ma=istk(ia+1)
      na=istk(ia+2)
      if(na.ne.ma) then
         err=1
         call error(20)
         return
      endif
      la=sadr(ia+4)
c
      if(mb.ne.na.or.nc.ne.na.or.nb.ne.nd.or.mc.ne.md) then
         call error(60)
         return
      endif
      if(na.eq.0) then
         return
      endif
      lr=ld+md*nd
      li=lr+na
      laf=li+na
      lbf=laf+(na+mc)*(nb+na)
      lka=lbf+(na+mc)*(nb+na)
      nwrk1=max(nb,mc)
      nwrk2=max(nwrk1,na)+1
      lk1=lka+na*na
      lk2=lk1+nwrk1
      err=lk2+nwrk2-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call sszer(na,nb,mc,stk(la),na,stk(lb),stk(lc),mc,stk(ld),eps
     x     ,stk(lr),stk(li),nu,irank,stk(laf)
     x     ,na+mc,stk(lbf),na+nb,stk(lka),stk(lk1),nwrk1,
     x           stk(lk2),nwrk2,err)
      if(err.gt.0) then
         call error(24)
         return
      endif
c     sortie des resultats
      if(irank.eq.0.and.nu.gt.0) then
         call dset(nu,0.0d0,stk(lr),1)
         call dset(nu,0.0d0,stk(li),1)
         call dset(nu,0.0d0,stk(lk2),1)
      endif
      istk(ia+1)=nu
      istk(ia+2)=min(nu,1)
      istk(ia+3)=1
      call unsfdcopy(nu,stk(lr),1,stk(la),1)
      call unsfdcopy(nu,stk(li),1,stk(la+nu),1)
      lstk(top+1)=la+nu+nu
      top=top+1
      ib=iadr(lstk(top))
      lb=sadr(ib+4)
      istk(ib)=1
      istk(ib+1)=nu
      istk(ib+2)=min(nu,1)
      istk(ib+3)=0
      call unsfdcopy(nu,stk(lk2),1,stk(lb),1)
      lstk(top+1)=lb+nu
      if(lhs.eq.3) then
         top=top+1
         ib=iadr(lstk(top))
         lb=sadr(ib+4)
         istk(ib)=1
         istk(ib+1)=1
         istk(ib+2)=1
         istk(ib+3)=0
         stk(lb)=irank
         lstk(top+1)=lb+1
      endif
      goto 99
c
c reponse frequentielle freq
c ==========================
 40   continue
c
c verification des quantites de parametres d'entree et de sortie
c
      if(rhs.le.0) then
         call error(39)
         return
      endif
      i1=iadr(lstk(top+1-rhs))
      if(rhs.eq.3) goto 45
c
c systeme defini par sa representation d'etat
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
   41 lw1 = lw + 2*n*n + 2*n
      lwgr = lw1 + n
      if (itfr.eq.1) then
        lwgi = lwgr + p*ido*q
      else
        lwgi=lwgr
      endif
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
      do 42 ind = 1,ido
      ig = (ind-1)*p*q
      lf = lfr+ind-1
      if (itfr.eq.1) then
         call dfrmg(job,n,n,q,q,p,n,stk(la),stk(lb),stk(lc),stk(lf),
     &        stk(lf+ido),stk(lwgr+ig),stk(lwgi+ig),rcond,stk(lw),
     $        stk(lw1))
      else
         call dfrmg(job,n,n,q,q,p,n,stk(la),stk(lb),stk(lc),stk(lf),
     &        0.0d0,stk(lwgr+ig),gi,rcond,stk(lw),stk(lw1))
      endif
      if (1.0d+0+rcond.eq.1.0d+0) then
         call error(19)
         return
      endif
      if(rhs.eq.5) call dadd(p*q,stk(ld),1,stk(lwgr+ig),1)
   42 continue
      igran = (itfr+1)*p*q*ido
      call unsfdcopy(igran,stk(lwgr),1,stk(la),1)
      istk(ia) = 1
      istk(ia+1) = q
      istk(ia+2) = p*ido
      istk(ia+3) = itfr
      lstk(top+1)=la+igran
      goto 99
c
c systeme defini par sa representation rationnelle n./d
c
   45 continue
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
   47 continue
   48 continue
      il=iadr(lstk(top))
      istk(il)=1
      istk(il+1)=md
      istk(il+2)=nd*nf
      istk(il+3)=itf
      l=sadr(il+4)
      call unsfdcopy(md*nd*nf*(itf+1),stk(lw),1,stk(l),1)
      lstk(top+1)=l+md*nd*nf*(itf+1)
      goto 99
c
c ltitr
c =====
c
   50 continue
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
c
c rtitr  simulation temporelle des systemes rationnels discrets
c =============================================================
c
 60   continue
c
c
c      systeme defini par sa representatio rationnelle n./d
c
c     y=rtitr(n,d,u [,u0,y0])
c
c     n  matrice polynomes  nd*nu
c     d  matrice polynomes  nd*nd
c     u  matrice scalaire   nu*ido
c     u0 matrice scalaire   nu*maxigd
c     y0 matrice scalaire   nd*maxigd
c     y  matrice scalaire   nd*ido
c
c     maxign= le plus grand degre de polynomes de matrice n
c     maxigd= le plus grand degre de polynomes de matrice d
c
c     verification des quantites de parametres d entree et de sortie
c
      if (lhs .ne. 1) then
         call error(39)
         return
      endif
      if (rhs .ne. 3 .and. rhs .ne. 5) then
         call error(39)
         return
      endif
      lw=lstk(top+1)
      job=1
      if (rhs.eq.5) then
        iy0=iadr(lstk(top))
        if(istk(iy0).ne.1) then
           err=5
           call error(53)
           return
        endif
        if(istk(iy0+3).ne.0) then
           err=5
           call error(52)
           return
        endif
c
        iu0=iadr(lstk(top-1))
        if(istk(iu0).ne.1) then
           err=4
           call error(53)
           return
        endif
        if(istk(iu0+3).ne.0) then
           err=4
           call error(52)
           return
        endif
        top=top-2
        job=2
      endif
c
      in=iadr(lstk(top-2))
      if(istk(in).gt.2) then
         err=1
         call error(54)
         return
      endif
      if(istk(in+3).ne.0) then
         err=1
         call error(52)
         return
      endif
      nx=istk(in+1)
      n1=istk(in+2)
      if(istk(in).eq.1) then
        maxign=0
        ln=sadr(in+4)
                        else
        ilpn=in+8
        call mpdegr(istk(ilpn),nx,maxign,nx,n1)
        ln=sadr(in+9+nx*n1)
      endif
c
      id=iadr(lstk(top-1))
      if(istk(id).gt.2) then
         err=2
         call error(54)
         return
      endif
      if(istk(id+3).ne.0) then
         err=2
         call error(52)
         return
      endif
      nd=istk(id+1)
      if(nd.ne.istk(id+2)) then
         err=2
         call error(20)
         return
      endif
      if(istk(id).eq.1) then
        maxigd=0
        ld=sadr(id+4)
                        else
        ilpd=id+8
        call mpdegr(istk(ilpd),nd,maxigd,nd,nd)
        ld=sadr(id+9+nd*nd)
      endif
c
      iu=iadr(lstk(top))
      if(istk(iu).ne.1) then
         err=3
         call error(54)
         return
      endif
      if(istk(iu+3).ne.0) then
         err=3
         call error(52)
         return
      endif
      top=top-2
      nu=istk(iu+1)
      mu=istk(iu+2)
c
      if(nd.ne.nx.or.n1.ne.nu) then
         call error(60)
         return
      endif
c
      ido=mu+maxigd-maxign
c
      lu=sadr (iu+4)
      lr=lw
c
      lw=lr+nd*ido
      err=lw-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      ly0=lw
      lu0=lw
      if (rhs .eq. 5) then
         if (istk(iy0+1).ne.istk(id+2).and.istk(iy0+1).ne.0) then
            call error(60)
            return
         endif
         if (istk(iy0+2) .ne. maxigd) then
            call error(60)
            return
         endif
         ly0=sadr(iy0+4)
c
         if (istk(iu0+1).ne.istk(in+2).and.istk(iu0+1).ne.0) then
            call error(60)
            return
         endif
         if (istk(iu0+2) .ne. maxigd) then
            call error(60)
            return
         endif
         lu0=sadr(iu0+4)
      endif
c
      if(nd.eq.1.and.nu.eq.1) then
c     cas siso
      call rtitr(1,1,mu,stk(ln),1,maxign,stk(ld),1,maxigd,stk(lu0),
     &           stk(lu),1,stk(ly0),stk(lr),1,job,iw,stk(lw),ierr)
c
      else
c     cas general
      ld1=lw
      ln1=ld1+nd*nd*(maxigd+1)*(istk(id)-1)
      lw=ln1+nd*nu*(maxign+1)*(istk(in)-1)
      ilw=iadr(lw+nd)
      err=sadr(ilw+nd)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c     conversion matrice de polynome --> polynome matriciel
      if(istk(id).eq.2) then
      call dmp2pm(stk(ld),istk(ilpd),nd,stk(ld1),maxigd,nd,nd)
                        else
      ld1=ld
      endif
      if(istk(in).eq.2) then
      call dmp2pm(stk(ln),istk(ilpn),nd,stk(ln1),maxign,nd,nu)
                        else
      ln1=ln
      endif
c
      call rtitr(nu,nd,mu,stk(ln1),nd,maxign,stk(ld1),nd,maxigd,
     &           stk(lu0),stk(lu),nu,stk(ly0),stk(lr),nd,job,
     &           istk(ilw),stk(lw),ierr)
      endif
c
      if (ierr.eq.1)then
c     matrice est quasi singuliere ou mal normalisee
         write(buf(1:13),'(1pd13.4)') stk(lw)
         call msgs(5,0)
      endif
      if (ierr .eq. 2) then
         call error(19)
         return
      endif
c
c     rangement du resultat
c
      iy=iadr(lstk(top))
      istk(iy)=1
      istk(iy+1)=nd
      istk(iy+2)=ido
      istk(iy+3)=0
      ly=sadr(iy+4)
      call unsfdcopy(ido*nd,stk(lr),1,stk(ly),1)
      lstk(top+1)=ly+nd*ido
99    return
      end
