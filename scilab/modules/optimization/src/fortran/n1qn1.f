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
      subroutine n1qn1 (simul,n,x,f,g,var,eps,
     1     mode,niter,nsim,imp,lp,zm,izs,rzs,dzs)
c
c!but
c     minimisation d une fonction reguliere sans contraintes
c!origine
c     c. lemarechal, inria, 1987
c     Copyright INRIA
c!methode
c     direction de descente calculee par une methode de quasi-newton
c     recherche lineaire de type wolfe
c!liste d appel
c     simul    : point d'entree au module de simulation (cf normes modulopt i)
c     n1qn1 appelle toujours simul avec indic = 4 ; le module de
c     simulation doit se presenter sous la forme subroutine simul
c     (n,x, f, g, izs, rzs, dzs) et e^tre declare en external dans le
c     programme appelant n1qn1.
c     n (e)    : nombre de variables dont depend f.
c     x (e-s)   : vecteur de dimension n ; en entree le point initial ;
c                 en sortie : le point final calcule par n1qn1.
c     f (e-s)   : scalaire ; en entree valeur de f en x (initial), en sortie
c                 valeur de f en x (final).
c     g (e-s)   : vecteur de dimension n : en entree valeur du gradient en x
c                 (initial), en sortie valeur du gradient en x (final).
c     var (e)   : vecteur strictement positif de dimension n. amplitude de la
c                 modif souhaitee a la premiere iteration sur x(i).une bonne
c                 valeur est 10% de la difference (en valeur absolue) avec la
c                 coordonee x(i) optimale
c     eps (e-s) : en entree scalaire definit la precision du test d'arret.
c      le programme considere que la convergence est obtenue lorque il lui
c      est impossible de diminuer f en attribuant a au moins une coordonnee
c      x(i) une variation superieure a eps*var(i).
c      en sortie, eps contient le carre de la norme du gradient en x (final).
c     mode (e)     : definit l approximation initiale du hessien
c                  =1 n1qn1 l initialise lui-meme
c                  =2 le hessien est fourni dans zm sous forme compressee (zm
c                     contient les colonnes de la partie inferieure du hessien)
c     niter (e-s)  : en entree nombre maximal d'iterations : en sortie nombre
c                    d'iterations reellement effectuees.
c     nsim (e-s)  : en entree nombre maximal d'appels a simul (c'est a dire
c         avec indic = 4). en sortie le nombre de tels appels reellement faits.
c      imp (e)   : contro^le les messages d'impression :
c                  0 rien n'est imprime
c                  = 1 impressions initiales et finales
c                  = 2 une impression par iteration (nombre d'iterations,
c                      nombre d'appels a simul, valeur courante de f).
c                  >=3 informations supplementaires sur les recherches
c                      lineaires ;
c                      tres utile pour detecter les erreurs dans le gradient.
c      lp (e)    : le numero du canal de sortie, i.e. les impressions
c                  commandees par imp sont faites par write (lp, format).
c     zm     : memoire de travail pour n1qn1 de   dimension n*(n+13)/2.
c     izs,rzs,dzs memoires reservees au simulateur (cf doc)
c
c!
      implicit double precision (a-h,o-z)
      dimension x(n),g(n),var(n),zm(*),izs(*),dzs(*)
      real rzs(*)
      character bufstr*(4096)
      external simul
      if (imp.gt.0) then
         call basout(io, lp, '')
         call basout(io, lp, 
     $    '***** enters -qn code- (without bound cstr)')

         write(bufstr,750)n,eps,imp
         call basout(io ,lp ,bufstr(1:lnblnk(bufstr)))

750   	 format('dimension=',i10,', epsq=',e24.16,
     $ ', verbosity level: imp=',i10)


         
         write(bufstr,751)niter
         call basout(io ,lp ,bufstr(1:lnblnk(bufstr)))
751   	 format('max number of iterations allowed: iter=',i10)

         
         write(bufstr,752) nsim
         call basout(io ,lp ,bufstr(1:lnblnk(bufstr)))
752   	 format('max number of calls to costf allowed: nap=',i10)
         
         call basout(io ,lp ,
     $    '------------------------------------------------')
      endif
      nd=1+(n*(n+1))/2
      nw=nd+n
      nxa=nw+n
      nga=nxa+n
      nxb=nga+n
      ngb=nxb+n
      call n1qn1a (simul,n,x,f,g,var,eps,mode,
     1 niter,nsim,imp,lp,zm,zm(nd),zm(nw),zm(nxa),zm(nga),
     2 zm(nxb),zm(ngb),izs,rzs,dzs)
      if (imp.gt.0) then
       write(bufstr,753) sqrt(eps)
       call basout(io ,lp ,bufstr(1:lnblnk(bufstr)))
753    format('***** leaves -qn code-, gradient norm=',e24.16)
     
      endif
      end
