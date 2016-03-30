c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) 1986 - INRIA - F. BONNANS
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
      subroutine qnbd(indqn,simul,n,x,f,g,imp,io,zero,
     & napmax,itmax,epsf,epsg,epsx,df0,binf,bsup,nfac,
     & trav,ntrav,itrav,nitrav,izs,rzs,dzs)
c!but
c     code de minimisation d une fonction reguliere sous contraintes
c     de bornes , aux normes modulopt
c!methode
c     principe de l algorithme : quasi-newton + projection
c     details dans le rapport inria n. 242,1983
c     cette version permet de tester plusieurs variantes de l algorithme
c     en modifiant certains parametres internes (cf comment dans le code)
c     taille memoire necessaire de l ordre de n**2/2
c     pour les problemes de grande taille le code gcbd est mieux adapte
c
c!sous programmes appeles
c     zqnbd   optimiseur effectif
c     proj    projection
c     calmaj  mise a jour du hessien 
c     ajour mise a jour des facteurs de choleski 
c     rlbd,satur   recherche lineaire avec bornes
c
c!liste d'appel
c
c     subroutine qnbd(indqn,simul,n,x,f,g,imp,io,zero,
c    & napmax,itmax,epsf,epsg,epsx,df0,binf,bsup,nfac,
c    & trav,ntrav,itrav,nitrav,izs,rzs,dzs)
c
c     indqn   indicateur de qnbd                                  es
c       en entree =1 standard
c                 =2 dh et indic initialises au debut de trav et itrav
c                    ifac,f,g initialises
c       en sortie
c        si < 0 incapacite de calculer un point meilleur que le point initial
c        si = 0 arret demande par l utilisateur
c        si > 0 on fournit un point meilleur que le point de depart
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
c     simul voir les normes modulopt
c     n dim de x                                                 e
c     binf,bsup bornes inf,sup,de dim n                          e
c     x variables a optimiser (controle)                          es
c     f valeur du critere                                         s
c     g gradient de f                                             s
c     zero  proche zero machine                                             e
c     napmax nombre maximum d appels de simul                               e
c     itmax nombre maximum d iterations de descente               e
c     itrav vect travail dim nitrav=2n , se decompose en indic et izig
c     nfac nombre de variables factorisees                  (e si indqn=2)  s
c     imp facteur d impression                                              e
c         varie de 0 (pas d impressions) a 3 (nombreuses impressions)
c     io numero du fichier de resultats                                     e
c     epsx vect dim n precision sur x                                       e
c     epsf critere arret sur f                                              e
c     epsg arret si sup a norm2(g+)/n                                       e
c     trav vect travail dim ntrav
c     il faut ntrav > n(n+1)/2 +6n
c     df0>0 decroissance f prevue     (prendre 1. par defaut)               e
c     izs,rzs,dzs : cf normes modulopt                                     es
c!
c     indications sur les variables internes a qnbd et zqnbd
c     izig  sert a la memorisation des contraintes (actif si izag>1)
c     si i ne change pas d ens on enleve 1 a izig  (positif)
c     sinon on ajoute izag
c     factorisation seulement si izig est nul
c     dh estimation hessien dim n(n+1)/2 rangee en troismorceaux es
c     indic(i) nouvel indice de l indice i
c     indic vect dim n ordre de rangement des indices                       es
c     pas necessaire de l initialiser si indqn=1
c
c     parametres de la recherche lineaire
c     amd,amf param. du test de wolfe .    (.7,.1)
c     napm nombre max d appels dans la rl  (=15)
c
      implicit double precision (a-h,o-z)
      real rzs(*)
      double precision dzs(*)
      character bufstr*(4096)
      dimension binf(n),bsup(n),x(n),g(n),epsx(n)
      dimension trav(ntrav),itrav(nitrav),izs(*)
      external simul
c
c---- initial printing
      if(imp.ge.1) then
         call basout(io_out, io, '')
         write(bufstr,1010)
         call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
         write(bufstr,750) n,epsg,imp
         call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
         write(bufstr,751) itmax
         call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
         write(bufstr,752) napmax
         call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
         call basout(io_out ,io ,
     $    '------------------------------------------------')
1010    format(' *********** qnbd (with bound cstr) ****************')
750     format('dimension=',i10,', epsq=',e24.16,
     $ ', verbosity level: imp=',i10)
751     format('max number of iterations allowed: iter=',i10)
752     format('max number of calls to costf allowed: nap=',i10)
      endif
c
c
c     parametres caracteristiques de l algorithme
c     si les parametres sont nuls l algorithme est celui du rr 242
c     ig=1 test sur grad(i) pour relach var
c     in=1 limite le nombre de factorisations par iter a n/10
c     irel=1 test sur decroissance grad pour rel a iter courante
c     epsrel taux de decroissance permettant le relachement (cf irit)
c     iact blocage variables dans ib (gestion contraintes actives)
c     ieps1 =1 eps1 egal a zero
c     note eps1 correspond a eps(xk)
      ig=0
      in=0
      irel=1
      epsrel=.50d+0
      izag=0
      iact=1
      ieps1=0
c
c     decoupage du vecteur trav
      n1=(n*(n+1))/2 +1
      n2=n1+n
      n3=n2+n
      n4=n3+n
      n5=n4+n-1
      if(ntrav.lt.n5) then
         if(imp.gt.0) then
           write(bufstr,110)ntrav,n5
           call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
           endif
110      format(' qnbd : ntrav=',i8,' devrait valoir ',i8)
         indqn=-11
         return
      endif
      ni1=n+1
      if(nitrav.lt.2*n) then
         ni2=2*n
         if(imp.gt.0) then
           write(bufstr,111)nitrav,ni2
           call basout(io_out ,io ,bufstr(1:lnblnk(bufstr)))
           endif
111      format(' qnbd : nitrav=',i8,'devrait valoir',i8)
         indqn=-12
         return
      endif
      call zqnbd(indqn,simul,trav(1),n,binf,bsup,x,f,g,zero,napmax,
     &itmax,itrav,itrav(ni1),nfac,imp,io,epsx,epsf,epsg,trav(n1),
     &trav(n2),trav(n3),trav(n4),df0,ig,in,irel,izag,iact,
     &epsrel,ieps1,izs,rzs,dzs)
      return
      end
