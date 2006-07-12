      subroutine bezout(a,da,b,db,f,df,v,dv,ip)
c!but
c     etant donnes deux polynomes a et b cette subroutine calcule:
c     - le pgcd de a et b
c     - les facteurs x,y de degre mini de la decomposition de bezout:
c       a*x+b*y=pgcd
c     - les polynomes x1 et y1 de degre minimum tels que a*x1+b*y1=0
c!liste d'appel
c
c     subroutine bezout(a,da,b,db,f,df,v,dv,ip)
c
c     double precision a(da+1),b(db+1),f(da+db+2),v(2*(da+db+2))
c     integer da,db,df(2),dv(2,2),ip
c
c     a : tableau de taille da+1 contenant les coefficients du
c         polynome a ranges consecutivement et par puissances
c         croissantes
c     da : degre de a
c     b,db : definitions similaires a a,da pour le polynome b
c     f : tableau contenant apres execution le pgcd du couple (a,b)
c         range a partir de f(1) si ip=0 ou de f(da+2) si ip=1
c     df : vecteur contenant apres execution le degre
c          du pgcd  range en df(ip+1)
c     v : tableau contenant apres execution
c        les facteurs x,y,x1,y1,ranges respectivement a partir de :
c        * v(1),v(2+db),v(3+da+db),v(4+da+2*db) si ip=0
c        * v(3+da+db),v(4+da+2*db),v(1),v(2+db) si ip=1
c     dv : tableau  contenant apres execution les degres
c          polynomes x,y,x1,y1 ranges respectivement en:
c          dv(1,1),dv(2,1),dv(1,2),dv(2,2)  si ip=0
c          dv(1,2),dv(2,2),dv(1,1),dv(2,1) si ip=1
c     ip : flag de sortie signifiant s'il faut ou non permuter les
c          resultats
c          voir ci dessus
c!reference
c     V. kucera  discrete linear control (the polynomial equation
c     approach)
c     John Wiley & son
c!auteur
c     serge steer inria 1986
c!sous programmes appeles
c     dasum dcopy dset (blas)
c!
c
c     Copyright INRIA
      double precision a(*),b(*),f(*),v(*)
      integer da,db,df(2),dv(2,2),ip
c
      double precision dasum,x(2),lambda,fact,c(2),eps,dlamch
      double precision xs(2)
      integer jv(2),jf(2),inc,k,k1,k2,na,nb,n,l1,l2,i
c
      eps=dlamch('p')*10.0d+0
      ip=0
      xs(1)=dasum(da+1,a,1)
      xs(2)=dasum(db+1,b,1)
      x(1)=xs(1)
      x(2)=xs(2)
      if(xs(1).eq.0.0d+0) xs(1)=1.0d+0
      if(xs(2).eq.0.0d+0) xs(2)=1.0d+0
      na=da+1
   10 na=na-1
      if(abs(a(na+1)).le.eps*abs(xs(1)).and.na.ge.1) goto 10
      nb=db+1
   11 nb=nb-1
      if(abs(b(nb+1)).le.eps*abs(xs(2)).and.nb.ge.1) goto 11
c
c initialisation
c
      jf(1)=1
      jf(2)=da+2
      call dset(da+db+2,0.0d+0,f,1)
      call dcopy(na+1,a,1,f(1),1)
      call dscal(na+1,1.0d+0/xs(1),f(1),1)
      call dcopy(nb+1,b,1,f(jf(2)),1)
      call dscal(nb+1,1.0d+0/xs(2),f(jf(2)),1)
      df(1)=na
      df(2)=nb
c
      call dset(2*(da+db+2),0.0d+0,v,1)
      jv(1)=1
      jv(2)=3+db+da
      inc=db+1
      v(1)=1.0d+0
      v(jv(2)+inc)=1.0d+0
      dv(1,1)=0
      dv(2,1)=0
      dv(1,2)=0
      dv(2,2)=0
      c(1)=1.0d+0
      c(2)=1.0d+0
c
      k1=1
      k2=2
c
      if(abs(x(1)).le.eps*abs(x(2))) goto 35
      if(abs(x(2)).le.eps*abs(x(1))) goto 50
      x(1)=1.0d+0
      x(2)=1.0d+0
c algorithme d'euclide
c
   20 CRES=df(k1)-df(k2)
      if (CRES .lt. 0) then 
         goto 22
      elseif (CRES .eq. 0) then 
         goto 21
      else
         goto 23
      endif
   21 if(abs(f(jf(k1)+df(k1))).lt.abs(f(jf(k2)+df(k2)))) goto 23
   22 k1=3-k1
      k2=3-k2
   23 continue
c test degre  du diviseur
      fact=(c(1)+c(2))
      fact=fact*fact
c      if(x(k2)+fact.le.fact) then
      if(abs(x(k2)).le.eps*fact) then
                                  df(k2)=0
                                  goto 40
      endif
      n=df(k2)+1
      l2=n+jf(k2)
   24 l2=l2-1
      if(n.eq.0) goto 30
      n=n-1
      if(abs(f(l2)).le.eps*abs(x(k1))) goto 24
      df(k2)=n
c calcul de la transf elem
      lambda=f(jf(k1)+df(k1))/f(jf(k2)+df(k2))
      n=df(k1)-df(k2)
      fact=sqrt(lambda*lambda+1.0d+0)
c on l'applique a droite a f
      l2=jf(k2)
      l1=jf(k1)+n
      do 25 i=0,df(k2)
      f(l1+i)=f(l1+i)-lambda*f(l2+i)
   25 continue
c on l'applique a v
      l2=jv(k2)
      l1=jv(k1)+n
      c(k1)=0.0d+0
      do 28 k=1,2
        if(dv(k,k2).eq.0.and.v(l2).eq.0.0d+0) goto 27
        do 26 i=0,dv(k,k2)
   26   v(l1+i)=v(l1+i)-lambda*v(l2+i)
      dv(k,k1)=max(dv(k,k1),n+dv(k,k2))
      c(k1)=c(k1)+dasum(dv(k,k1)+1,v(jv(k1)),1)
   27 l1=l1+inc
      l2=l2+inc
   28 continue
c
      n=df(k1)
      l1=jf(k1)
      x(k1)=dasum(n+1,f(l1),1)
      f(l1+n)=0.0d+0
      df(k1)=n-1
      goto 20
c
c     fin
c
   30 if(k1.eq.1) ip=1
      goto 50
   35 ip=1
      goto 50
   40 if(k2.eq.1) ip=1
c
   50 continue
      l2=jv(2)
      l1=jv(1)
      do 53 k=1,2
        call dscal(dv(k,1)+1,1.0d+0/xs(k),v(l1),1)
        call dscal(dv(k,2)+1,1.0d+0/xs(k),v(l2),1)
      l1=l1+inc
      l2=l2+inc
   53 continue
c
      end
