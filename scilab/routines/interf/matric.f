      subroutine matric
c ================================== ( Inria    ) =============
c       equations de lyapounov,sylvester et riccati
c ====================================================================
c     Copyright INRIA
      include '../stack.h'
      integer iadr,sadr
c
      double precision eps,rcond
      integer cont,disc
      data disc/13/,cont/12/
c
c    fin    1       2       3
c         lyap    sylv     ricc
c
c
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matric '//buf(1:4))
      endif
c
      eps=stk(leps)
      lz=lstk(top+1)
c
      if(fin.eq.1) then
         if(rhs.ne.3) then
            call error(39)
            return
         endif
      endif
      if(fin.eq.2.or.fin.eq.3) then
         if(rhs.ne.4) then
            call error(39)
            return
         endif
      endif
      ilcd=iadr(lstk(top))
      if(istk(ilcd).ne.10) then
         err=rhs
         call error(55)
         return
      endif
      icd=abs(istk(ilcd+5+istk(ilcd+1)*istk(ilcd+2)))
      if(icd.ne.cont .and. icd.ne.disc) then
         err=rhs
         call error(36)
         return
      endif
c
c     second membre
      top=top-1
      ic=iadr(lstk(top))
      if(istk(ic).ne.1) then
         err=rhs-1
         call error(53)
         return
      endif
      if(istk(ic+3).ne.0) then
         err=rhs-1
         call error(52)
         return
      endif
      mc=istk(ic+1)
      nc=istk(ic+2)
      nnc=nc*mc
      lc=sadr(ic+4)
      top=top-1
c
      err=0
      goto(10,20,30) fin
c lyapounov
  10  continue
c
      if(mc.ne.nc) then
         err=2
         call error(20)
         return
      endif
c
c    dynamique
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
      nna=na*ma
      la=sadr(ia+4)
      if(ma.ne.na) then
         call error(20)
         return
      endif
c
      if(ma.ne.mc) then
         call error(60)
         return
      endif
c
      err=lz+nna+na - lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      if(icd.eq.cont) call lybsc(na,stk(la),na,stk(lc),stk(lc),
     x                           stk(lz),eps,stk(lz+nna),0,err)
      if(icd.eq.disc) call lybad(na,stk(la),na,stk(lc),stk(lc),
     x                           stk(lz),eps,stk(lz+nna),0,err)
      if(err.gt.0) then
         call error(24)
         return
      endif
      call unsfdcopy(nna,stk(lc),1,stk(la),1)
      return
c
  20  continue
c    sylvester a*x+x*b=c
c
c    matrice b
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
      if(mb.ne.nb) then
         err=2
         call error(20)
         return
      endif
      nnb=nb*mb
      lb=sadr(ib+4)
c
c       matrice a
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
      if(ma.ne.na) then
         err=1
         call error(20)
         return
      endif
      nna=ma*na
      ita=istk(ia+3)
      la=sadr(ia+4)
c
      if(na.ne.mc.or.nc.ne.mb) then
         call error(60)
         return
      endif
c
      if(icd.eq.disc) goto 25
      nw1=2*nna+7*na
      nw2=max(na,nb)
      nw3=4*na
      err=lz+nna+nw1+nw2+nw3-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      na4=na+4
      call syhsc(na,nb,stk(la),na,stk(lb),nb,stk(lc),
     x                 stk(lz),eps,stk(lz+nnb),
     x           nw1,stk(lz+nnb+nw1),nw2,stk(lz+nnb+nw1+nw2),nw3,err)
      if(err.gt.0) then
         call error(19)
         return
      endif
      call unsfdcopy(nnc,stk(lc),1,stk(la),1)
      istk(ia+1)=mc
      istk(ia+2)=nc
      lstk(top+1)=la+mc*nc
      return
   25 continue
      lu=lz
      lv=lu+na*na
      lw=lv+nb*nb
      err=lw+max(na,nb)-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
      call sybad(na,nb,stk(la),na,stk(lb),nb,stk(lc),na,stk(lu),
     1           stk(lv),eps,stk(lw),0,err)
      if(err.gt.0) then
         call error(19)
         return
      endif
      call unsfdcopy(na*nb,stk(lc),1,stk(la),1)
      istk(ia+1)=mc
      istk(ia+2)=nc
      lstk(top+1)=la+mc*nc
      return
c
c    riccati
c
  30  continue
c
c    a'*x+x*a-x*b*x+c=0
c
c         matrice b
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
      if(mb.ne.nb) then
         err=2
         call error(20)
         return
      endif
      nnb=mb*nb
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
      ma=istk(ia+1)
      na=istk(ia+2)
      if(ma.ne.na) then
         err=1
         call error(20)
         return
      endif
      nna=na*ma
      nn=na+na
      la =sadr(ia+4)
c
      if(ma.ne.mb) then
         call error(42)
         return
      endif
c
      lw=lz
      lz=lw+nn*nn
      lw1=lz+nn*nn
      lw2=lw1+nn
      lw3=lw2+nn
      err=lw3+nn-lstk(bot)
      if(err.gt.0) then
         call error(17)
         return
      endif
c
      if(icd.eq.disc) goto 31
      call rilac(na,nn,stk(la),na,stk(lc),stk(lb),rcond,stk(la),
     x           stk(lw),nn,stk(lz),eps,stk(lw3),
     x           stk(lw1),stk(lw2),err)
      if(err.ne.0) then
         call error(24)
         return
      endif
      return
  31  continue
      call ricd(na,nn,stk(la),na,stk(lc),stk(lb),rcond,stk(la),
     x          stk(lw),nn,stk(lz),eps,stk(lw3),
     x          stk(lw1),stk(lw2),err)
      if(err.ne.0) then
         call error(24)
         return
      endif
      return
      end
