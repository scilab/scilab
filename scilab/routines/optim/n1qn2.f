      subroutine n1qn2 (simul,prosca,n,x,f,g,dxmin,df1,epsg,impres,io,
     /                  mode,niter,nsim,dz,ndz,izs,rzs,dzs)
c!But
c     Minimisation sans contrainte par un algorithme
c     de quasi-Newton a memoire limitee.
c!Origine
c     Version 1.0 de n1qn2 (Modulopt, INRIA), septembre 1988.
c     Jean Charles Gilbert, Claude Lemarechal.
c     Copyright INRIA
c!Commentaires
c     Ce code est en principe destine aux problemes de grande taille,
c     n grand, mais convient egalement pour n quelconque. La methode
c     utilisee est du type quasi-Newton (BFGS) a encombrement variable,
c     ce qui permet d'utiliser au maximum la memoire declaree dispo-
c     nible. On estine que plus la memoire utilisee est importante, plus
c     rapide sera la decroissance du critere f.
c!Sous-routines appelees
c     n1qn2:    routine-chapeau qui structure la memoire declaree dispo-
c               nible et appelle n1qn2a,
c     n1qn2a:   optimiseur proprement dit,
c     strang:   routine de calcul de la direction de descente,
c     nlis0:    routine de recherche lineaire.
c
c     De son cote, l'utilisateur doit fournir:
c     1) une routine qui appelle le module d'optimisation n1qn2,
c     2) une routine de simulation, appelee simul par n1qn2, qui
c        calcule la valeur de f et de son gradient en un point donne,
c     3) une routine, appelee prosca par n1qn2, qui realise le produit
c        scalaire de deux vecteurs, ce produit scalaire doit etre
c        celui utilise pour calculer le gradient de f dans simul.
c!Liste d'appel
c     subroutine n1qn2 (simul,prosca,n,x,f,g,dxmin,df1,epsg,impres,io,
c    /                  mode,niter,nsim,dz,ndz,izs,rzs,dzs)
c
c     Dans la description des arguments qui suit, (e) signifie que
c     l'argument doit etre initialise avant l'appel de n1qn2, (s)
c     signifie que l'argument est une variable n'ayant de signification
c     qu'en sortie et (es) = (e)+(s). Les arguments du type (s) et (es)
c     sont en general modifies par n1qn2 et ne peuvent donc pas etre
c     des constantes.
c
c     simul:     Nom d'appel de la sous-routine de simulation qui
c                qui calcule la valeur de f et de son gradient g
c                a l'itere courant. Ce module doit se presenter comme
c                suit:
c                   subroutine simul (indic,n,x,f,g,izs,rzs,dzs).
c                Le nom de la sous-routine doit etre declare external
c                dans le module appelant n1qn2. Les arguments n, x, f,
c                g, izs, rzs et dzs ont la meme signification que ci-
c                dessous. N1qn2 appelle simul soit avec indic=1, dans
c                ce cas le simulateur fera ce qu'il veut mais ne chan-
c                gera pas la valeur des arguments, ou avec indic=4, dans
c                cas le simulateur calculera a la fois f et g.
c     prosca:    Nom d'appel de la sous-routine effectuant le produit
c                scalaire de deux vecteurs u et v. Ce module doit se
c                presente sous la forme:
c                   subroutine prosca (n,u,v,ps,izs,rzs,dzs).
c                Le nom de la sous-routine doit etre declare external
c                dans le module appelant n1qn2. Les argument n, izs,
c                rzs et dzs ont la meme signification que ci-dessous.
c                Les arguments u, v et ps sont des vecteurs de dimension
c                n du type double precision. Ps donne le produit
c                scalaire de u et v.
c     n(e):      Scalaire du type integer. Donne la dimension n de la
c                variable x.
c     x(es):     Vecteur de dimension n du type double precision. En
c                entree il faut fournir la valeur du point initial, en
c                sortie, c'est le point final calcule par n1qn2.
c     f(es):     Scalaire du type double precision. En entree, c'est la
c                valeur de f en x (initial), valeur que l'on obtiendra
c                en appelant le simulateur simul avant d'appeler n1qn2.
c                En sortie, c'est la valeur de f en x (final).
c     g(es):     Vecteur de dimension n du type double precision.
c                En entree, il faut fournir la valeur du gradient de f
c                en x (initial), valeur que l'on obtiendra en appelant
c                le simulateur simul avant d'appeler n1qn2. En sortie en
c                mode 1, c'est la valeur du gradient de f en x (final).
c     dxmin(e):  Scalaire du type double precision, strictement positif.
c                Cet argument definit la resolution sur x en norme
c                l-infini: deux points dont la distance en norme l-
c                infini est superieure a dxmin seront consideres comme
c                non distinguables par la routine de recherche lineaire.
c     df1(e):    Scalaire du type double precision, strictement positif.
c                Cet argument donne une estimation de la diminution
c                escomptee pour f lors de la premiere iteration.
c     epsg(es):  Scalaire du type double precision, strictement positif
c                et strictement inferieur a 1. Sa valeur en entree,
c                determine le test d'arret qui porte sur la norme
c                (prosca) du gradient. Le minimiseur considere que la
c                convergence est atteinte en x(k) et s'arrete en mode 1
c                si E(k) := |g(k)|/|g(1)| < epsg, ou g(1) et g(k) sont
c                les gradients au point d'entree et a l'iteration k,
c                respectivement. En sortie, epsg = E(k).
c     impres(e): Scalaire du type integer qui controle les sorties.
c                <0:  Rien n'est imprime et n1qn2 appelle le simulateur
c                     avec indic=1 toutes les (-impres) iterations.
c                =0:  Rien n'est imprime.
c                >=1: Impressions initiales et finales, messages
c                     d'erreurs.
c                >=3: Une ligne d'impression par iteration donnant
c                     l'ordre k de l'iteration courante menant de x(k)
c                     a x(k+1), le nombre d'appels au simulateur avant
c                     cette iteration, la valeur du critere f et sa
c                     derivee directionnelle suivant d(k).
c                >=4: Impression de nlis0.
c                >=5: Impressions supplentaires en fin d'iteration k:
c                     le test d'arret E(k+1), prosca(y(k),s(k)) qui
c                     doit etre positif, le facteur de Oren-Spedicato
c                     et l'angle de la direction de descente d(k) avec
c                     -g(k).
c     io(e):     Scalaire du type integer qui sera pris comme numero
c                de canal de sortie pour les impressions controlees
c                par impres.
c     mode(s):   Scalaire du type integer donnant le mode de sortie de
c                n1qn2.
c                <0: Impossibilite de poursuivre la recherche lineaire
c                    car le simulateur a repondu avec indic<0. Mode
c                    renvoie cette valeur de indic.
c                =0: Arret demande par le simulateur qui a repondu avec
c                    indic=0.
c                =1: Fin normale avec test sur le gradient satisfait.
c                =2: Arguments d'entree mal initialises. Il peut s'agir
c                    de n<=0, niter<=0, nsim<=0, dxmin<=0, epsg<=0,
c                    epsg>1 ou de nrz<5n+1 (pas assez de memoire
c                    allouee).
c                =3: La recherche lineaire a ete bloquee sur tmax=10^20
c                    (mode tres peu probable).
c                =4: Nombre maximal d'iterations atteint.
c                =5: Nombre maximal de simulations atteint.
c                =6: Arret sur dxmin lors de la recherche lineaire. Ce
c                    mode de sortie peut avoir des origines tres
c                    diverses. Si le nombre d'essais de pas lors de la
c                    derniere recherche lineaire est faible, cela peut
c                    signifier que dxmin a ete pris trop grand. Il peut
c                    aussi s'agir d'erreurs ou d'imprecision dans le
c                    calcul du gradient. Dans ce cas, la direction de
c                    recherche d(k) peut ne plus etre une direction de
c                    descente de f en x(k), etant donne que n1qn2
c                    s'autorise des directions d(k) pouvant faire avec
c                    -g(k) un angle proche de 90 degres. On veillera
c                    donc a calculer le gradient avec precision.
c                =7: Soit (g,d) soit (y,s) ne sont pas positifs (mode
c                    de sortie tres improbable).
c     niter(es): Scalaire du type integer, strictement positif. En
c                entree, c'est le nombre maximal d'iterations admis.
c                En sortie, c'est le nombre d'iterations effectuees.
c     nsim(es):  Scalaire du type integer, strictement positif. En
c                entree, c'est le nombre maximal de simulations admis.
c                En sortie, c'est le nombre de simulations effectuees.
c     rz(s):     Vecteur de dimension nrz du type double precision.
c                C'est l'adresse d'une zone de travail pour n1qn2.
c     nrz(e):    Scalaire du type integer, strictement positif, donnant
c                la dimension de la zone de travail rz. En plus des
c                vecteurs x et g donnes en arguments, n1qn2 a besoin
c                d'une zone de travail composee d'au moins trois
c                vecteurs de dimension n et chaque mise a jour demandee
c                necessite 1 scalaire et 2 vecteurs de dimension n
c                supplementaires. Donc si m est le nombre de mises a
c                jour desire pour la construction de la metrique
c                locale, il faudra prendre
c                   nrz >= 3*n + m*(2*n+1).
c                En fait, le nombre m est determine par n1qn2 qui prend:
c                   m = partie entiere par defaut de ((nrz-3*n)/(2*n+1))
c                Ce nombre doit etre >= 1. Il faut donc nrz >= 5*n +1,
c                sinon n1qn2 s'arrete en mode 2.
c     izs, rzs, dzs:  Adresses de zones-memoire respectivement du type
c                integer, real et double precision. Elles sont reservees
c                a l'utilisateur. N1qn2 ne les utilise pas et les
c                transmet a simul et prosca.
c!
c-----------------------------------------------------------------------
c
c     arguments
c
      integer n,impres,io,mode,niter,nsim,ndz,izs(*)
      real rzs(*)
      double precision x(*),f,g(*),dxmin,df1,epsg,dz(*)
      double precision dzs(*)
      external simul,prosca
