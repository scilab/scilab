      subroutine matopt
c     Copyright INRIA
      INCLUDE '../stack.h'
c     
      double precision zero,df0,zng,dxmin
      double precision epsg,epsg1,epsf,dzs
      integer top2,topin,topind,topx,top3
      character*80   nomsub
      common /optim/ nomsub
      integer       nizs,nrzs,ndzs
      common /nird/ nizs,nrzs,ndzs
      external foptim,boptim,fuclid,ctonb,ctcab
      integer coin,coar,coti,cotd,cosi,cosd,nfac
c     
      character*(nlgh+1) namef,namej
      common/csolve/namef,namej
c     for semidef
      double precision abstol,reltol,nu,tv
      integer sz,upsz
c
      integer impn(nsiz)
      logical eqid, getscalar
      integer iadr, sadr
c     
      parameter (nsiz1=nsiz-1)
      data impn/672732690,nsiz1*673720360/
      data coin,coar,coti,cotd,cosi,cosd,nfac
     &     /   5906,6922,4637,3357,4636,3356, 0/
c     
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
c     
      if (ddt .eq. 4) then
         write(buf(1:4),'(i4)') fin
         call basout(io,wte,' matopt '//buf(1:4))
      endif
c     
c     optim   quapro   semidef fsolve lsqrsolve
c     1        2        3        4       5
c     
      goto(01,400,500,600,700 ) fin
c     
      
c     optim
 01   continue
      napm=100
      itmax=100
      epsf=0.0d+0
      iepsx=1
      indtes=0
      imp=0
      io=wte
      if(wio.gt.0) io=wio
      zero=stk(leps)
      df0=1.0d+0
      immx=0
      epsg=zero
      indtv=0
      icontr=1
      ialg=1
      irecd=0
      ireci=0

c
      if(infstk(top).eq.1) then
         infstk(top)=0
         if(eqid(idstk(1,top),impn)) then
            if (.not.getscalar('optim',top,top,lr)) return
            imp=stk(lr)
            top=top-1
            rhs=rhs-1
         endif
      endif

      lf=lstk(top+1)
      ldisp=lf+1
      lizs=iadr(ldisp)
      lrzs=ldisp
      ldzs=ldisp
      ldisp=ldzs+1
c     
      top2=top+1-rhs
      top3=top2
      topin=top2-1
      il=iadr(lstk(top2))
      if (rhs.lt.2) then
         call error(39)
         return
      endif
c     
c     
c     traitement de simul
      l1=istk(il)
      if((l1-10)*(l1-11)*(l1-13)*(l1-15).ne.0) then
         err=top2-topin
         call error(80)
         return
      endif
c     cas simul=liste
      if(istk(il).eq.10) then
         if (istk(il+1)*istk(il+2).ne.1) then
            err=top2-topin
            call error(89)
            return
         endif
         nc=min(istk(il+5)-1,80)
         nomsub(1:80)= ' '
         call cvstr(nc,istk(il+6),nomsub,1)
         if(err.gt.0) return
         nomsub(nc+1:nc+1)=char(0)
         call setfoptim(nomsub,irep)
         if ( irep.eq.1) then 
            buf = nomsub
            call error(50)
            return
         endif
         isim=1
      endif
c     cas simul=macro ou liste
      if(istk(il).eq.11.or.istk(il).eq.13.or.istk(il).eq.15) then
         kopt=top2
         isim=2
      endif
      top2=top2+1
      il=iadr(lstk(top2))
c     
c     
c     contraintes de borne (chaine "b" , xinf , xsup )
      if(istk(il).eq.10.and.istk(il+5).eq.2.and.
     +     abs(istk(il+6)).eq.11) then
         if (rhs.lt.5) then
            call error(39)
            return
         endif
         top2=top2+1
         il=iadr(lstk(top2))
         if(istk(il).gt.2)  then
            err=top2-topin
            call error(54)
            return
         endif
         nbi=istk(il+1)*istk(il+2)
         if(istk(il).eq.1) then
            nbi=nbi*(istk(il+3)+1)
            lbi=sadr(il+4)
         else
            lbi=sadr(il+9+nbi)
            nbi=(istk(il+8+nbi)-1)*(istk(il+3)+1)
         endif
         top2=top2+1
         il=iadr(lstk(top2))
         if(istk(il).gt.2)  then
            err=top2-topin
            call error(54)
            return
         endif
         nbs=istk(il+1)*istk(il+2)
         if(istk(il).eq.1) then
            nbs=nbs*(istk(il+3)+1)
            lbs=sadr(il+4)
         else
            lbs=sadr(il+9+nbs)
            nbs=(istk(il+8+nbs)-1)*(istk(il+3)+1)
         endif
         if((nbs.ne.nbi)) then
            call error(139)
            return
         endif
         icontr=2
         top2=top2+1
         il=iadr(lstk(top2))
      end if
c     
c     point initial
      if(istk(il).gt.2)  then
         err=top2-topin
         call error(54)
         return
      endif
      topx=top2
      nx1=istk(il+1)
      nx2=istk(il+2)
      itvx=istk(il)
      ilx=il
      if(istk(il).eq.1) then
         nx=nx1*nx2*(istk(il+3)+1)
         lx=sadr(il+4)
      else
         nx=(istk(il+8+nx1*nx2)-1)*(istk(il+3)+1)
         lx=sadr(il+9+nx1*nx2)
      endif
      if (icontr.ne.1.and.nx.ne.nbi) then
         call error(135)
         return
      endif
c     a quoi servent les 2 lignes suivantes. elle pose pb pour le nom de la macro
c     simulateur dans les messages d'erreur
c     idstk(1,top-1)=nx1
c     idstk(2,top-1)=nx2
c     
c     stockage de g
      lg=ldisp
      ldisp=lg + nx
      err=ldisp - lstk(bot)
      if (err.gt.0) then
         call error(17)
         return
      endif
      if (top2.eq.top) go to 200
      top2=top2+1
      il=iadr(lstk(top2))
c     
c     choix d algorithme
      if(istk(il).eq.10) then
         if (istk(il+5)-1.ne.2) then
            err=top2-topin
            call error(36)
            return
         endif
         ic1=abs(istk(il+6))
         ic2=abs(istk(il+7))
         ialg1=0
         if (ic1.eq.26.and.ic2.eq.23) ialg1=1
         if (ic1.eq.16.and.ic2.eq.12) ialg1=2
         if (ic1.eq.23.and.ic2.eq.13) ialg1=10
         if (ialg1.ne.0) then
            ialg=ialg1
            if (top2.eq.top) go to 200
            top2=top2+1
            il=iadr(lstk(top2))
         end if
      endif
c     
c     df0
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         df0=stk(sadr(il+4))
         if (df0.le.0) then
            call error(143)
            return
         endif
         if (top.eq.top2) go to 200
         top2=top2 + 1
         il=iadr(lstk(top2))
      endif
c     
c     mmx
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         l=sadr(il+4)
         mmx=int(stk(l))
         immx=1
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      end if
c     
c     hot start (optimiseurs n1qn1 et qnbd)
      if(istk(il).eq.1.and.istk(il+3).eq.0) then
         if (ialg.ne.1) then
            call error(137)
            return
         endif
         ntv=istk(il+1)*istk(il+2)
         if(icontr.eq.1.and.ntv.ne.nx*(nx+13)/2) then
            err=top2-topin
            call error(142)
            return
         endif
         ltv=sadr(il+4)
         indtv=1
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      end if
c     
c     chaine 'ar'
      if(istk(il).ne.10)  then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if(istk(il+5)-1.ne.2 ) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).ne.coar) goto 150
      if (top2.eq.top) go to 200
      top2=top2+1
      il=iadr(lstk(top2))
