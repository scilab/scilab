      subroutine isoval(a,lda,m,n,c,path,npath,maxp,ierr,iw,job)
c!    but
c     Etant donnee une matrice A telle que A(l,k)=f(X(l),Y(k)) ou
c     f est une fonction de R X R dans R, ce sous programme recherche
c     les lignes d'isovaleurs (relatives a la valeur c) de la 
c     tabulation reguliere ( X(l+1)-X(l)=DX,Y(k+1)-Y(k)=DY )
c     de f donnee par A
c!    liste d'appel
c     subroutine isoval(a,lda,m,n,c,path,npath,maxp,ierr,iw,job)
c     
c     double precision a(lda,n),c,path(2,maxp)
c     integer iw(m*n-n)
c     integer lda,m,n,maxp,ierr,job
c     
c     a     : tableau contenant la tabulation de f
c     lda   : nombre de lignes du tableau a
c     m     : nombre de lignes effectif de a
c     n     : nombre de colonnes de a
c     c     : valeur pour la quelle on cherche les isovaleurs
c     path  : contient en sortie la description des isovaleurs:
c     path=[path ,...., path     ]
c     1           npath 
c     ou pathi a la structure suivante :
c     [np x1,...xnp]
c     [0  y1,...ynp]
c     si :
c     np est la longueur de l'isovaleur
c     xj,yj les coordonnees interpolees des points de l'isovaleur
c     npath : le nombre de courbes disjointes d'isovaleurs
c     maxp  : la dimension maximale admise pour le tableau path
c     ierr  : indicateur d'erreur
c     0 : ok
c     1 : nombre de points decrivant les isovaleur > maxp
c     iw    : tableau de travail 
c     job   : flag permettant d'indiquer au programme si la fonction f
c     est definie sur l'ensemble des points de la matrice A
c     job=0 : f definie partout
c     job=1 : f n'est pas definie au points A(i,j) si 
c             iw(i+(j-1)m)=0
c!    origine
c     programme par S Steer a partir de la macro scilab  de Carey Bunks 
c     1990
c
c     Corrige par C Bunks pour isovaleurs qui sont exactement egales
c     a des entrees de la matrice 6 mars 1991.
c!    
c     Copyright INRIA
      double precision a(*),c,path(2,*)
      integer iw(*)
      integer lda,m,n,maxp,ierr,job
c     
      logical pend
      integer north,south,east,west,dir
      data north/0/,south/1/,east/2/,west/3/
c     
      ierr=0
c     
      kv=1
      kh=kv+m*n
      kw=kh+m*(n-1)
c
c     perturb values which are exactly equal to the level value
      do 5 ip=1,n*m
         if(a(ip).eq.c) a(ip)=a(ip)+1d-14
 5    continue

c     
c     make horizontal and vertical edge matrices for level value
      if(job.eq.0) then
         ih=kh-1
         ia=-lda
         do 11 k=1,n-1
            ia=ia+lda
            do 10 l=1,m
               ih=ih+1
               iw(ih)=1
               if((a(ia+lda+l)-c)*(a(ia+l)-c).lt.0.0d0) iw(ih)=-1
 10         continue
 11      continue
c     
         iv=kv-1
         ia=-lda
         do 13 k=1,n
            ia=ia+lda
            do 12 l=1,m-1
               iv=iv+1
               iw(iv)=1
               if((a(ia+l+1)-c)*(a(ia+l)-c).lt.0.0d0) iw(iv)=-1
 12         continue
 13      continue
      else
c
         kj=kv
c
         ih=kh-1
         ia=-lda
         ij=kj-1
         do 21 k=1,n-1
            ia=ia+lda
            do 20 l=1,m
               ij=ij+1
               ih=ih+1
               iw(ih)=1
               if(iw(ij)*iw(ij+m).eq.0) goto 20
               if((a(ia+lda+l)-c)*(a(ia+l)-c).lt.0.0d0) iw(ih)=-1
 20         continue
 21      continue
c     
         iv=kv-1
         ia=-lda
         ij=kj-1
         do 23 k=1,n
            ia=ia+lda
            do 22 l=1,m-1
               iv=iv+1
               ij=ij+1
               iw(iv)=1
               if(iw(ij)*iw(ij+1).eq.0) goto 22
               if((a(ia+l+1)-c)*(a(ia+l)-c).lt.0.0d0) iw(iv)=-1
 22         continue
               ij=ij+1
 23      continue
      endif
c     
      npath=0
      kpath0=1
      kpath=0
c     
c     search pathes (starting with boundaries)
c     
      
