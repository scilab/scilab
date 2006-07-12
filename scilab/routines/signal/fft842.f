      subroutine fft842(in, n, x, y,err)
c          programe de transformee de fourier rapide pour n=2**m
c             -  algorithme de cooley-tukey   -
c!entrees
c
c  ce logiciel traite des entrees complexes sous forme de deux tableaux
c
c -1] tableau representant les coefficients reels      :x(i)
c -1] tableau representant les coefficients complexes  :y(i)
c
c options :
c           directe  ----> code=0
c           indirecte----> code=1
c
c          si la dimension (suite de nombres complexes n'est pas de
c          dimension 2**n tel que  0<n<15 )err est positionne a 1 .
c
c
      integer ij,ji,m,nt,n2pow,fn,n8pow,nxtlt,lengt
      integer err
      double precision pi2,p7
      double precision pi,r,fi
      double precision x(*), y(*)
      dimension l(15)
      equivalence(l15,l(1)),(l14,l(2)),(l13,l(3)),(l12,l(4)),
     & (l11,l(5)),(l10,l(6)),(l9,l(7)),(l8,l(8)),(l7,l(9)),
     & (l6,l(10)),(l5,l(11)),(l4,l(12)),(l3,l(13)),(l2,l(14)),
     & (l1,l(15))
c
      pi2=8.0d+0*atan(1.0d+0)
      pi=acos(-1.0d+0)
      p7=1.0d+0/sqrt(2.0d+0)
      do 10 i=1,15
      m=i
      nt=2**i
      if(n.eq.nt)goto 20
10    continue
c
c          erreur puissance de 2 non respecte)==> err=1
c
      err=1
      return
c         aucune erreur===> err=0
20    err=0
      n2pow=m
      nthpo=n
      fn=nthpo
      if(in.eq.1)goto 40
      do 30 i=1,nthpo
      y(i)=-y(i)
30    continue
40    n8pow=n2pow/3
      if(n8pow.eq.0)goto 60
c
c developement de l'algoritme en base 8 si ...
c
      do 50 ipass=1,n8pow
      nxtlt=2**(n2pow-3*ipass)
      lengt=8*nxtlt
      call r8tx(nxtlt, nthpo, lengt, x(1), x(nxtlt+1), x(2*nxtlt+1),
     & x(3*nxtlt+1), x(4*nxtlt+1), x(5*nxtlt+1), x(6*nxtlt+1),
     & x(7*nxtlt+1), y(1), y(nxtlt+1), y(2*nxtlt+1), y(3*nxtlt+1),
     & y(4*nxtlt+1), y(5*nxtlt+1), y(6*nxtlt+1), y(7*nxtlt+1))
50    continue
c
c is there a four factor left
c
60    CRES=n2pow-3*n8pow-1
      if (CRES .lt. 0) then
         goto 90
      elseif (CRES .eq. 0) then
         goto 70
      else
         goto 80
      endif
c
c iteration de l'algoritme en base 2
c
70    call r2tx(nthpo, x(1), x(2), y(1), y(2))
      goto 90
c
c iteration de l'algoritme en base 4
c
80    call r4tx(nthpo, x(1), x(2), x(3), x(4), y(1), y(2), y(3), y(4))
c
90    do 110 j=1,15
      l(j)=1.0d+0
      CRES=j-n2pow
      if (CRES .le. 0) goto 100
      goto 110
100   l(j)=2.0d+0**(n2pow+1-j)
110   continue
      ij=1
      do 130 j1=1,l1
      do 130 j2=j1,l2,l1
      do 130 j3=j2,l3,l2
      do 130 j4=j3,l4,l3
      do 130 j5=j4,l5,l4
      do 130 j6=j5,l6,l5
      do 130 j7=j6,l7,l6
      do 130 j8=j7,l8,l7
      do 130 j9=j8,l9,l8
      do 130 j10=j9,l10,l9
      do 130 j11=j10,l11,l10
      do 130 j12=j11,l12,l11
      do 130 j13=j12,l13,l12
      do 130 j14=j13,l14,l13
      do 130 ji=j14,l15,l14
      CRES=ij-ji
      if (CRES .ge. 0) then
         goto 130
      else
         goto 120
      endif
120   r=x(ij)
      x(ij)=x(ji)
      x(ji)=r
      fi=y(ij)
      y(ij)=y(ji)
      y(ji)=fi
130   ij=ij+1
      if(in.eq.1)goto 150
      do 140 i=1,nthpo
      y(i)=-y(i)
140   continue
      goto 170
150   do 160 i=1,nthpo
      x(i)=x(i)/fn
      y(i)=y(i)/fn
160   continue
170   return
      end
