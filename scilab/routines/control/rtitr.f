C/MEMBR ADD NAME=RTITR,SSI=0
c     Copyright INRIA
      subroutine rtitr(nin,nout,nu,num,inum,dgnum,den,iden,dgden,
     &                 up,u,iu,yp,y,iy,job,iw,w,ierr)
c!but
c     le sous programme rtitr calcule la reponse temporelle d'un systeme
c     dynamique lineaire discret MIMO  represente par sa forme de
c     transfert: D**-1*N  soumis a une entree U
c!liste d'appel
c     subroutine rtitr(nin,nout,nu,num,inum,dgnum,den,iden,dgden,
c   &                 up,u,iu,yp,y,iy,job,iw,w,ierr)
c
c     integer nin,nout,nu,inum,dgnum,iden,dgden,iu,iy,job,ierr,iw(nout)
c     double precision num(inum,nin*(dgnum+1)),den(iden,nout*(dgden+1))
c     double precision up(iu,dgden+1),u(iu,nu),yp(iy,dgden+1)
c     double precision y(iy,nu+dgden-dgnum),w(nout)
c
c     nin    : nombre d'entrees du systeme dynamique, nombre de colonnes
c              de la matrice N.
c     nout   : nombre de sorties du systeme dynamique, nombre de lignes
c              de la matrice N et dimensions de D.
c     nu     : nombre d'echantillon de la reponse temporelle a calculer
c     num    : tableau contenant les coefficients (matriciels) du polynome
c              matriciel numerateur N. Si N=somme(Nk*z**k) alors num
c              est la matrice bloc : num=[N ,N ,....N       ]
c                                          0  1      dgnum+1
c              num est modifie par l'execution ( normalisation par l
c              coefficient de plus haut degre de D D(dgden+1) )
c     inum   : nombre de ligne du tableau num dans le programme appelant
c     dgnum  : degre du polynome matriciel numerateur
c     den    : tableau contenant les coefficients (matriciels) du polynome
c              matriciel denominateur D. Si D=somme(Dk*z**k) alors den
c              est la matrice bloc : den=[D ,D ,....D       ]
c                                          0  1      dgden+1
c              den est modifie par l'execution (normalisation par la
c              matrice de plus haut degre D(dgden+1) )
c     iden   : nombre de ligne du tableau den dans le programme appelant
c     dgden  : degre du polynome matriciel denominateur
c     up     : tableau contenant eventuellement (voir job) les dgden+1
c              entrees  passees du systeme stockees par colonnes:
c              up=[U      , ....,U  ] . Si omis up est pris nul.
c                   -dgden        -1
c     u      : tableau contenant les nu echantillons d'entrees soumis
c              au systeme . u=[U , .... , U    ]
c                               0          nu-1
c     iu     : nombre de lignes des tableaux up et u dans la programme
c              appelant
c     yp     : tableau contenant eventuellement (voir job) les dgden+1
c              sorties  passees du systeme stockees par colonnes:
c              yp=[Y      , .... , Y    ] . Si omis yp est pris nul.
c                   -dgden          -1
c     y      : tableau contenant apres execution les nt echantillons
c              de sorties du systeme . y=[Y ,....,Y                ]
c                                          0       nu+dgden-dgnum-1
c     iy     : nombre de lignes des tableaux yp et y dans la programme
c              appelant
c     job    : Si job = +-1 le programme suppose que les valeurs passees
c                           de U et Y sont nulles up et yp ne sont alors
c                           pas references
c              Si job = +-2 les valeurs passees de U et Y sont donnees
c                           par up et yp
c              job > 0 le sous programme effectue la normalisation
c              job < 0 on suppose que la normalisation a deja ete effectuee
c                      (rappel de rtitr pour le meme systeme)
c     iw ,w  : tableaux de travail. En retour w(1) contient le
c              conditionnement evalue par dgeco.
c     ierr   : indicateur d'erreur:
c              0 --> ok
c              1 --> la matrice coefficient de plus haut degre de D est
c                    mal conditionnee le conditionnement est estime par
c                    dgeco et le sous programme teste s'il est
c                    negligeable par rapport a 1. Dans ce cas le calcul
c                    est effectue
c              2 --> la matrice coefficient de plus haut degre de D n'est
c                    pas inversible. Calcul abandonne.
c             -1 --> argument d'appel incorrect (dimensionnement des
c                    tableaux negatif ou nul ou degre de N et D negatif)
c!sous programmes appeles
c     dgeco,dgesl (linpack)
c     ddif,ddad (blas)
c     dmmul (blas etendu)
c!methode
c
c     +inf                +inf           dn            dd
c     ---                 ---            ---           ---
c     \     -k            \     -k       \     i       \     j
c si U=> U z     ,    Y=   > Y z   ,  N=  > N z  ,  D=  > D z
c     /   k               /   k          /   i         /   j
c     ---                 ---            ---           ---
c     -inf                -inf            0             0
c
c la sortie Y verifie l'equation polynomiale D*Y=N*U qui peut s'ecrire:
c
c              dd-1          dn
c              ---           ---
c              \             \
c   D  Y    = - > D Y     +   > N U              -inf < i < +inf
c    dd i+dd   /   k i+k     /   l i+l
c              ---           ---
c               0             0
c
c Si  D  est inversible l'equation precedente donne directement la
c      dd
c recursion permettant de calculer Y    connaissant les dd echantillons
c                                   i+dd
c precedents de Y et U
c
c!origine
c     Serge Steer INRIA 1988
c!
c
      integer nin,nout,nu,inum,dgnum,iden,dgden,iu,iy,ierr,iw(nout)
      double precision num(inum,*),den(iden,*)
      double precision up(iu,*),u(iu,nu),yp(iy,*),y(iy,*),w(nout)