c     horizontal boundaries
c     northern border
      ih=kh-m
      ia=1-lda
      
      do 31 ic=1,n-1
         ih=ih+m
         ia=ia+lda
         if(iw(ih).lt.0) then
            kpath=1
            path(2,kpath0+1)=1.0d0
            path(1,kpath0+1)=ic+(c-a(ia))/(a(ia+lda)-a(ia))
            i=1
            j=ic
            dir=north
            pend=.false.
 30         call isova0(a,lda,m,n,path(1,kpath0+1),kpath,i,j,dir,pend,
     $           iw(kh),iw(kv),c)
            if(kpath0+kpath.ge.maxp) goto 999
            if(.not.pend) goto 30
            if(kpath.gt.1) then
               path(1,kpath0)=kpath
               path(2,kpath0)=0
               kpath0=kpath0+1+kpath
               npath=npath+1
            endif
            kpath=0
         endif
 31   continue
c     
c     southern border
c     
      ih=kh+(m-1)+(n-1)*m
      ia=1+(m-1)+(n-1)*lda
      do 41 ic=n-1,1,-1
         ih=ih-m
         ia=ia-lda
         if(iw(ih).lt.0) then
            kpath=1
            path(2,kpath0+1)=m
            path(1,kpath0+1)=ic+(c-a(ia))/(a(ia+lda)-a(ia))
            i=m-1
            j=ic
            dir=south
            pend=.false.
 40         call isova0(a,lda,m,n,path(1,kpath0+1),kpath,i,j,dir,pend,
     $           iw(kh),iw(kv),c)
            if(kpath0+kpath.ge.maxp) goto 999
            if(.not.pend) goto 40
            if(kpath.gt.1) then
               path(1,kpath0)=kpath
               path(2,kpath0)=0
               kpath0=kpath0+kpath+1
               npath=npath+1
            endif
            kpath=0
         endif
 41   continue
c     
c     vertical boundaries
c     
c     eastern border
      
      iv=kv-1+(n-1)*(m-1)
      ia=(n-1)*lda
      do 51 ir=1,m-1
         iv=iv+1
         ia=ia+1
         if(iw(iv).lt.0) then
            kpath=1
            path(2,kpath0+1)=ir+(c-a(ia))/(a(ia+1)-a(ia))
            path(1,kpath0+1)=n
            i=ir
            j=n-1
            dir=east
            pend=.false.
 50         call isova0(a,lda,m,n,path(1,kpath0+1),kpath,i,j,dir,pend,
     $           iw(kh),iw(kv),c)
            if(kpath0+kpath.ge.maxp) goto 999
            if(.not.pend) goto 50
            if(kpath.gt.1) then
               path(1,kpath0)=kpath
               path(2,kpath0)=0
               kpath0=kpath0+1+kpath
               npath=npath+1
            endif
            kpath=0
         endif
 51   continue
c     
c     western border
c     
      iv=kv+m-1
      ia=m
      do 61 ir=m-1,1,-1
         iv=iv-1
         ia=ia-1
         if(iw(iv).lt.0) then
            kpath=1
            path(2,kpath0+1)=ir+(c-a(ia))/(a(ia+1)-a(ia))
            path(1,kpath0+1)=1.0d0
            i=ir
            j=1
            dir=west
            pend=.false.
 60         call isova0(a,lda,m,n,path(1,kpath0+1),kpath,i,j,dir,pend,
     $           iw(kh),iw(kv),c)
            if(kpath0+kpath.ge.maxp) goto 999
            if(.not.pend) goto 60
            if(kpath.gt.1) then
               path(1,kpath0)=kpath
               path(2,kpath0)=0
               kpath0=kpath0+kpath+1
               npath=npath+1
            endif
            kpath=0
         endif
 61   continue
c     
c     all the rest
c     
      ih=kh-1
      ia1=1-lda
      do 72 ic=1,n-1
         ia1=ia1+lda
         ia=ia1
         ih=ih+1
         do 71 ir=2,m-1
            ih=ih+1
            ia=ia+1
            if(iw(ih).lt.0) then
               kpath=1
               path(2,kpath0+1)=ir
               path(1,kpath0+1)=ic+(c-a(ia))/(a(ia+lda)-a(ia))
               i=ir
               j=ic
               dir=north
               pend=.false.
 70            call isova0(a,lda,m,n,path(1,kpath0+1),kpath,i,j,dir,
     $              pend,iw(kh),iw(kv),c)
               if(kpath0+kpath.ge.maxp) goto 999
               if(.not.pend) goto 70
               if(kpath.gt.1) then
                  path(1,kpath0)=kpath
                  path(2,kpath0)=0
                  kpath0=kpath0+kpath+1
                  npath=npath+1
               endif
               kpath=0
            endif
 71      continue
         ih=ih+1
 72   continue
      return
 999  ierr=1
      end

 
