c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c This file must be used under the terms of the CeCILL.
c This source file is licensed as described in the file COPYING, which
c you should have received as part of this distribution.  The terms
c are also available at    
c http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
c
c     --------------------------
c     SCILAB function : optim
c     --------------------------
      subroutine scioptim(fname)
      
      character*(*) fname
	INCLUDE 'stack.h'
      double precision tol,df0,zng,dxmin
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
c
      integer impn(nsiz)
      logical eqid, getscalar
      integer iadr, sadr
c     
      parameter (nsiz1=nsiz-1)
      data impn/672732690,nsiz1*673720360/
c
      data coin,coar,coti,cotd,cosi,cosd,nfac
     &     /   5906,6922,4637,3357,4636,3356, 0/
c     
      
      iadr(l)=l+l-1
      sadr(l)=(l/2)+1

      napm=100
      itmax=100
      epsf=0.0d+0
      iepsx=1
      indtes=0
      imp=0
      io=wte
      tol=stk(leps)
      df0=1.0d+0
      immx=0
      epsg=tol
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
c
c Compute isim :
c - isim=1 if the cost function is given as a string (code 10), that is, the cost function is 
c   a C or Fortran routine.
c   In that case, the cost function is computed with a call to "foptim".
c - isim=2 if the cost function is given as a script (code 11), a compiled script (code 13),
c   or a list (code 15)
c   In that case, the cost function is computed with a call to "boptim".
c
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
c     cas simul=macro
      if(istk(il).eq.11.or.istk(il).eq.13.or.istk(il).eq.15) then
         kopt=top2
         isim=2
      endif
      top2=top2+1
      il=iadr(lstk(top2))
c     
c Compute icontr
c - icontr=1 if without constraints
c - icontr=2 if with bound constraints
c
c     contraintes de borne :chaine "b" (code 11), xinf , xsup
      if(istk(il).eq.10.and.istk(il+5).eq.2.and.
     +     abs(istk(il+6)).eq.11) then
         if (rhs.lt.5) then
            call error(39)
            return
         endif
         top2=top2+1
         il=iadr(lstk(top2))
         if(istk(il).gt.2.or.istk(il).eq.0)  then
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
         if(istk(il).gt.2.or.istk(il).eq.0)  then
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
      if(istk(il).gt.2.or.istk(il).eq.0)  then
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
c
c Compute ialg1
c     (26,23) > "qn" > Quasi-Newton > ialg1=1
c     (16,12) > "gc" > Gradient Conjugate > ialg1=2
c     (23,13) > "nd" > Non Differentiable > ialg1=10
c
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
c
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
      if(abs(istk(il+6))+256*abs(istk(il+7)).eq. coin) then
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
         if(err.gt.0.or.err1.gt.0) return
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
c     mettre ti = izs et/ou td = dzs en sortie (chaine 'si' ou 'sd')
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
         if(err.gt.0.or.err1.gt.0)return
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
c     optimiseur n1qn1 : Quasi-Newton without constraints
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
         if(err.gt.0.or.err1.gt.0) return
c     affectation de indopt
         epsg=sqrt(epsg)
         indopt=9
         if(itmax.ge.itmax1) indopt=5
         if(napm.ge.napm1) indopt=4
         if(epsg1.ge.epsg) indopt=1
         go to 300
      endif

c     algorithme n1qn3 : Gradient Conjugate without constraints
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
         if (err.gt.0.or.err1.gt.0) return
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
c     optimiseur n1fc1 : non smooth without constraints
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
               stk(lepsx+i - 1)=tol
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
     &           stk(lepsx),df0,epsf,tol,imp,io,mode,itmax,napm,mmx,
     &           istk(litv),stk(ltv1),stk(ltv2),istk(lizs),sstk(lrzs),
     $           stk(ldzs))
         else
            call n1fc1(boptim,fuclid,nx,stk(lx),stk(lf),stk(lg),
     &           stk(lepsx),df0,epsf,tol,imp,io,mode,itmax,napm,mmx,
     &           istk(litv),stk(ltv1),stk(ltv2),istk(lizs),sstk(lrzs),
     $           stk(ldzs))
         endif
         if (err.gt.0.or.err1.gt.0) return
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
c     optimiseur qnbd : quasi-newton with bound constraints
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
               stk(lepsx+i)=tol
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
     &           stk(lg),imp,io,tol,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call qnbd(indopt,boptim,nx,stk(lx),stk(lf),
     &           stk(lg),imp,io,tol,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0.or.err1.gt.0) return
         go to 300
      endif
c     
c     optimiseur gcbd : Gradient Conjugate with bound constraints
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
               stk(lepsx+i)=tol
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
     &           stk(lf),stk(lg),imp,io,tol,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         else
            call gcbd(indopt,boptim,nomsub,nx,stk(lx),stk(lf),
     &           stk(lg),imp,io,tol,napm,itmax,epsf,epsg,
     &           stk(lepsx),df0,stk(lbi),stk(lbs),nfac,
     &           stk(ltv),ntv,istk(litv),nitv,
     &           istk(lizs),sstk(lrzs),stk(ldzs))
         endif
         if(err.gt.0.or.err1.gt.0) return
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
            call writebufscioptim(buf,epsg)
            call msgs(12,0)
         elseif(indopt.eq.2) then
            call writebufscioptim(buf,epsg)
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
      end
c     --------------------------

