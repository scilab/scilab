C/MEMBR ADD NAME=DPSPLN,SSI=0
      subroutine dpspln(n,x,f,d,a,fail)
c!but
c     etant donnee une fonction f periodique definie par ses valeurs
c     en n points x1<x2<....<xn,cette routine determine une fonction
c     spline qui interpole les donnees.: s(xi)=fi i=1:n.n doit etre
c     superieur a 4
c!liste d'appel
c     subroutine dpspln(n,x,f,d,a,fail)
c     double precision x(n),f(n),d(n),a(3*n)
c     integer fail,n
c
c     n : nombre de points n>4
c     x : vecteur de longueur n contenant les abscisses
c         les xi doivent etre strictement croissants
c     f: vecteur de longeur n contenant les  f(xi). f(1)=f(n)
c     d : vecteur de longueur n contenant apres execution les
c         derivees de s par rapport a x aux point xi.
c     a : tableau de travail de taille 3*n
c     fail : indicateur de bon deroulement:
c            fail=0 : ok
c            fail=1 : les abscisses ne sont pas strictement croissantes
c            fail=2 : f(1).ne.f(n)
c!
      double precision a,d,f,x,h1,h2,f1,f2,a3n1,p
      integer fail
      dimension x(n),f(n),d(n),a(*)
c
      fail=0
      do 5 i=2,n
      if(x(i)-x(i-1).le.0.0d+0) go to 6
5     continue
      if(f(1).eq.f(n)) go to 10
      fail=2
      return
6     fail=1
      return
c
10    do 30 i=2,n
      h1=1.0d+0/(x(i)-x(i-1))
      f1=f(i-1)
      if(i.eq.n)go to 20
      h2=1.0d+0/(x(i+1)-x(i))
      f2=f(i+1)
      go to 25
20    h2=1.0d+0/(x(2)-x(1))
      f2=f(2)
25    a(3*i-2)=h1
      a(3*i-1)=2.0d+0*(h1+h2)
      a(3*i)=h2
30    d(i)=3.0*(f2*h2*h2+f(i)*(h1*h1-h2*h2)-f1*h1*h1)
c
      n2=n-2
      k=5
      a3n1=a(3*n-1)
      do 50 i=2,n2
      p=a(k+2)/a(k)
      a(k+3)=a(k+3)-p*a(k+1)
      d(i+1)=d(i+1)-p*d(i)
      a(k+2)=-p*a(k-1)
      p=a(k-1)/a(k)
      a3n1=-p*a(k-1)+a3n1
      d(n)=d(n)-p*d(i)
50    k=k+3
      p=(a(k+2)+a(k-1))/a(k)
      a3n1=a3n1-p*(a(k+1)+a(k-1))
      d(n)=(d(n)-p*d(n-1))/a3n1
c
      do 60 i=3,n
      j=n+2-i
60    d(j)=(d(j)-a(3*j)*d(j+1)-a(3*j-2)*d(n))/a(3*j-1)
      d(1)=d(n)
      end