c     
c     napm et itmax
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         l=sadr(il+4)
         napm=int(stk(l))
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      end if
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         l=sadr(il+4)
         itmax=int(stk(l))
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      end if
c     
c     epsg,epsf,epsx (note : epsx est un vecteur)
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         epsg=stk(sadr(il+4))
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      endif
      if(istk(il).eq.1.and.istk(il+1)*istk(il+2).eq.1) then
         epsf=stk(sadr(il+4))
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      endif
      if(istk(il).eq.1.and.istk(il+3).eq.0) then
         if(istk(il+1)*istk(il+2).ne.nx) then
            call error(138)
            return
         endif
         iepsx=0
         lepsx=sadr(il+4)
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      endif
c     
c     chaine 'in'
 150  if(istk(il).ne.10) then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if (istk(il+5)-1.ne.2) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).eq.coin) then
         if(isim.ne.1) then
            buf='''in'' not allowed with simulator defined '//
     $           'by a function'
            call error(9999)
            return
         endif

c     on initialise nizs,nrzs,ndzs
         indsim=10
         if(isim.eq.1) then
            call foptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call boptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           izs,rzs,dzs)
         endif
         if(err.gt.0) return
c     
         if(indsim.le.0) then
            indopt=-7
            if(indsim.eq.0) indopt=0
            go to 350
         endif
c     stockage de izs,rzs,dzs dans la pile
         l1=ldisp
         lizs=iadr(l1)
         lrzs=lizs+nizs
         ldzs=sadr(lrzs+nrzs)
         ldisp=ldzs + ndzs
         err=ldisp - lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
         indsim=11
         lstk(top+1)=ldisp
         if(isim.eq.1) then
            call  foptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(indsim.le.0) then
            if(indsim.eq.0) indopt=0
            go to 350
         endif
         if (top2.eq.top) go to 200
         top2=top2 + 1
         il=iadr(lstk(top2))
      endif
c     
c     izs et dzs en entree (chaine 'ti' et/ou 'td' suivie du tableau)
      if(istk(il).ne.10) then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if(istk(il+5)-1.ne.2) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).eq.coti) then
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
         if(istk(il).eq.1.and.istk(il+3).eq.0) then
            nizs=istk(il+1)*istk(il+2)
            lizs1=sadr(il+4)
            lizs=iadr(lizs1)
            do 185 i=0,nizs-1
               istk(lizs+i)=int(stk(lizs1+i))
 185        continue
            if (top2.eq.top) go to 200
            top2=top2+1
            il=iadr(lstk(top2))
         endif
      endif
      if(istk(il).ne.10) then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if(istk(il+5)-1.ne.2) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).eq.cotd)  then
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
         if(istk(il).eq.1.and.istk(il+3).eq.0) then
            ndzs=istk(il+1)*istk(il+2)
            ldzs=sadr(il+4)
            if (top2.eq.top) go to 200
            top2=top2+1
            il=iadr(lstk(top2))
         endif
      endif
c     
c     mettre izs et/ou dzs en sortie (chaine 'si' ou 'sd')
      if(istk(il).ne.10) then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if(istk(il+5)-1.ne.2) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).eq.cosi) then
         ireci=1
         if (top2.eq.top) go to 200
         top2=top2+1
         il=iadr(lstk(top2))
      endif
c     
      if(istk(il).ne.10) then
         err=top2-topin
         call error(55)
         return
      endif
      if (istk(il+1)*istk(il+2).ne.1) then
         err=top2-topin
         call error(89)
         return
      endif
      if(istk(il+5)-1.ne.2) then
         err=top2-topin
         call error(36)
         return
      endif
      if(abs(istk(il+6))+256*abs(istk(il+7)).ne.cosd) then
         err=top2-topin
         call error(36)
         return
      endif
      irecd=1
c     
c     fin epluchage liste appel
 200  if (top.ne.top2) then
         call error(39)
         return
      endif
c     
c     creation des variables contenant le simulateur et ind
      top=top+1
      topind=top
      lstk(top)=ldisp
      il=iadr(ldisp)
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      ldisp=sadr(il+4)
      ldisp=ldisp+1
c     
      top=top+1
      lstk(top)=ldisp
      il=iadr(ldisp)
      istk(il)=1
      istk(il+1)=il+2
      istk(il+2)=kopt
      istk(il+3)=topx
      istk(il+4)=topind
      ldisp=sadr(il+5)
c     
c     initialisation eventuelle de f et g
      iarret=0
      if (napm.lt.2.or.itmax.lt.1) iarret=1
c     
      if((icontr.eq.1.and.(ialg.eq.2.or.ialg.eq.10)).or.
     &     (icontr.eq.2.and.ialg.eq.1.and.indtv.eq.1) .or.
     &     (iarret.eq.1)    )   then
         indsim=4
         lstk(top+1)=ldisp
         if(isim.eq.1) then
            call foptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call boptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0)return
         if(indsim.le.0) then
            indopt=-7
            if(indsim.eq.0) indopt=0
            go to 350
         endif
         if (napm.lt.2.or.itmax.lt.1) go to 300
      endif
c     
c     appel de l optimiseur
c     
c     optimiseur n1qn1
      if(icontr.eq.1.and.ialg.eq.1) then
         lvar=ldisp
         mode=3
         ntv=nx*(nx+13)/2
         ldisp=lvar + nx
         if(indtv.eq.0) then
            mode=1
            ltv=lvar  + nx
            ldisp=ltv + ntv
         endif
         err=ldisp - lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
         do 50 i=0,nx-1
            stk(lvar+i)=0.10d+0
 50      continue
         nitv=0
         lstk(top + 1)=ldisp
c     
c     mise en memoire de parametres d entree pour l affectation de indop
         itmax1=itmax
         napm1=napm
         epsg1=epsg
c     
         if(isim.eq.1) then
            call n1qn1(foptim,nx,stk(lx),stk(lf),stk(lg),
     &           stk(lvar),epsg,mode,itmax,napm,imp,io,stk(ltv),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call n1qn1(boptim,nx,stk(lx),stk(lf),stk(lg),
     &           stk(lvar),epsg,mode,itmax,napm,imp,io,stk(ltv),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0) return
c     affectation de indopt
         epsg=sqrt(epsg)
         indopt=9
         if(itmax.ge.itmax1) indopt=5
         if(napm.ge.napm1) indopt=4
         if(epsg1.ge.epsg) indopt=1
         go to 300
      endif

c     algorithme n1qn3
      if(icontr.eq.1.and.ialg.eq.2) then
c     calcul de epsrel
         zng=0.0d+0
         do 230 i=0,nx-1
            zng=zng + stk(lg+i)**2
 230     continue
         zng=sqrt(zng)
         if (zng.gt.0.0d+0) epsg=epsg/zng
c     calcul du scalaire dxmin
         dxmin=stk(leps)
         if (iepsx.eq.0) then
            dxmin=stk(lepsx)
            if (nx.gt.1) then
               do 235 i=1,nx-1
                  dxmin=min(dxmin,stk(lepsx+i))
 235           continue
            endif
         endif
c     tableaux de travail (mmx=nombre de mises a jour)
         if (immx.eq.0) mmx=10
         ntv=4*nx + mmx*(2*nx + 1)
         ltv=ldisp
         ldisp=ltv + ntv
         err=ldisp - lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
         lstk(top+1)=ldisp
c     
         if(isim.eq.1) then
            indsim=4
            call foptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
            call n1qn3(foptim,fuclid,ctonb,ctcab,nx,stk(lx),stk(lf),
     $           stk(lg),dxmin,df0,epsg,imp,io,mode,itmax,napm,
     &           stk(Ltv),Ntv,istk(lizs),sstk(lrzs),stk(ldzs) )
         else
            indsim=4
            call boptim(indsim,nx,stk(lx),stk(lf),stk(lg),
     &           istk(lizs),sstk(lrzs),stk(ldzs))
            call n1qn3(boptim,fuclid,ctonb,ctcab,nx,stk(lx),stk(lf),
     &           stk(lg),dxmin,df0,epsg,imp,io,mode,itmax,napm,
     &           stk(ltv),ntv,istk(lizs),sstk(lrzs),stk(ldzs) )
         endif
         if (err.gt.0) return
         indopt=9
         if (mode.eq.0) indopt=0
         if (mode.eq.1) indopt=1
         if (mode.eq.2) indopt=-10
         if (mode.eq.3) indopt=7

         if (mode.eq.4) indopt=5
         if (mode.eq.5) indopt=4
         if (mode.eq.6) indopt=3
         if (mode.eq.7) indopt=7
         go to 300
      endif
c     
c     optimiseur n1fc1 (non smooth)
      if(icontr.eq.1.and.ialg.eq.10) then
         if (immx.eq.0) mmx=10
         nitv=2*mmx + 2
         itv1=5*nx + (nx+4)*mmx
         itv2=(mmx+9)*mmx + 8
         err=ldisp + iepsx*nx + nitv/2 +1 +itv1 +itv2 -lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
         if (iepsx.eq.1) then
            lepsx=ldisp
            do 115 i=1,nx
               stk(lepsx+i - 1)=zero
 115        continue
            ldisp=lepsx+nx
         endif
         litv=iadr(ldisp)
         ltv1=sadr(litv+nitv)
         ltv2=ltv1  + itv1
         ldisp=ltv2 + itv2
         lstk(top+1)=ldisp
         if (isim.eq.1) then
            call n1fc1(foptim,fuclid,nx,stk(lx),stk(lf),stk(lg),
     &           stk(lepsx),df0,epsf,zero,imp,io,mode,itmax,napm,mmx,
     &           istk(litv),stk(ltv1),stk(ltv2),istk(lizs),sstk(lrzs),
     $           stk(ldzs))
         else
            call n1fc1(boptim,fuclid,nx,stk(lx),stk(lf),stk(lg),
     &           stk(lepsx),df0,epsf,zero,imp,io,mode,itmax,napm,mmx,
     &           istk(litv),stk(ltv1),stk(ltv2),istk(lizs),sstk(lrzs),
     $           stk(ldzs))
         endif
         if (err.gt.0) return
c     interpretation de la cause de retour
         indopt=9
         if (mode.eq.0)indopt=0
         if (mode.eq.1)indopt=2
         if (mode.eq.2)indopt=-10
         if (mode.eq.4)indopt=5
         if (mode.eq.5)indopt=4
         if (mode.eq.6)indopt=3
         go to 300
      endif
c     
c     optimiseur qnbd
      if(icontr.eq.2.and.ialg.eq.1) then
         if (iepsx.eq.1) then
            err=ldisp +nx - lstk(bot)
            if (err.gt.0) then
               call error(17)
               return
            endif
            lepsx=ldisp
            ldisp=lepsx + nx
            do 118 i=0,nx-1
               stk(lepsx+i)=zero
 118        continue
         endif
         ntv1=nx*(nx+1)/2 + 4*nx + 1
         nitv= 2*nx
         if (indtv.eq.0) then
            ntv=ntv1
            err= ldisp + ntv + nitv/2 +1 - lstk(bot)
            if (err.gt.0) then
               call error(17)
               return
            endif
            ltv=ldisp
            litv=iadr(ltv+ntv)
            lstk(top+1)=sadr(litv+nitv)
         else
            if (ntv.ne.ntv1+nitv) then
               err=top2-topin
               call error(142)
               return
            endif
            ntv=ntv1
            litv1=ltv+ntv
            litv=iadr(litv1)
            do 117 i=0,nitv-1
               istk(litv+i)=int(stk(litv1+i))
 117        continue
         endif
         indopt=1 +indtv
         if(isim.eq.1) then
            call qnbd(indopt,foptim,nx,stk(lx),stk(lf),
     &           stk(lg),imp,io,zero,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call qnbd(indopt,boptim,nx,stk(lx),stk(lf),
     &           stk(lg),imp,io,zero,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0) return
         go to 300
      endif
c     
c     optimiseur gcbd
      if(icontr.eq.2.and.ialg.eq.2) then
         nt=2
         if (immx.eq.1) nt= max(1,mmx/3)
         ntv=nx*(5 + 3*nt) + 2*nt +1
         nitv=nx + nt + 1
         err= ldisp + iepsx*nx + ntv + nitv/2 - lstk(bot)
         if (err.gt.0) then
            call error(17)
            return
         endif
         if (iepsx.eq.1) then
            lepsx=ldisp
            ltv=lepsx + nx
            do 119 i=0,nx-1
               stk(lepsx+i)=zero
 119        continue
         else
            ltv=ldisp
         endif
         litv=iadr(ltv+ntv)
         lstk(top+1)=sadr(litv+nitv)
         indopt=1
         if (indtes.ne.0) indopt=indtes
         if(isim.eq.1) then
            call gcbd(indopt,foptim,nomsub,nx,stk(lx),
     &           stk(lf),stk(lg),imp,io,zero,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call gcbd(indopt,boptim,nomsub,nx,stk(lx),stk(lf),
     &           stk(lg),imp,io,zero,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0) return
         go to 300
      endif
c     
c     algorithme non implante
      call error(136)
      return
c     
c     laissons la pile aussi propre qu on aurait aime la trouver
 300  top2=top3
      top =top3 + lhs - 1
      fun=0
c     
      lhs1=lhs - ireci -irecd
      if (lhs1.le.0) then
         call error(41)
         return
      endif
c     
c     sauvegarde de f
      il=iadr(lstk(top2))
      istk(il)=1
      istk(il+1)=1
      istk(il+2)=1
      istk(il+3)=0
      l=sadr(il+4)
      stk(l)=stk(lf)
      lstk(top+1)=l+1
      if(lhs.eq.1) go to 320
c     
c     sauvegarde de x
      l=l+1
      top2=top2 + 1
      lstk(top2)=l
      il=iadr(l)
      if(itvx.eq.1) then
         call icopy(4,istk(ilx),1,istk(il),1)
         lx1=sadr(il+4)
      else
         call icopy(9+nx1*nx2,istk(ilx),1,istk(il),1)
         lx1=sadr(il+9+nx1*nx2)
      endif
      ilx=il
      lstk(top+1)=lx1+nx
      call unsfdcopy(nx,stk(lx) ,1,stk(lx1),1)
      if(lhs1.eq.2) go to 320
c     
c     sauvegarde de g
      top2=top2 + 1
      lstk(top2)=lstk(top+1)
      il=iadr(lstk(top2))
      if(itvx.eq.1) then
         call icopy(4,istk(ilx),1,istk(il),1)
         l=sadr(il+4)
      else
         call icopy(9+nx1*nx2,istk(ilx),1,istk(il),1)
         l=sadr(il+9+nx1*nx2)
      endif
      call unsfdcopy(nx,stk(lg) ,1,stk(l),1)
      lstk(top+1)=l+nx
      if(lhs1.eq.3) goto 320
c     
c     sauvegarde de tv (tableau interne a l' optimiseur - pour hot start
      if (lhs1.eq.4) then
         istv=0
         if(ialg.eq.1) istv=1
         if (istv.eq.0) then
c     pas de hot start pour cet algorithme
            call error(137)
            return
         endif
         top2=top2 + 1
         lstk(top2)=lstk(top+1)
         il=iadr(lstk(top2))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=ntv + nitv
         istk(il+3)=0
         l=sadr(il+4)
c     recopie eventuelle de dzs et izs pour ne pas les ecraser
         if (indtv.eq.0.and.(ireci-1)*(irecd-1).eq.0) then
            err=l+ntv+nitv+ireci*nizs+irecd*ndzs-lstk(bot)
            if (err.gt.0) then
               call error(17)
               return
            endif
c     if (l+ntv+nitv+1.ge.sadr(lizs)) then
            ldzs2=lstk(bot)-ndzs
            lizs2=iadr(ldzs2)-nizs
            if (sadr(lizs2).le.ltv+ntv+nitv+1) then
               call error(17)
               return
            endif
            call unsfdcopy(ndzs,stk(ldzs),-1,stk(ldzs2),-1)
            do 315 i=nizs-1,0,-1
               istk(lizs2+i)=istk(lizs+i)
 315        continue
            ldzs=ldzs2
            lizs=lizs2
         endif
      endif
      call unsfdcopy(ntv,stk(ltv),1,stk(l),1)
      if (nitv.gt.0) then
         do 316 i=nitv-1,0,-1
            istk(litv+2*i)=istk(litv+i)
 316     continue
         litv1=l+ntv
         do 317 i=0,nitv-1
            stk(litv1+i)=real(istk(litv+i))
 317     continue
      endif
      lstk(top+1)=l + ntv + nitv
c     
c     sauvegarde de izs et dzs
 320  if (lhs.eq.lhs1) go to 350
      if (ireci.eq.1) then
         top2=top2 + 1
         lstk(top2)=lstk(top+1)
         il=iadr(lstk(top2))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=nizs
         istk(il+3)=0
         l=sadr(il+4)
         do 325 i=0,nizs-1
            stk(l+i)=real(istk(lizs+i))
 325     continue
         lstk(top+1)=l+nizs
      endif
      if (irecd.eq.1) then
         top2=top2 + 1
         lstk(top2)=lstk(top+1)
         il=iadr(lstk(top2))
         istk(il)=1
         istk(il+1)=1
         istk(il+2)=ndzs
         istk(il+3)=0
         l=sadr(il+4)
         call unsfdcopy(ndzs,stk(ldzs) ,1,stk(l),1)
         lstk(top+1)=l+ndzs
      endif
      go to 350
c     
c     commentaires finaux
 350  continue
      if (iarret.eq.1) return
      if (indopt.gt.0) go to 360
      if(indopt.eq.0) then
         call error(131)
         return
      elseif(indopt.eq.-7) then
         call error(134)
         return
      elseif (indopt.eq.-14) then
         call error(133)
         return
      elseif(indopt.le.-10) then
         call error(132)
         return
      endif

 360  continue
      if(imp.ne.0) then
         if(indopt.eq.1) then
            write(buf(1:15),'(1pd15.7)') epsg
            call msgs(12,0)
         elseif(indopt.eq.2) then
            write(buf(1:15),'(1pd15.7)') epsf
            call msgs(13,0)
         elseif(indopt.eq.3)  then
            call msgs(14,0)
         elseif(indopt.eq.4)  then
            call msgs(15,0)
         elseif(indopt.eq.5)  then
            call msgs(16,0)
         elseif(indopt.eq.6)  then
            call msgs(17,0)
         elseif(indopt.eq.7)  then
            call msgs(18,0)
         elseif(indopt.eq.8)  then
            call msgs(19,0)
         elseif(indopt.eq.9)  then
            call msgs(20,0)
         elseif(indopt.ge.10)  then
            call msgs(21,0)
         endif
      endif
      return
c     
c     quapro
c     
 400  continue
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 10 .and. rhs .ne. 9) then
         call error(39)
         return
      endif
      if (lhs .gt. 3 .or. lhs .lt. 2) then
         call error(41)
         return
      endif
c     checking variable x0 (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 1) then
         err = 1
         call error(53)
         return
      endif
      if (istk(il1+2) .ne. 1) then
         err = 1
         call error(89)
         return
      endif
      n1 = istk(il1+1)
      l1 = sadr(il1+4)
c     checking variable h (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         err = 2
         call error(53)
         return
      endif
      if (istk(il2+1) .ne. istk(il2+2)) then
         err = 2
         call error(20)
         return
      endif
      n2 = istk(il2+1)
      m2 = istk(il2+2)
      l2 = sadr(il2+4)
c     checking variable p (number 3)
c     
      il3 = iadr(lstk(top-rhs+3))
      if (istk(il3) .ne. 1) then
         err = 3
         call error(53)
         return
      endif
      if (istk(il3+2) .ne. 1) then
         err = 3
         call error(89)
         return
      endif
      n3 = istk(il3+1)
      l3 = sadr(il3+4)
c     checking variable c (number 4)
c     
      il4 = iadr(lstk(top-rhs+4))
      if (istk(il4) .ne. 1) then
         err = 4
         call error(53)
         return
      endif
      n4 = istk(il4+1)
      m4 = istk(il4+2)
      l4 = sadr(il4+4)
c     checking variable d (number 5)
c     
      il5 = iadr(lstk(top-rhs+5))
      if (istk(il5) .ne. 1) then
         err = 5
         call error(53)
         return
      endif
      if (istk(il5+2) .ne. 1.and.istk(il5+2) .ne. 0) then
         err = 5
         call error(89)
         return
      endif
      m5 = istk(il5+1)
      l5 = sadr(il5+4)
c     checking variable ci (number 6)
c     
      il6 = iadr(lstk(top-rhs+6))
      if (istk(il6) .ne. 1) then
         err = 6
         call error(53)
         return
      endif
      if (istk(il6+2) .gt. 1) then
         err = 6
         call error(89)
         return
      endif
      n6 = istk(il6+1)
      l6 = sadr(il6+4)
c     checking variable cs (number 7)
c     
      il7 = iadr(lstk(top-rhs+7))
      if (istk(il7) .ne. 1) then
         err = 7
         call error(53)
         return
      endif
      if (istk(il7+2) .gt. 1) then
         err = 7
         call error(89)
         return
      endif
      n7 = istk(il7+1)
      l7 = sadr(il7+4)
c     checking variable mi (number 8)
c     
      il8 = iadr(lstk(top-rhs+8))
      if (istk(il8) .ne. 1) then
         err = 8
         call error(53)
         return
      endif
      if (istk(il8+1)*istk(il8+2) .ne. 1) then
         err = 8
         call error(89)
         return
      endif
      mi = stk(sadr(il8+4))
c     checking variable modo (number 9)
c     
      il9 = iadr(lstk(top-rhs+9))
      if (istk(il9) .ne. 1) then
         err = 9
         call error(53)
         return
      endif
      if (istk(il9+1)*istk(il9+2) .ne. 1) then
         err = 9
         call error(89)
         return
      endif
      modo = stk(sadr(il9+4))
      if(modo.ne.1.and.modo.ne.2.and.modo.ne.3) then
         err=9
         call error(36)
         return
      endif
      if (rhs .eq. 10) then
c     checking variable imp (number 10)
c     
         il10 = iadr(lstk(top-rhs+10))
         if (istk(il10) .ne. 1) then
            err = 10
            call error(53)
            return
         endif
         if (istk(il10+1)*istk(il10+2) .ne. 1) then
            err = 10
            call error(89)
            return
         endif
         imp = stk(sadr(il10+4))
      else
         imp = 0
      endif
c     
c     cross variable size checking
c     
      if (n3 .ne. n2) then
         call error(42)
         return
      endif
      if (n1 .ne. n3) then
         if(modo.eq.3) then
            call error(42)
            return
         else
            n1=n3
         endif
      endif
      if (n4.ne.0.and.n3 .ne. n4) then
         call error(42)
         return
      endif
      if (n6 .eq. 0) then
         ira=0
      elseif (n1 .ne. n6) then
         call error(42)
         return
      else
         ira=1
      endif
      if(n7 .eq. 0) then
      elseif (n1 .ne. n7) then
         call error(42)
         return
      else
         ira=ira+2
      endif
      if (m4 .ne. m5) then
         call error(42)
         return
      endif
      md=m4-mi
c     
c     x
c      lw9=lw
c     lw=lw+n1
      lw9=l1
c     f
      lw10=lw
      lw=lw+1
c     w
      lw11=lw
      lw=lw+4*n1*(n1+1)+md+max(md,2*n1-2+n1*(n1+1)/2)
c     iv
      lw12=lw
      lw=lw+3*n1+2*md+mi+1
c     lagr
      lw13=lw
      if(ira.gt.0) then
         nl=n1+m4
      else
         nl=m4
      endif
      lw=lw+nl
c     
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     
c     call PLCBAS(H,P,C,D,CI,CS,IRA,MI,MD,X,F,W,IV,LAGR,IMP,IO,N,
c     &                  MODO,INFO)
      call plcbas(stk(l2),stk(l3),stk(l4),stk(l5),stk(l6),stk(l7),
     &     ira,mi,md,stk(lw9),stk(lw10),stk(lw11),stk(lw12),
     &     stk(lw13),imp,wte,n1,modo,info)
      if (info .ne. 0) then
         if (info .eq. 1) then
            call msgs(104,0)
         elseif (info .eq. -1) then
            call error(123)
            return
         elseif (info .eq. -2) then
            call msgs(11,0)
         elseif (info .eq. -3) then
            call error(125)
            return
         elseif (info .eq. -4) then
            call error(42)
            return
         elseif (info .eq. -11) then
            call error(126)
            return
         elseif (info .eq. -12) then
            call error(127)
            return
         elseif (info .eq. -13) then
            call error(128)
            return
         elseif (info .eq. -14) then
            call error(129)
            return
         endif
      endif
      
c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
c     Variable de sortie: x
c     
      top=top+1
      ilw=iadr(lw)
      err=lw+4+n1-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=n1
      istk(ilw+2)=1
      istk(ilw+3)=0
      lw=sadr(ilw+4)
      call unsfdcopy(n1,stk(lw9),1,stk(lw),1)
      lw=lw+n1
      lstk(top+1)=lw-mv
c     
c     Variable de sortie: f
c     
      top=top+1
      ilw=iadr(lw)
      err=lw+5-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      istk(ilw)=1
      istk(ilw+1)=1
      istk(ilw+2)=1
      istk(ilw+3)=0
      lw=sadr(ilw+4)
      call unsfdcopy(1,stk(lw10),1,stk(lw),1)
      lw=lw+1
      lstk(top+1)=lw-mv
c     
      if ( lhs .ge. 3) then
c     Variable de sortie: lagr
c     
         top=top+1
         ilw=iadr(lw)
         err=lw+4+m4-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         istk(ilw)=1
         istk(ilw+1)=nl
         istk(ilw+2)=1
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(nl,stk(lw13),1,stk(lw),1)
         lw=lw+nl
         lstk(top+1)=lw-mv
      endif
      
c     Remise en place de la pile
      call unsfdcopy(lw-lw0,stk(lw0),1,stk(l0),1)
      return
c     
 500  continue
c     
c     SCILAB function : semidef
c     --------------------------
      lw = lstk(top+1)
      l0 = lstk(top+1-rhs)
      if (rhs .ne. 6) then
         call error(39)
         return
      endif
      if (lhs .gt. 4) then
         call error(41)
         return
      endif
c     checking variable x (number 1)
c     
      il1 = iadr(lstk(top-rhs+1))
      if (istk(il1) .ne. 1) then
         err = 1
         call error(53)
         return
      endif
      m = istk(il1+1)*istk(il1+2)
      l1 = sadr(il1+4)
c     checking variable Z (number 2)
c     
      il2 = iadr(lstk(top-rhs+2))
      if (istk(il2) .ne. 1) then
         err = 2
         call error(53)
         return
      endif
      m2 = istk(il2+1)*istk(il2+2)
      l2 = sadr(il2+4)
c     checking variable F (number 3)
c     
      il3 = iadr(lstk(top-rhs+3))
      if (istk(il3) .ne. 1) then
         err = 3
         call error(53)
         return
      endif
      m3 = istk(il3+1)*istk(il3+2)
      l3 = sadr(il3+4)
c     checking variable ind (number 4)
c     
      il4 = iadr(lstk(top-rhs+4))
      if (istk(il4) .ne. 1) then
         err = 4
         call error(53)
         return
      endif
      m4 = istk(il4+1)*istk(il4+2)
      l4 = sadr(il4+4)
c     checking variable c (number 5)
c     
      il5 = iadr(lstk(top-rhs+5))
      if (istk(il5) .ne. 1) then
         err = 5
         call error(53)
         return
      endif
      m5 = istk(il5+1)*istk(il5+2)
      l5 = sadr(il5+4)
c     checking variable pars (number 6)
c     
      il6 = iadr(lstk(top-rhs+6))
      if (istk(il6) .ne. 1) then
         err = 6
         call error(53)
         return
      endif
      m6 = istk(il6+1)*istk(il6+2)
      if (m6 .ne. 5) then
         err = 6
         call error(89)
         return
      endif
      l6 = sadr(il6+4)
      nu=stk(l6)
      abstol=stk(l6+1)
      reltol=stk(l6+2)
      tv=stk(l6+3)
      iters=stk(l6+4)
c     
c     cross variable size checking
c     
      if (m .ne. m5) then
         call error(42)
         return
      endif
      call entier(m4,stk(l4),istk(iadr(l4)))
      nn8=2
      lw8=lw
      lw=lw+nn8
      n=0
      sz=0
      upsz=0
      maxn=0
      lind=iadr(l4)
      do 10 i=0,m4-1
         indi=istk(lind+i)
         n=n+indi
         sz=sz+indi*(indi+1)/2
         upsz=upsz+indi*indi
         maxn=max(maxn,indi)
 10   continue
c     optimal block size for dgels ????
      nb=32
      lwork=(m+2)*sz+upsz+2*n+max(m+sz*nb,3*maxn+maxn*(maxn+1),3*m)
      nn14=lwork
      lw14=lw
      lw=lw+nn14
      ilw15=iadr(lw)
      lw=sadr(ilw15+m)
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
c     call sp(m, L,  F,     ind,           c,      x,      Z,
      call spf(m,m4,stk(l3),istk(iadr(l4)),stk(l5),stk(l1),stk(l2),
c     ul,      nu,abstol,reltol,tv,iters,work,     lwork,info)
     &     stk(lw8),nu,abstol,reltol,tv,iters,stk(lw14),lwork,
     &     istk(ilw15),info)
      if(info.lt.0) then
         call error(230)
         return
      endif

c     
      top=top-rhs
      lw0=lw
      mv=lw0-l0
c     
      if(lhs .ge. 1) then
c     
c     output variable: x
c     
         top=top+1
      endif
      if(lhs .ge. 2) then
c     
c     output variable: Z
c     
         top=top+1
      endif
      if(lhs .ge. 3) then
c     
c     output variable: ul
c     
         top=top+1
         lw=lstk(top)
         err=lw+4+nn8-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         ilw=iadr(lw)
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=nn8
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         call unsfdcopy(nn8,stk(lw8),1,stk(lw),1)
         lw=lw+nn8
         lstk(top+1)=lw
      endif
c     
      if(lhs .ge. 4) then
c     
c     output variable: [info iters]
c     
         top=top+1
         lw=lstk(top)
         err=lw+4+2-lstk(bot)
         if (err .gt. 0) then
            call error(17)
            return
         endif
         ilw=iadr(lw)
         istk(ilw)=1
         istk(ilw+1)=1
         istk(ilw+2)=2
         istk(ilw+3)=0
         lw=sadr(ilw+4)
         stk(lw)=info
         stk(lw+1)=iters
         lw=lw+2
         lstk(top+1)=lw
      endif
      return
c
c     fsolve
c     
 600  call scisolv("fsolve")
      return
c
c     lsqrsolve
c     
 700  call intlsqrsolve("lsqrsolve")
      return


      end


      subroutine scisolv(fname)
c     ----------------------------
c     Scilab fsolve function 
c     ---------------------------
      character*(*) fname
c      implicit undefined (a-z)
      include '../stack.h'
      integer topk,kres,kjac,kx,m1,n1,lr1,lr,lw,gettype
      logical checklhs,checkrhs,getrmat,getexternal,cremat,jac
      logical type,getscalar
      double precision tol
      external bsolv,bjsolv,setfsolvf,setfsolvj
      character*(nlgh+1) namef,namej
      common/csolve/namef,namej
      integer iadr, sadr
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1
      topk = top
      if (.not.checkrhs(fname,2,4)) return
      if (.not.checklhs(fname,1,3)) return
c     checking variable x (number 1)
c     
      kx=top-rhs+1
      if(.not.getrmat(fname,topk,kx,m1,n1,lr1))return
      mn1=m1*n1
c     checking variable fcn (number 2)
c     
      kres=top-rhs+2
      if (.not.getexternal(fname,topk,kres,namef,type,
     $     setfsolvf)) return
c     
c     checking variable jac (number 3)
c     
      jac=.false.
      kjac=0
      iskip=0
      if (rhs.ge.3) then 
         itype=gettype(top-rhs+3) 
         if (itype.eq.13.or.itype.eq.10.or.itype.eq.11.or.
     $        itype.eq.15) then 
            jac=.true.
            kjac=top-rhs+3
            if (.not.getexternal(fname,topk,kjac,namej,
     $           type,setfsolvj)) return
         else
            iskip=1
         endif
      endif
c     checking variable tol (number 4)
c     
      if(rhs.eq.4-iskip) then
         if(.not.getscalar(fname,topk,top,lr4))return
         tol=stk(lr4)
      else
         tol=1.d-10
      endif
c     
c     An Ugly feature to keep track of externals 
C     -------------------------------------------
      top=top+1
      lw = lstk(top)
      ilext=iadr(lw)
      istk(ilext)=2
      istk(ilext+1)=ilext+4
      istk(ilext+2)=ilext+7
      istk(ilext+3)=ilext+10
      istk(ilext+4)=kres
      istk(ilext+5)=mn1
      istk(ilext+6)=kx
      istk(ilext+7)=kjac
      istk(ilext+8)=mn1
      istk(ilext+9)=kx
c     Working areas 
C     -------------------------------------------
      lw=sadr(ilext+10)
      lw4=lw
      lw=lw4+mn1
      lw6=lw
      lw=lw+1
      if(jac) then
         nn7=(mn1*(mn1+13))/2+mn1*mn1
      else
         nn7=(mn1*(3*mn1+13))/2
      endif
      lw7=lw
      lw=lw+nn7
      err=lw-lstk(bot)
      if (err .gt. 0) then
         call error(17)
         return
      endif
      lstk(top+1)=lw
      if(jac) then
         call hybrj1(bjsolv,mn1,stk(lr1),stk(lw4),stk(lw7),
     $        mn1,tol,info,stk(lw7+mn1*mn1),nn7)
      else
         call hybrd1(bsolv,mn1,stk(lr1),stk(lw4),tol,info,
     $        stk(lw7),nn7)
      endif 
      if(err.gt.0) return
c     
      top=top-1
      if(lhs.eq.1) then
         top=top-rhs+1
         goto 999
      endif
      top=top-rhs+2
      if (.not.cremat(fname,top,0,m1,n1,lr,lc)) return
      call unsfdcopy(mn1,stk(lw4),1,stk(lr),1)
      if(lhs.eq.3) then
c     info = 0   improper input parameters.
c     info = 1   algorithm estimates that the relative error
c     between x and the solution is at most tol.
c     info = 2   number of calls to fcn with iflag = 1 has
c     reached 100*(n+1).
c     info = 3   tol is too small. no further improvement in
c     the approximate solution x is possible.
c     info = 4   iteration is not making good progress.
         top=top+1
         if (.not.cremat(fname,top,0,1,1,lr,lc)) return
         stk(lr)=info
      endif
      goto 999
      
c     
 999  continue
      return
      end
      





