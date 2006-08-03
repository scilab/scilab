      subroutine n1gc2 (simul,prosca,n,x,f,g,dxmin,df1,epsrel,imp,io,
     /                  mode,niter,nsim,rz,nrz,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
c!but
c     minimisation sans contraintes par un algorithme de quasi-Newton
c     a memoire limitee
c!origine
c     c. lemarechal, inria, 1987
c     Copyright INRIA
c!commentaires
c le sous-programme n1gc2 (gradient conjugue a encombrement variable)
c est une interface entre le programme appelant et le sous-programme
c n1gc2a, minimiseur proprement dit.
c nrz est la dimension declaree pour le tableau de travail rz.
c rz est subdivise en 4 vecteurs de dimension n
c                     et un tableau de dimension memh.
c memh est la dimension allouee a la matrice de quasi newton h.
c pour l'usage de la liste d'appel : voir la documentation de n1qn1
c!
      double precision  zero, un
      parameter ( zero=0.0d+0 , un=1.0d+0 )
c declaration des tableaux
      double precision   x(n), g(n), rz(nrz), dzs(*)
      real rzs(*)
      integer izs(*)
c declaration des scalaires
      double precision   f, epsrel, dxmin, df1
      integer   n, nrz, imp, nsim, mode
      integer    id, ix, ig, iaux, ih, memh
c
      external    simul, prosca
c
      if (imp .gt. 0) write(io,1)n,nrz,niter,nsim,imp,
     / epsrel,df1,dxmin
1     format(19h entree dans n1gc2:,6x,22hdimension du probleme ,
     /i3/2x,4hnrz=,i4,4x,6hniter=,i3,4x,5hnsim=,i4,4x,4himp=,i3/2x,
     /7hepsrel=,d8.2,4x,4hdf1=,d8.2,4x,6hdxmin=,d8.2)
      if ( n.le.0 .or. niter.le.0 .or. nsim.le.0 .or.
     / dxmin.le.zero .or. df1.le.zero
     / .or. epsrel.le.zero .or. epsrel.gt.un ) then
      mode=2
      if (imp .gt. 0) write(io,3)
      return
      endif
c
c calculs des pointeurs destines a subdiviser le tableau rz
      id=1
      ix=id +n
      ig=ix +n
      iaux=ig +n
      ih=iaux + n
c
c calcul du nombre de places memoire affectees a h
      memh=nrz - 4*n
c
      if (memh .le. 0) then
      mode=3
      goto 100
      else
      continue
      endif
c
c appel du sous-programme n1gc2a qui effectue la reelle optimisation
      call n1gc2a(simul,prosca,n,x,f,g,dxmin,df1,epsrel,imp,io,
     /            niter,nsim,mode,memh,rz(id),rz(ix),rz(ig),
     /            rz(iaux),rz(ih),izs,rzs,dzs)
c
100   if (imp .gt. 0) then
      if (mode .eq. 3) then
      write(io,2)
      else if (mode .eq. 6) then
      write(io,4)
      else
      write(io,5)epsrel,niter,nsim
      endif
      endif
      return
2     format(/,38h n1gc2   rz insuffisamment dimensionne)
3     format(/,25h n1gc2   appel incoherent)
4     format(/,22h n1gc2   fin sur dxmin)
5     format(/,16h sortie de n1gc2,7x,12hnorme de g =,d15.9/9x,
     / 6hniter=,i4,4x,5hnsim=,i5)
      end
