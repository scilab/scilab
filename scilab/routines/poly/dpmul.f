C/MEMBR ADD NAME=DPMUL,SSI=0
c     Copyright INRIA
      subroutine dpmul(p1,d1,p2,d2,p3,d3)
c!purpose
c  ce sous programme effectue le calcul:
c
c                p3(x) = p3(x) + (p1(x) * p2(x))
c
c ou  p1,  p2  et p3 sont des polynomes de degres d1, d2 et d3,
c respectivement. Ils sont classes en ordre croissant.
c Tous  les  parametres  sont  d'entree, sauf p3 et d3 qui sont
c d'entree-sortie.
c!
c auteur: c. klimann.
c date: 22 fevrier 1985.
c &&var
      implicit double precision (a-h,o-z)
      double precision dlamch,ddot
      double precision p1(*),p2(*),p3(*)
      integer d1,d2,d3
      integer dmax,dmin,dsum
      integer i, k, j, l
      integer e1, e2
      eps=dlamch('p')
c &&ker
      dsum = d1 + d2
c fixation de dmax et dmin
      dmax = d1
      if (d2 .gt. d1) dmax = d2
      dmin = dsum - dmax
c fixation de d3
      if (d3 .ge. dsum) goto 1
      e1 = d3+2
      e2 = dsum +1
      do 2 i = e1 , e2
         p3(i) = 0.0d+0
    2 continue
      d3 = dsum
    1 continue
c cas des degres egaux a zero
      if (d1 .eq. 0 .or. d2 .eq. 0) goto 53
c produit et addition
      e1 = 1
      e2 = dmin + 1
      do 10 i = e1, e2
         w=ddot(i,p1(1),1,p2(1),-1)
         w1=p3(i)+w
         if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
            p3(i) = w1
         else
            p3(i)=0.0d+0
         endif
   10 continue
      k = 1
      if (d1 .eq. d2) goto 21
      e1 = dmin + 2
      e2 = dmax + 1
c si d1 > d2
      if (d1 .lt. d2) goto 25
      do 20 i = e1, e2
        k = k + 1
        w=ddot(dmin+1, p1(k), 1, p2(1), -1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   20 continue
   21 e1 = dmax + 2
      e2 = dsum + 1
      l = 1
      j = dmin + 1
      do 40 i = e1, e2
        j = j -1
        k = k + 1
        l =l + 1
        w=ddot(j, p1(k), 1, p2(l), -1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   40 continue
      return
c si d2 > d1
   25 do 30 i = e1, e2
        k = k + 1
        w=ddot(dmin+1, p2(k), -1, p1(1), 1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   30 continue
      e1 = dmax + 2
      e2 = dsum + 1
      l = 1
      j = dmin + 1
      do 50 i = e1, e2
        j = j -1
        k = k + 1
        l =l + 1
        w=ddot(j, p1(l), 1, p2(k), -1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   50 continue
      return
c cas des degres egaux a zero
   53 if (d1 .eq. 0 .and. d2 .eq. 0) goto 100
      e1 = 1
      if (d1 .eq.0) goto 60
      e2 = d1 + 1
      do 55 i = e1, e2
        w=p1(i) * p2(1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   55 continue
      return
   60 e2 = d2 + 1
      do 65 i = e1, e2
         w=p2(i) * p1(1)
        w1=p3(i)+w
        if(abs(w1).gt.eps*max(abs(p3(i)),abs(w))) then
           p3(i) = w1
        else
           p3(i)=0.0d+0
        endif
   65 continue
      return
  100 p3(1) = p3(1) + p1(1) * p2(1)
      return
      end
