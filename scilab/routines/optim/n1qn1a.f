      subroutine n1qn1a (simul,n,x,f,g,scale,acc,mode,
     1     niter,nsim,iprint,lp,h,d,w,xa,ga,xb,gb,izs,rzs,dzs)
c     Copyright INRIA

*     A (very) few modifs by Bruno (14 March 2005): I have translated some output
*     informations in english (but they don't use format instruction 
*     which is put in the secong arg of write). Also for the linear
*     search output informations I divide by the direction vector norm
*     to get the "normalized" directionnal derivative. Note that this is
*     just for output (the computing code is normaly not modified).

      implicit double precision (a-h,o-z)
      dimension x(n),g(n),scale(n),h(*),d(n),w(n),
     1 xa(n),ga(n),xb(n),gb(n),izs(*),dzs(*)
      real rzs(*)
      external simul
      double precision dnrm2 ! (blas routine) added by Bruno to get
                                ! a better information concerning directionnal derivative
      integer vfinite ! added by Serge to avoid Inf and Nan's
 1000 format (46h n1qn1 ne peut demarrer (contrainte implicite))
 1001 format (40h n1qn1 termine par voeu de l'utilisateur)
 1010 format (45h n1qn1 remplace le hessien initial (qui n'est,
     1 20h pas defini positif)/27h par une diagonale positive)
 1023 format (40h n1qn1 bute sur une contrainte implicite)
c
c              calcul initial de fonction-gradient
c
      indic=4
      call simul (indic,n,x,f,g,izs,rzs,dzs)
c     next line added by Serge to avoid Inf and Nan's (04/2007)
      if (vfinite(1,f).ne.1.and.vfinite(n,g).ne.1) indic=-1
      if (indic.gt.0) go to 13
      if (iprint.eq.0) go to 12
      if (indic.lt.0) write (lp,1000)
      if (indic.eq.0) write (lp,1001)
   12 acc=0.0d+0
      niter=1
      nsim=1
      return
   13 nfun=1
      iecri=0
      itr=0
      np=n+1
c                  initialisation du hessien, en fonction de var
      if (mode.ge.2) go to 60
   20 c=0.0d+0
      do 30 i=1,n
   30 c=max(c,abs(g(i)*scale(i)))
      if (c.le.0.0d+0) c=1.0d+0
      k=(n*np)/2
      do 40 i=1,k
   40 h(i)=0.0d+0
      k=1
      do 50 i=1,n
      h(k)=0.010d+0*c/(scale(i)*scale(i))
   50 k=k+np-i
      go to 100
c               factorisation du hessien
   60 if (mode.ge.3) go to 80
      k=n
      if(n.gt.1) go to 300
      if(h(1).gt.0.0d+0) go to 305
      h(1)=0.0d+0
      k=0
      go to 305
  300 continue
      np=n+1
      ii=1
      do 304 i=2,n
      hh=h(ii)
      ni=ii+np-i
      if(hh.gt.0.0d+0) go to 301
      h(ii)=0.0d+0
      k=k-1
      ii=ni+1
      go to 304
  301 continue
      ip=ii+1
      ii=ni+1
      jk=ii
      do 303 ij=ip,ni
      v=h(ij)/hh
      do 302 ik=ij,ni
      h(jk)=h(jk)-h(ik)*v
  302 jk=jk+1
  303 h(ij)=v
  304 continue
      if(h(ii).gt.0.0d+0) go to 305
      h(ii)=0.0d+0
      k=k-1
 305  continue
c
      if (k.ge.n) go to 100
   70 if (iprint.ne.0) write (lp,1010)
      go to 20
c                verification que la diagonale est positive
   80 k=1
      do 90 i=1,n
      if (h(k).le.0.0d+0) go to 70
   90 k=k+np-i
c                quelques initialisations
  100 dff=0.0d+0
  110 fa=f
      isfv=1
      do 120 i=1,n
      xa(i)=x(i)
  120 ga(i)=g(i)
c                   iteration
  130 itr=itr+1
      ial=0
      if (itr.gt.niter) go to 250
      iecri=iecri+1
      if (iecri.ne.-iprint) go to 140
      iecri=0
      indic=1
      call simul(indic,n,x,f,g,izs,rzs,dzs)
c               calcul de la direction de recherche
  140 do 150 i=1,n
  150 d(i)=-ga(i)
      w(1)=d(1)
      if(n.gt.1)go to 400
      d(1)=d(1)/h(1)
      go to 412
  400 continue
      do 402 i=2,n
      ij=i
      i1=i-1
      v=d(i)
      do 401 j=1,i1
      v=v-h(ij)*d(j)
  401 ij=ij+n-j
      w(i)=v
  402 d(i)=v
      d(n)=d(n)/h(ij)
      np=n+1
      do 411 nip=2,n
      i=np-nip
      ii=ij-nip
      v=d(i)/h(ii)
      ip=i+1
      ij=ii
      do 410 j=ip,n
      ii=ii+1
  410 v=v-h(ii)*d(j)
  411 d(i)=v
  412 continue
c               calcul du pas minimum
c               et de la derivee directionnelle initiale
      c=0.0d+0
      dga=0.0d+0
      do 160 i=1,n
      c=max(c,abs(d(i)/scale(i)))
  160 dga=dga+ga(i)*d(i)
c               test si la direction est de descente
      if (dga.ge.0.0d+0) go to 240
c               initialisation du pas
      stmin=0.0d+0
      stepbd=0.0d+0
      steplb=acc/c
      fmin=fa
      gmin=dga
      step=1.0d+0
      if (dff.le.0.0d+0) step=min(step,1.0d+0/c)
      if (dff.gt.0.0d+0) step=min(step,(dff+dff)/(-dga))

      if (iprint.ge.2) then
         write (lp,'(A,I4,A,I4,A,G10.4)') ' iter num ',itr,
     $                ', nb calls=',nfun,', f=',fa
         if (iprint.ge.3) then
            write (lp,'(A,G10.4)')
     $            ' linear search: initial derivative=',dga/dnrm2(n,d,1)
         endif
      endif
c              boucle de reherche lineaire
  170 c=stmin+step
      if (nfun.ge.nsim) go to 250
      nfun=nfun+1
c              calcul de fonction-gradient
      do 180 i=1,n
  180 xb(i)=xa(i)+c*d(i)
      indic=4
      call simul (indic,n,xb,fb,gb,izs,rzs,dzs)
c     next line added by Serge to avoid Inf and Nan's (04/2007)
      if (vfinite(1,fb).ne.1.and.vfinite(n,gb).ne.1) indic=-1
c              test sur indic
      if (indic.gt.0) goto 185
      if (indic.lt.0) goto 183
      if (iprint.gt.0) write (lp,1001)
      do 182 i=1,n
      x(i)=xb(i)
  182 g(i)=gb(i)
      go to 250
  183 stepbd=step
      ial=1
      step=step/10.0d+0
      if (iprint.ge.3) then
         write (lp,'(A,G10.4,A,I2)') 
     $   '                step length=',c,', indic=',indic
      endif
      if (stepbd.gt.steplb) goto 170
      if (iprint.ne.0.and.isfv.lt.2) write (lp,1023)
      goto 240
c             stockage si c'est la plus petite valeur
  185 isfv=min(2,isfv)
      if (fb.gt.f) go to 220
      if (fb.lt.f) go to 200
      gl1=0.0d+0
      gl2=0.0d+0
      do 190 i=1,n
      gl1=gl1+(scale(i)*g(i))**2
  190 gl2=gl2+(scale(i)*gb(i))**2
      if (gl2.ge.gl1) go to 220
  200 isfv=3
      f=fb
      do 210 i=1,n
      x(i)=xb(i)
  210 g(i)=gb(i)
c               calcul de la derivee directionnelle
  220 dgb=0.0d+0
      do 230 i=1,n
  230 dgb=dgb+gb(i)*d(i)
      if (iprint.lt.3) goto 231
      s=fb-fa
* a small change (Bruno): to give a better indication about
*  the directionnal derivative I scale it by || d ||
      write (lp,'(A,G10.4,A,G10.4,A,G10.4)')
     $  '                step length=',c,
     $  ', df=',s,', derivative=',dgb/dnrm2(n,d,1)
c               test si la fonction a descendu
  231 if (fb-fa.le.0.10d+0*c*dga) go to 280
      ial=0
c               iteration terminee si le pas est minimum
      if (step.gt.steplb) go to 270
  240 if (isfv.ge.2) go to 110
c               ici, tout est termine
  250 if (iprint.gt.0) then
         write (lp,'(A,I4,A,I4,A,G10.4)') ' iter num ',itr,
     $                ', nb calls=',nfun,', f=',f
      endif
      acc=0.0d+0
      do 260 i=1,n
  260 acc=acc+g(i)*g(i)
      niter=itr
      nsim=nfun
      return
c               interpolation cubique
  270 stepbd=step
      c=gmin+dgb-3.0d+0*(fb-fmin)/step
      if(c.eq.0.0d+0) goto 250
      cc=abs(c)-gmin*(dgb/abs(c))
      cc=sqrt(abs(c))*sqrt(max(0.0d+0,cc))
      c=(c-gmin+cc)/(dgb-gmin+cc+cc)
      step=step*max(0.10d+0,c)
      go to 170
c               ceci est un pas de descente
  280 if (ial.eq.0) goto 285
      if (stepbd.gt.steplb) go to 285
      if (iprint.ne.0.and.isfv.lt.2) write (lp,1023)
      go to 240
  285 stepbd=stepbd-step
      stmin=c
      fmin=fb
      gmin=dgb
c               extrapolation
      step=9.0d+0*stmin
      if (stepbd.gt.0.0d+0) step=0.50d+0*stepbd
      c=dga+3.0d+0*dgb-4.0d+0*(fb-fa)/stmin
      if (c.gt.0.0d+0) step=min(step,stmin*max(1.0d+0,-dgb/c))
      if (dgb.lt.0.70d+0*dga) go to 170
c                 recherche lineaire terminee, test de convergence
      isfv=4-isfv
      if (stmin+step.le.steplb) go to 240
c                 formule de bfgs
      ir=-n
      do 290 i=1,n
      xa(i)=xb(i)
      xb(i)=ga(i)
      d(i)=gb(i)-ga(i)
  290 ga(i)=gb(i)
      call majour(h,xb,w,n,1.0d+0/dga,ir,1,0.0d+0)
      ir=-ir
      call majour(h,d,d,n,1.0d+0/(stmin*(dgb-dga)),ir,1,0.0d+0)
c                  test du rang de la nouvelle matrice
      if (ir.lt.n) go to 250
c                  nouvelle iteration
      dff=fa-fb
      fa=fb
      go to 130
      end