c
      double precision rcond,dmx,ddot
c
      ierr=0
      nt=nu+dgden-dgnum
      if(nin.le.0.or.nout.le.0.or.nt.le.0.or.inum.le.0.or.iden.le.0
     &   .or.iu.le.0.or.iy.le.0.or.dgden.lt.0.or.dgnum.lt.0) then
      ierr=-1
      return
      endif
c
      if(nout.eq.1) goto 40
c     initialisation de la reponse
      do 01 k=1,nout
 01   call dset(nt,0.0d+0,y(k,1),iy)
      if(job.gt.0) then
c
c     normalisation
c
c     factorisation du coeff de plus haut degre en z**-1 de d
      kd=1+dgden*nout
      call dgeco(den(1,kd),iden,nout,iw,rcond,w)
      if (rcond .eq. 0.0d+0) then
       ierr=2
       w(1)=0.0d+0
       return
      endif
      if (1.0d+0+rcond.le.1.0d+0 ) ierr=1
c     normalisation de N et D
      if(dgden.gt.0) then
      do 10 k=1,nout*dgden
      call dgesl (den(1,kd),iden,nout,iw,den(1,k),0)
 10   continue
      endif
      do 11 k=1,nin*(dgnum+1)
      call dgesl (den(1,kd),iden,nout,iw,num(1,k),0)
 11   continue
      endif
c
c     recursion
c
      do 30 n=0,nt-1
      if(dgden-n.lt.1.or.abs(job).eq.1) goto 25
c     termes faisant intervenir les valeurs passees
      kd=1
      do 20 k=1,dgden-n
      call dmmul(den(1,kd),iden,yp(1,n+k),iy,w,nout,nout,nout,1)
      call ddif(nout,w,1,y(1,1+n),1)
      kd=kd+nout
 20   continue
      ln=1
      do 21 l=1,min(dgden-n,dgnum+1)
      call dmmul(num(1,ln),inum,up(1,n+l),iu,w,nout,nout,nin,1)
      call dadd(nout,w,1,y(1,1+n),1)
      ln=ln+nin
 21   continue
 22   continue
c
 25   continue
c     autres termes
      mx=max(1,dgden-n+1)
      if(mx.gt.dgden) goto 27
      kd=1+(mx-1)*nout
      do 26 k=mx,dgden
      call dmmul(den(1,kd),iden,y(1,n+k-dgden),iy,w,nout,nout,nout,1)
      call ddif(nout,w,1,y(1,1+n),1)
      kd=kd+nout
 26   continue
 27   if(mx.gt.dgnum+1) goto 30
      ln=1+(mx-1)*nin
      do 28 l=mx,dgnum+1
      call dmmul(num(1,ln),inum,u(1,n+l-dgden),iu,w,nout,nout,nin,1)
      call dadd(nout,w,1,y(1,1+n),1)
      ln=ln+nin
 28   continue
 30   continue
      w(1)=rcond
      return
c
 40   continue
c     cas particulier d'un systeme mono-sortie. Evaluation plus directe
c
c     initialisation de la reponse
      call dset(nt,0.0d+0,y,iy)
      if(job.gt.0) then
      dmx=den(1,dgden+1)
      if( dmx.eq.0) then
        ierr=2
        w(1)=0.0d+0
        return
      endif
      dmx=1.0d+0/dmx
      call dscal(dgden+1,dmx,den,iden)
      call dscal(nin*(dgnum+1),dmx,num,inum)
      endif
c     recursion
      do 50 n=0,nt-1
      if(dgden-n.lt.1.or.abs(job).eq.1) goto 42
c     termes faisant intervenir les valeurs passees
      y(1,1+n)=-ddot(dgden-n,den,iden,yp(1,n+1),iy)
      do 41 l=1,nin
      y(1,1+n)=y(1,1+n)+ddot(min(dgden-n,dgnum+1),num(1,l),inum*nin,
     &                       up(l,n+1),iu)
 41   continue
 42   continue
c     autres termes
      mx=max(1,dgden-n+1)
      if(mx.gt.dgden) goto 43
      y(1,1+n)=y(1,1+n)-ddot(dgden-mx+1,den(1,mx),iden,
     &                       y(1,n+mx-dgden),iy)
 43   if(mx.gt.dgnum+1) goto 50
      ln=(mx-1)*nin
      do 44 l=1,nin
      y(1,1+n)=y(1,1+n)+ddot(dgnum+2-mx,num(1,ln+l),inum*nin,
     &                       u(l,n+mx-dgden),iu)
 44   continue
 50   continue
      w(1)=1.0d+0
      return
c
      end
