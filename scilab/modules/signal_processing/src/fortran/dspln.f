C/MEMBR ADD NAME=DSPLN,SSI=0
      subroutine dspln(n,x,f,d,a,fail)
c!but
c     etant donnee une fonction f definie par ses valeurs en n points
c     x1<x2<....<xn,cette routine determine une fonction spline qui
c     interpole les donnees.: s(xi)=fi i=1:n.n doit etre superieur a 3
c!liste d'appel
c     subroutine dspln(n,x,f,d,a,fail)
c     double precision x(n),f(n),d(n),a(3*n)
c     integer fail,n
c
c     n : nombre de points n>3
c     x : vecteur de longueur n contenant les abscisses
c         les xi doivent etre strictement croissants
c     f: vecteur de longeur n contenant les  f(xi)
c     d : vecteur de longueur n contenant apres execution les
c         derivees de s par rapport a x aux point xi.
c     a : tableau de travail de taille 3*n
c     fail : indicateur de bon deroulement:
c            fail=0 : ok
c            fail=1 : les abscisses ne sont pas strictement croissantes
c!
      double precision a,d,f,h1,h2,p,x
      integer fail ,n
      dimension x(n),f(n),d(n),a(*)
c
      fail=0
      do 5 i=2,n
      if(x(i).gt.x(i-1))goto 5
      fail=1
      return
5     continue
c
      j=2
      i=1
      goto 35
10    do 30 i=2,n-1
      h1=1.0d+0/(x(i)-x(i-1))
      h2=1.0d+0/(x(i+1)-x(i))
      a(3*i-2)=h1
      a(3*i-1)=2.0d+0*(h1+h2)
      a(3*i)=h2
      d(i)=3.0d+0*((f(i+1)-f(i))*h2*h2+(f(i)-f(i-1))*h1*h1)
30    continue
      j=n-1
      i=n
35    h1=1.0d+0/(x(j)-x(j-1))
      h2=1.0d+0/(x(j+1)-x(j))
      a(3*i-2)=h1*h1
      a(3*i-1)=h1*h1-h2*h2
      a(3*i)=-h2*h2
      d(i)=2.0d+0*(h1*h1*h1*(f(j)-f(j-1))+h2*h2*h2*(f(j)-f(j+1)))
      if(i.eq.1) goto 10
c
      p=a(4)/a(1)
      a(5)=a(5)-p*a(2)
      a(6)=a(6)-p*a(3)
      d(2)=d(2)-p*d(1)
      do 50 i=3,n
      k=3*i-4
      p=a(k+2)/a(k)
      a(k+3)=a(k+3)-p*a(k+1)
      d(i)=d(i)-p*d(i-1)
      if(i.ne.n-1)go to 50
      p=a(k+5)/a(k)
      a(k+5)=a(k+6)-p*a(k+1)
      a(k+6)=a(k+7)
      d(n)=d(n)-p*d(n-2)
50    continue
      d(n)=d(n)/a(3*n-1)
c
      do 60 i=3,n
      j=n+2-i
60    d(j)=(d(j)-a(3*j)*d(j+1))/a(3*j-1)
      d(1)=(d(1)-d(2)*a(2)-d(3)*a(3))/a(1)
      return
      end