c
c     variables locales
c
      integer m,ndzu,l1memo,id,igg,iaux,ialpha,iybar,isbar
      double precision r1,r2
      double precision ps
c
c---- impressions initiales et controle des arguments
c
      if (impres.ge.1)
     /    write (io,900) n,dxmin,df1,epsg,niter,nsim,impres
900   format (/,' n1qn2: point d''entree',/,
     /    5x,'dimension du probleme (n)              :',i6,/,
     /    5x,'precision absolue en x (dxmin)         :',d9.2,/,
     /    5x,'decroissance attendue pour f (df1)     :',d9.2,/,
     /    5x,'precision relative en g (epsg)         :',d9.2,/,
     /    5x,'nombre maximal d''iterations (niter)    :',i6,/,
     /    5x,'nombre maximal d''appels a simul (nsim) :',i6,/,
     /    5x,'niveau d''impression (impres)           :',i4)
      if (n.le.0.or.niter.le.0.or.nsim.le.0.or.dxmin.le.0.0d+0
     /    .or.epsg.le.0.0d+0.or.epsg.gt.1.0d+0) then
          mode=2
          if (impres.ge.1) write (io,901)
901       format (/,' >>> n1qn2 : appel incoherent')
          goto 904
      endif
      if (ndz.lt.5*n+1) then
          mode=2
          if (impres.ge.1) write (io,902)
