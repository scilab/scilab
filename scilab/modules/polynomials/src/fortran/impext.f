      subroutine impext(mp,d,m,n,row,nr,col,nc,mp1,d1,job,ierr)
c!but
c     impext extrait une sous matrice definie par un choix de lignes
c     et un choix de colonnes,d'une matrice polynomiale
c     a coefficients entiers ,donnee.
c!liste d'appel
c     subroutine impext(mp,d,m,n,row,nr,col,nc,mp1,d1,job,ierr)
c     integer mp(*),mp1(*)
c     integer d(*),m,n,row(*),nr,col(*),nc,d1(*),job,ierr
c
c     mp:tableau contenant les coefficients des polynomes de la
c        matrice polynomiale donnee
c     d:tableau des pointeurs sur les premiers coeff de chaque poly
c     m: nombre de ligne de la matrice polynomiale
c     n: nombre de colonnes
c     row:vecteur contenant les indices des lignes choisies
c         si nr <=0 row n'est pas reference
c     nr:nombre de lignes choisies ,si nr < 0 on choisit toutes
c        les lignes
c     col:vecteur contenant les indices des colonnes choisies
c         si nc <=0 col n'est pas reference.
c     nc:nombre de colonnes choisies, si nc < 0 on choisit toutes
c        les colonnes
c     mp1:tableau contenant les coeff de la matrice polynomiale
c        resultat. si job=0 mp1 n'est pas referencee
c     d1:matrice des pointeurs de la matrice polynomiale resultat
c        d1 est calculee si job.ne.1,si job =1 d1 doit etre fournie
c     job:indicateur d'execution
c         job=0 seul d1 est calcule
c         job=1 mp1 est calcule supposant d1 donnee
c         sinon d1 et mp1 sont calcules
c     ierr:indicateur d'erreur:
c          ierr=0 ok
c          ierr>0 une des lignes (colonnes) choisies n'appartient
c                 pas a la matrice donnee.
c!origine
c     Serge Steer INRIA  5/02/86
c!sous programmes appeles
c     icopy (blas)
c!
c     Copyright INRIA
      integer mp(*),mp1(*)
      integer d(*),m,n,row(*),nr,col(*),nc,d1(*),job,ierr
c
      ierr=0
      if(nr*nc.eq.0) return
      if(m.le.0.or.n.le.0) return
      if(nr.lt.0) goto 40
      if(nc.lt.0) goto 50
c
c un choix de lignes et un choix de colonnes
c
c verifications de la validite des vecteurs row et col
c      do 10 j=1,nc
c      if(col(j).le.0.or.col(j).gt.n) goto 100
c   10 continue
c      do 11 i=1,nr
c      if(row(i).le.0.or.row(i).gt.m) goto 100
c   11 continue
c
      if(job.eq.1)  goto 25
c calcul de la matrice deplacement de la matrice polynomiale resultat
      d1(1)=1
      id1=1
      do 20 j=1,nc
      id=m*(col(j)-1)+1
      do 20 i=1,nr
      id1=id1+1
   20 d1(id1)=d1(id1-1)+d(id+row(i))-d(id+row(i)-1)
      if(job.eq.0) return
c
   25 id1=1
      do 26 j=1,nc
      id=m*(col(j)-1)
      do 26 i=1,nr
      id1=id1+1
      call icopy(d1(id1)-d1(id1-1),mp(d(id+row(i))),1,mp1(d1(id1-1)),1)
   26 continue
      return
c
   40 if(nc.lt.0) goto 60
c toutes les lignes et un choix de colonnes (nr<0)
c      do 41 j=1,nc
c      if(col(j).le.0.or.col(j).gt.n) goto 100
c   41 continue
      if(job.eq.1) goto 45
      id1=1
      d1(id1)=1
      do 42 j=1,nc
      id=m*(col(j)-1)+1
      do 42 i=1,m
      id1=id1+1
      d1(id1)=d1(id1-1)+d(id+i)-d(id+i-1)
   42 continue
      if(job.eq.0) return
   45 id1=1
      do 46 j=1,nc
      id=1+m*(col(j)-1)
      call icopy(d(id+m)-d(id),mp(d(id)),1,mp1(d1(id1)),1)
      id1=id1+m
   46 continue
      return
c
   50 continue
c toutes les colonnnes et un choix de lignes
c      do 51 i=1,nr
c      if(row(i).le.0.or.row(i).gt.m) goto 100
c   51 continue
      if(job.eq.1) goto 55
      id1=1
      d1(1)=1
      id=1-m
      do 52 j=1,n
      id=id+m
      do 52 i=1,nr
      id1=id1+1
      d1(id1)=d1(id1-1)+d(id+row(i))-d(id+row(i)-1)
   52 continue
      if(job.eq.0) return
   55 continue
      id1=1
      do 53 j=1,n
      id=(j-1)*m
      do 53 i=1,nr
      idi=id+row(i)
      call icopy(d(idi+1)-d(idi),mp(d(idi)),1,mp1(d1(id1)),1)
      id1=id1+1
   53 continue
      return
c
   60 continue
c toutes les lignes et toutes les colonnes
      if(job.eq.1) goto 65
      do 61 i=1,m*n+1
      d1(i)=d(i)
   61 continue
      if(job.eq.0) return
   65 call icopy(d(m*n+1)-1,mp,1,mp1,1)
      return
c  100 ierr=1
c      return
      end
