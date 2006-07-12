      subroutine recbez(p1,n1,p2,n2,best,ipb,w,err)
c!but
c calcule la factorisation de bezout de deux polynomes p1,p2 c'est a dire
c les polynomes x,y,u, v et pgcd de degre minimal et tels que :
c                      [x  u]
c            [p1 p2] * [    ] = [0 Pgcd]
c                      [y  v]
c!liste d'appel
c     subroutine recbez(p1,n1,p2,n2,best,ipb,w,err)
c     double precision  p1(n1+1),p2(n2+1),w(*),best(*),err
c
c     p1 : vecteur des coefficients du polynome p1 ranges par puissances
c          croissantes
c     n1 : degre du polynome p1
c     p2 : vecteur des coefficients du polynome p2 ranges par puissances
c          croissantes
c     n2 : degre du polynome p2
c
c     best : tableau resultat doit etre de taille au moins :
c            (2*(n1+n2)+min(n1,n2)+3)  [x y u v pgcd]
c            x=best(1:ipb(2)-1),y=best(ipb(2):ipb(3)-1)
c            u=best(ipb(3):ipb(4)-1) v=best(ipb(4):ipb(5)-1)
c            pgcd=best(ipb(5):ipb(6)-1)
c     ipb :vecteur des pointeurs sur les adresses de debut de x,y,u,v,pgcd
c          dans best ( voir ci-dessus)
c     w: tableau de travail  w(7*n0+3*n0*n0) ou n0=max(n1,n2)+1
c     err : estimee de l'erreur d'equation
c! sous programmes appeles
c     dest dcopy bezstp dlamch
c!origine
c S Steer INRIA 1989
c!
c     Copyright INRIA
      double precision  p1(n1+1),p2(n2+1),w(*),best(*),err
      double precision  dlamch
      integer ipb(6)
c
      integer nn1,nn2
c
      err=dlamch('o')
      ia=1
      nn1=n1
      nn2=n2
c
c degre reel des polynomes
c
      nn1=nn1+1
 01   nn1=nn1-1
      if(nn1.lt.0) goto 30
      if(p1(nn1+1).eq.0.0d+0) goto 01
c
      nn2=nn2+1
 02   nn2=nn2-1
      if(nn2.lt.0) goto 30
      if(p2(nn2+1).eq.0.0d+0) goto 02
c
      n0=max(nn1,nn2)+1
      n02=n0*(n0+1)
      na=n0+1
      call dset(n02,0.0d+0,w(ia),1)
      iu=ia+n02
      nu=n0+1
      call dset(2*n02,0.0d+0,w(iu),1)
      iw=iu+2*n02
      ifree=iw+7*n0
c
      la=ia+na-1
      lu=iu+nu-1+2*n0*nu
c
      call dcopy(nn1+1,p1,1,w(la-1),na)
      call dcopy(nn2+1,p2,1,w(la),na)
      call dset(2,1.0d+0,w(lu-1-2*nu),nu+1)
c
      do 20 l=1,n0
         la=la-1
         lu=lu-1-2*nu
c     la :  pointeur sur le coin en haut a gauche de la matrice a courante
c     lu : pointeur sur le coin en haut a gauche de la matrice u courante
         call bezstp(p1,nn1,p2,nn2,w(la),na,w(lu),nu,l,w(la-1+na),
     $        w(lu-1-2*nu),w(iw),best,ipb,err)
 20   continue
      return
 30   continue
      err=0.0d+0
      ipb(1)=1
      if(min(nn1,nn2).eq.0) goto 70
      CRES=nn1-nn2
      if (CRES .lt. 0) then 
         goto 40
      elseif (CRES .eq. 0) then
         goto 50
      else
         goto 60
      endif
 40   continue
c     p1=0
      np=nn2
      call dcopy(nn2+1,p2,1,best(1),1)
      ipb(2)=ipb(1)+nn2+1
      best(ipb(2))=0.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=1.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=1.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=0.0d+0
      ipb(6)=ipb(5)+1
      return
 50   continue
c     p1=0,p2=0  la decomposition n'est pas definie
      np=0
      best(1)=0.0d+0
      ipb(2)=ipb(1)+1
      best(ipb(2))=1.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=0.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=1.0d+0
      ipb(6)=ipb(5)+1
      return
 60   continue
c     p2=0
      np=nn1
      call dcopy(nn1+1,p1,1,best(1),1)
      ipb(2)=ipb(1)+nn1+1
      best(ipb(2))=1.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      best(ipb(4))=0.0d+0
      ipb(5)=ipb(4)+1
      best(ipb(5))=1.0d+0
      ipb(6)=ipb(5)+1
      return
c
 70   best(1)=1.0d+0
      ipb(2)=2
      CRES=nn1-nn2
      if (CRES .lt. 0) then 
         goto 90
      elseif (CRES .eq. 0) then 
         goto 95
      else
         goto 100
      endif
 90   continue
c     p1=cte
      best(ipb(2))=1.0d+0/p1(1)
      ipb(3)=ipb(2)+1
      best(ipb(3))=0.0d+0
      ipb(4)=ipb(3)+1
      call dcopy(nn2+1,p2,1,best(ipb(4)),1)
      ipb(5)=ipb(4)+nn2+1
      best(ipb(5))=-p1(1)
      ipb(6)=ipb(5)+1
      return
  95  continue
c     p1=cte,p2=cte
      if(abs(p1(1)).gt.abs(p2(1))) goto 90
c
 100  continue
c     p2=cte
      best(ipb(2))=0.0d+0
      ipb(3)=ipb(2)+1
      best(ipb(3))=1.0d+0/p2(1)
      ipb(4)=ipb(3)+1
      best(ipb(4))=-p2(1)
      ipb(5)=ipb(4)+1
      call dcopy(nn1+1,p1,1,best(ipb(5)),1)
      ipb(6)=ipb(5)+nn1+1
      return
c
      end
