      subroutine gcbd(indgc,simul,nomf,n,x,f,g,imp,io,zero,
     &napmax,itmax,epsf,epsg,epsx,df0,binf,bsup,nfac,
     &vect,nvect,ivect,nivect,izs,rzs,dzs)
c!but
c     algorithme de minimisation d une fonction reguliere sous
c     contraintes de borne
c!methode
c     methode de bfgs a memoire limitee + projection
c!origine
c     f. bonnans inria juin 1985
c     Copyright INRIA
c
c!sous programmes (modulopt)
c     proj rlbd majysa majz calbx gcp relvar bfgsd shanph
c!liste d' appel
c     indgc   indicateur de gcbd                                  es
c       en entree =1 standard
c                 =2 dh et indic initialises au debut de trav et itrav
c                    ifac,f,g initialises
c       en sortie
c        si < 0 incapacite de calculer un point meilleur que le point initial
c        si = 0 arret demande par l utilisateur
c        si > 0 on fournit un point meilleur que le point de depart
c        = -14 insuffisance memoire
c        = -13 indgc non egal a zero ou 1 en entree
c        = -12 zero,epsg ou df0 non strict. positifs
c        = -11 n,napmax,itmax ou io non strict. positifs
c        < -10 parametres d entree non convenables
c        = -6  arret lors du calcul de la direction de descente et iter=1
c        = -5  arret lors du calcul de l approximation du hessien  iter=1
c        = -3  anomalie de simul : indic negatif en un point ou
c              f et g ont ete precedemment calcules
c        = -2  echec de la recherche lineaire a la premiere iteration
c        = -1  f non definie au point initial
c        =  1  arret sur epsg
c        =  2            epsf
c        =  3            epsx
c        =  4            napmax
c        =  5            itmax
c        =  6  pente dans la direction opposee au gradient trop petite
c        =  7  arret lors du calcul de la direction de descente
c        =  8  arret lors du calcul de l approximation du hessien
c        = 10  arret par echec de la recherche lineaire , cause non precisee
c        = 11  idem avec indsim < 0
c        = 12            un pas trop petit proche d un pas trop grand
c                        ceci peut resulter d une erreur dans le gradient
c        = 13            trop grand nombre d appels dans une recherche lineaire
c
c     simul  subroutine permettant de calculer f et g (norme modulopt)
c     n dim de x                                                 e
c     x variables a optimiser (controle)                          es
c     f valeur du critere                                         s
c     g gradient de f                                             s
c     imp si =0 pas d impression
c             1  impressions en debut etfin dexecution
c             2  3 lignes a chaque iteration
c             >=3 nombreuses impressions    e
c     io numero fichier sortie            e
c     zero  proche zero machine                                             e
c     napmax nombre maximum d appels de simul                               e
c     itmax nombre maximum d iterations de gcbd                             e
c     epsf critere arret sur f            e
c     epsg arret si sup a norm2(g+)/n     e
c     epsx vect dim n precision sur x     e
c     df0>0 decroissance f prevue         e
c     binf,bsup bornes inf,sup,de dim n                          e
c     nfac nombre de variables non bloquees a l optimum          s
c     vect,ivect vecteurs de travail de dim nvect,nivect
c     izs,rzs,dzs : cf normes modulopt         es
c
c!
c         signification de quelques variables internes
c
c     {y}={g1}-{g0}                                        l (locale)
c     {s}={x1}-{x0}                                        l
c     {z}=[b]*{s}. [b] est une estimation de hessien       l
c     ys=<y>*{s}                                           l
c     zs=<z>*{s}                                           l
c     diag approximation diagonale du hessien  es
c     si indgc=0 diag initialise a *******************
c     puis remis a jour par bfgs diagonal
c     nt: le nombre de deplacements pris en compte          l
c     index(nt) repere les vect y,s,z                       l
c     wk1,wk2: vecteurs de travail de dim n                 l
c     ibloc vect dim n  ; si x(i) est bloque, ibloc(i)=iteration de blocage ;
c     si x(i) est libre, ibloc(i)=-1*(iteration de deblocage)
c     irit: irit=1, si relachement de vars a l'iter courante, 0 sinon
c     ired: ired=1 decision de redemarrage, 0 sinon
c     alg(1)=param
c     alg(2)=condmax
c     alg(6)=eps
c     alg(9)=tetaq ( critere de redemarrage)
c     ialg(1)       correction de powell sur y si (y,s)trop petit
c          0:       sans correction de powell
c          1:       avec correction
c     ialg(2)       mise a jour de diag par bfgsd
c          0:       pas de remise a jour
c          1:       remise a jour de diag par bfgsd
c     ialg(3)       mise a l'echelle par methode de shanno-phua
c          0:       pas de mise a l'echelle
c          1:       mise a l'echelle a toutes les iterations
c          2:       mise a l'echelle a la 2ieme iteration seulement
c     ialg(4):      memorisation pour choix iterations
c          0:                sans memorisation
c          1:      avec memorisation
c     ialg(5):      memorisation par variable
c          0:      sans memorisation
c          1:       avec memorisation
c     ialg(6):      choix des iterations de relachement
c          1:      relachement a toutes les iterations
c          2:      relachement si decroissance g norme gradient
c          10:     relachement si decroissance critere % iter.initcycle
c          11:      relachement si decroissance critere % decroissance cycle
c     ialg(7):      choix des variables a relacher
c          1:       methode de bertsekas modifiee
c     ialg(8):      choix de la direction de descente
c          3:      qn sans memoire: nt derniers deplacements
c          4:      redemarrage sans accumulation
c          5:      redemarrage avec accumulation
c     ialg(9):     critere de redemarrage
c          2:       redemarrage si fact. ou defact.
c          10:     decroissance critere % decroissance iter_init du cycle
c          11:     decroissance critere % decroissance totale du cycle
c          12:      diminution de znglib d un facteur alg(9)=tetaq
c     eps0 sert a partitionner les variables
c     ceps0 utilise dans le calcul de eps0
c     izag nombre d iterations min pendant lesquelles une var reste bloquee
c     nap nombre d appels de simul
c     iter iteration courante
c     ind indicateur de simul
c     icv memoire entree indgc
c     np  param utilise pour la gestion de vect. nb de vect courant.
c     lb  param utilise pour la gestion de vect. 1er place libre.
c     nb  param utilise pour la gestion de vect.
c        nb=2 si l'algorithme utilise est qn sans memoire +redem +pas acc
c        nb=1 sinon
c
      implicit double precision (a-h,o-z)
      real rzs(*)
      double precision dzs(*)
      dimension x(n),g(n),binf(n),bsup(n),epsx(n)
      dimension izs(*),vect(nvect),ivect(nivect),ialg(15),alg(15)
      character*6 nomf
      external simul
