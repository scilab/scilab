c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1987 - INRIA - Claude LEMARECHAL
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine n1gc2 (simul,prosca,n,x,f,g,dxmin,df1,epsrel,imp,io,
     /                  mode,niter,nsim,rz,nrz,izs,rzs,dzs)
      implicit double precision (a-h,o-z)
c!but
c     minimisation sans contraintes par un algorithme de quasi-Newton
c     a memoire limitee
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
      character bufstr*(4096)
c
      external    simul, prosca
c
      if (imp .gt. 0) then
      
      write(bufstr,1) n
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr))) 
      
      write(bufstr,11) nrz,niter,nsim,imp
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr))) 
      
      write(bufstr,12) epsrel,df1,dxmin
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr))) 
      
      endif
1     format(19h entree dans n1gc2:,6x,22hdimension du probleme ,i3)
11    format(2x,4hnrz=,i4,4x,6hniter=,i3,4x,5hnsim=,i4,4x,4himp=,i3)
12    format(2x,7hepsrel=,d9.2,4x,4hdf1=,d9.2,4x,6hdxmin=,d9.2)
      if ( n.le.0 .or. niter.le.0 .or. nsim.le.0 .or.
     / dxmin.le.zero .or. df1.le.zero
     / .or. epsrel.le.zero .or. epsrel.gt.un ) then
      mode=2
      if (imp .gt. 0) then
        write(bufstr,3)
        call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      endif
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
      write(bufstr,2)
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      else if (mode .eq. 6) then
      write(bufstr,4)
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      else
      write(io,5)epsrel
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      write(io,51) niter,nsim
      call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
      endif
      endif
      return
2     format(38h n1gc2   rz insuffisamment dimensionne)
3     format(25h n1gc2   appel incoherent)
4     format(22h n1gc2   fin sur dxmin)
5     format(16h sortie de n1gc2,7x,12hnorme de g =,d16.9)
51     format(9x, 6hniter=,i4,4x,5hnsim=,i5)
      end
