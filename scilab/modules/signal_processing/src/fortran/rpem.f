C/MEMBR ADD NAME=RPEM,SSI=0
      subroutine rpem(theta,p,n,u,y,lambda,k,c,istab2,v,eps,
     &  eps1,idim,fi,psi,tstab,work,f,g,l)
c!but
c        -1           -1           -1
c     a(q  )y(t) = b(q  )u(t) + c(q  )e(t)
c
c!parametres
c
c     ***description des parametres***
c
c     theta - vecteur d'ordre (3*n) qui contienne les parametres
c             de estimation
c             theta=(a(1) ...a(n),b(1) ...b(n), c(1) ...c(n)
c             theta est changee dans la subroutine
c     p     - matrice symetrique d'ordre (3*n)
c             p est employee dans la forme u-d
c             p=u*d*u(transposee)
c             avec d diagonale et u triangulaire superieure
c             les elements de d sont conserves dans la diagonale de d
c             les elements de u sont gardes dans la partie diagonale
c             superieure de p. p est changee dans la subroutine.
c     n     - modele d'ordre (min 1, max 10)
c     u     - derniere valeur d'entree
c     y     - derniere valeur de sortie
c     lambda- facteur d'oubli (a fournir)
c     k     - facteur de contraction employe pour le filtrage
c             des donnees (a fournir)
c             commentaire:
c             pour des resultats raisonnables
c             0.lt.lambda.le.1
c             lambda proche de 1 apres plusieurs appels a rpem
c             0.lt.k.lt.1
c             k proche de 1 apres plusieurs appels a rpem
c     c     - parametre employe pour la regularisation
c             c doit etre choisi plutot grand
c     istab1- flag (a fournir) pour les tests d'estabilite de c(z).
c             si istab1=0 on n'execute pas des controles (test
c             d'estabilite et reductions de pas)
c             si istab1.ne.0 on execute des controles (test
c             d'estabilite et eventuelles reductions de pas)
c     istab2- entier donne en sortie qu'indique le nombre des
c             reductions de pas executees. si istab1=0,
c             alors la valeur de istab2 n'est pas significative
c     v     - fonction de perte- addition des carres des erreurs
c             predis. une modification due a des incertitudes dans
c             la phase transiente est inclue
c             v est changee par la subroutine
c     eps   - erreur de prediction (retourne)
c     eps1  - residu(retourne)
c     init  - flag employe pour commencer la prediction
c             si init=0 tous les parametres sont mis a jour
c             si init.ne.0 des valeurs initiaux adequats sont
c             mis en premier et apres les parametres sont mis
c             en employant les donnees disponibles u et y
c     po    - parametre scalaire employe pour donner a p une
c             valeur initiale (a etre fourni quand init.ne.0)
c             if init.ne.0 p=po*matrice unitaire
c     idim  - parametre de dimension
c
c!subroutines necessaires
c     nstabl
c!
      double precision theta(*),p(idim,*),u,y,c,eps,eps1,po
      double precision fi(*),psi(*),tstab(*),work(*),f(*),g(*)
      double precision lambda,k,l(*)
      double precision amy,y1,u1,e1,ci,alfa,gamma,beta,dd,al,s,v
c
      nn=n*3
c     ========================================
c     test pour initialisation
c     ========================================
      init=0
      istab1=1
      po=1
      if (init.eq.0) goto 100
c
      v=0.0d+0
      do 10 i=1,nn
      do 10 j=1,nn
10    p(i,j)=0.0d+0
      do 20 i=1,nn
      p(i,i)=po
      theta(i)=0.0d+0
      l(i)=0.0d+0
      fi(i)=0.0d+0
20    psi(i)=0.0d+0
c     ========================================
c     calcul de l'erreur de prediction
c     ========================================
100   eps=y
      do 110 i=1,nn
110   eps=eps-fi(i)*theta(i)
c     ========================================
c     calcul des nouvelles estimations de parametres
c     ========================================
      amy=1.0d+0
c     ========================================
c     test pour determiner si on fait des controles
c     ========================================
      if(istab1.eq.0) goto 200
      istab2=0
120   do 130 i=1,n
      ni=2*n+i
130   tstab(i+1)=theta(ni)+l(ni)*eps*amy
      tstab(1)=1.0d+0
c     ========================================
c     test pour stabilite de c(z)
c     ========================================
      call nstabl(tstab,n,work,ist)
      if (ist.eq.0) goto 200
      amy=amy/2.0d+0
      if(1.0d+0+amy.le.1.0d+0) goto 200
      istab2=istab2+1
      goto 120
c     ========================================
c     mise a jour de parametres d'estimation
c     ========================================
200   do 210 i=1,nn
210   theta(i)=theta(i)+l(i)*eps*amy
c     ========================================
c     calcul residus
c     ========================================
      eps1=y
      do 220 i=1,nn
220   eps1=eps1-fi(i)*theta(i)
c     ========================================
c     calcul des signaux filtrees y1, u1, e1
c     ========================================
      y1=y
      u1=u
      e1=eps1
c
      do 620 i=1,n
      ci=theta(2*n+i)*k**i
      y1=y1+ci*psi(i)
      u1=u1-ci*psi(n+i)
620   e1=e1-ci*psi(n*2+i)
c     ========================================
c     mise a jour des vecteurs fi et psi
c     ========================================
      if(n.eq.1) goto 720
      do 700 j=2,n
      i=n+2-j
      fi(i)=fi(i-1)
      psi(i)=psi(i-1)
      i=2*n+2-j
      fi(i)=fi(i-1)
      psi(i)=psi(i-1)
      i=3*n+2-j
      fi(i)=fi(i-1)
700   psi(i)=psi(i-1)
720   fi(1)=-y
      psi(1)=-y1
      fi(n+1)=u
      psi(n+1)=u1
      fi(2*n+1)=eps1
      psi(2*n+1)=e1
c     ========================================
c     calcul du vecteur de gain l, mise a jour de p et v
c     ========================================
      do 810 i=2,nn
      j=nn+2-i
      alfa=psi(j)
      j1=j-1
      do 800 kk=1,j1
800   alfa=alfa+p(kk,j)*psi(kk)
      f(j)=alfa
810   g(j)=p(j,j)*alfa
      g(1)=p(1,1)*psi(1)
      f(1)=psi(1)
c
      alfa=lambda+f(1)*g(1)
      gamma=0.0d+0
      if (alfa.gt.0.0d+0) gamma=1.0d+0/alfa
      if(g(1).ne.0.0d+0) p(1,1)=gamma*p(1,1)
c
      do 830 j=2,nn
      beta=alfa
      dd=g(j)
      alfa=alfa+dd*f(j)
      if(alfa.eq.0.0d+0) goto 830
      al=-f(j)*gamma
c
      j1=j-1
      do 820 i=1,j1
      s=p(i,j)
      p(i,j)=s+al*g(i)
820   g(i)=g(i)+dd*s
      gamma=1.0d+0/alfa
      p(j,j)=beta*gamma*p(j,j)/lambda
      p(j,j)=min(p(j,j),c)
830   continue
c
      v=v+eps**2/alfa
      do 840 i=1,nn
840   l(i)=g(i)/alfa
c     ========================================
c     fin des calculs
c     ========================================
      return
      end