c
c     initialisation des parametres
      nt=2
      alg(1)= 1.0d-5
      alg(2)= 1.0d+6
      alg(6)=.50d+0
      alg(9)=.50d+0
c
      ialg(1)=1
      ialg(2)=0
      ialg(3)=2
      ialg(4)=0
      ialg(5)=0
      ialg(6)=2
      ialg(7)=1
      ialg(8)=4
      ialg(9)=12
c
c     verification des entrees
      ii=min(n,napmax,itmax)
      if(ii.gt.0)go to 10
      indgc=-11
      if(imp.gt.0) write(io,123)indgc
123   format(' gcbd : retour avec indgc=',i8)
      return
10    aa=min(zero,epsg,df0)
      do 11 i=1,n
11    aa=min(aa,epsx(i))
      if(aa.gt.0.0d+0) goto 12
      indgc=-12
      if(imp.gt.0) write(io,123) indgc
      return
12    continue
c
c     decoupage de la memoire
      ny=1
      ns=nt*n+ny
      nz=nt*n+ns
      nys=nt*n+nz
      nzs=nt+nys
      nd=nt+nzs
      ng=n+nd
      nx2=n+ng
      ndir=n+nx2
      ndiag=n+ndir
      nfin=n+ndiag
c
      if(nfin.gt.nvect) then
         write(io,1000)nfin,nvect
1000  format (' gcbd:insuffisance memoire; nvect=',i5,'devrait etre:',
     &  i5)
      indgc=-14
      return
      endif
c
      nindic=1
      nindex=n+nindic
      nfin=nt+nindex
      if(nfin.gt.nivect) then
         write(io,2000)nfin,nivect
2000  format (' gcbd:insuffisance memoire; nivect=',i5,'devrait etre:',
     &  i5)
      indgc=-14
      return
      endif
c
      call zgcbd(simul,n,binf,bsup,x,f,g,zero,napmax,itmax,indgc,ivect
     &(nindic),nfac,imp,io,epsx,epsf,epsg,vect(ndir),df0,vect(ndiag),
     &vect(nx2),izs,rzs,dzs,vect(ny),vect(ns),vect(nz),vect(nys),
     &vect(nzs),nt,ivect(nindex),vect(nd),vect(ng),alg,ialg,nomf)
      return
      end