902       format (/,' >>> n1qn2: memoire allouee insuffisante')
          goto 904
      endif
c
c---- calcul de m et des pointeurs subdivisant la zone de travail dz
c
      ndzu=ndz-3*n
      l1memo=2*n+1
      m=ndzu/l1memo
      ndzu=m*l1memo+3*n
      if (impres.ge.1) write (io,903) ndz,ndzu,m
903   format (/5x,'memoire allouee (ndz)  :',i7,/,
     /         5x,'memoire utilisee       :',i7,/,
     /         5x,'nombre de mises a jour :',i6,/)
      id=1
      igg=id+n
      iaux=igg+n
      ialpha=iaux+n
      iybar=ialpha+m
      isbar=iybar+n*m
c
c---- appel du code d'optimisation
c
      call n1qn2a (simul,prosca,n,x,f,g,dxmin,df1,epsg,
     /             impres,io,mode,niter,nsim,m,
     /             dz(id),dz(igg),dz(iaux),
     /             dz(ialpha),dz(iybar),dz(isbar),izs,rzs,dzs)
c
c---- impressions finales
c
904   continue
      if (impres.ge.1) write (io,905) mode,niter,nsim,epsg
905   format (/,1x,79('-'),/,
     /        /,1x,'n1qn2 : sortie en mode ',i2,
     /        /,5x,'nombre d''iterations              : ',i4,
     /        /,5x,'nombre d''appels a simul          : ',i6,
     /        /,5x,'precision relative atteinte sur g: ',d9.2)
      call prosca (n,x,x,ps,izs,rzs,dzs)
      r1=sqrt(ps)
      call prosca (n,g,g,ps,izs,rzs,dzs)
      r2=sqrt(ps)
      if (impres.ge.1) write (io,906) r1,f,r2
906   format (5x,'norme de x = ',d15.8,
     /      /,5x,'f          = ',d15.8,
     /      /,5x,'norme de g = ',d15.8)
c
      return
      end
