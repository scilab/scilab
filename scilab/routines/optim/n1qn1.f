      subroutine n1qn1 (simul,n,x,f,g,var,eps,
     1     mode,niter,nsim,imp,lp,zm,izs,rzs,dzs)

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
      external simul
      if (imp.gt.0) then
         write(lp,*)
         write(lp,*) '***** enters -qn code- (without bound cstr)'
         write(lp,*) 'dimension=',n,', epsg=',eps, 
     $                ', verbosity level: imp=',imp
         write(lp,*) 'max number of iterations allowed: iter=',niter
         write(lp,*) 'max number of calls to costf allowed: nap=',nsim
         write(lp,*) '------------------------------------------------'
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
         write (lp,*) '***** leaves -qn code-, gradient norm=',sqrt(eps)
      endif
      end
