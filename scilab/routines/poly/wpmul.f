C/MEMBR ADD NAME=WPMUL,SSI=0
c     Copyright INRIA
      subroutine wpmul(p1r,p1i,d1,p2r,p2i,d2,p3r,p3i,d3)
c!purpose
c  ce sous programme effectue le calcul:
c
c                p3(x) = p3(x) + (p1(x) * p2(x))
c
c ou  p1,  p2  et p3 sont des polynomes de degres d1, d2 et d3,
c respectivement, a coefficients complexes
c les coefficients sont ranges consecutivement et par puissance
c croissante
c Tous  les  parametres  sont  d'entree, sauf p3 et d3 qui sont
c d'entree-sortie.
c!
c auteur: c. klimann.
c date: 22 fevrier 1985.
c &&var
      implicit double precision (a-h,o-z)
      double precision p1r(*),p1i(*),p2r(*),p2i(*),p3r(*),p3i(*)
      integer d1,d2,d3
      integer dmax,dmin,dsum
      integer i, k, j, l
      integer e1, e2
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
         p3r(i) = 0.0d+0
         p3i(i) = 0.0d+0
    2 continue
      d3 = dsum
    1 continue
c cas des degres egaux a zero
      if (d1 .eq. 0 .or. d2 .eq. 0) goto 53
c produit et addition
      e1 = 1
      e2 = dmin + 1
      do 10 i = e1, e2
        p3r(i) = p3r(i) + ddot(i,p1r(1),1,p2r(1),-1)
     &                  - ddot(i,p1i(1),1,p2i(1),-1)
        p3i(i) = p3i(i) + ddot(i,p1r(1),1,p2i(1),-1)
     &                  + ddot(i,p1i(1),1,p2r(1),-1)
   10 continue
      k = 1
      if (d1 .eq. d2) goto 21
      e1 = dmin + 2
      e2 = dmax + 1
c si d1 > d2
      if (d1 .lt. d2) goto 25
      do 20 i = e1, e2
        k = k + 1
        p3r(i) = p3r(i) + ddot(dmin+1, p1r(k), 1, p2r(1), -1)
     &                  - ddot(dmin+1, p1i(k), 1, p2i(1), -1)
        p3i(i) = p3i(i) + ddot(dmin+1, p1r(k), 1, p2i(1), -1)
     &                  + ddot(dmin+1, p1i(k), 1, p2r(1), -1)
   20 continue
   21 e1 = dmax + 2
      e2 = dsum + 1
      l = 1
      j = dmin + 1
      do 40 i = e1, e2
        j = j -1
        k = k + 1
        l =l + 1
        p3r(i) = p3r(i) + ddot(j, p1r(k), 1, p2r(l), -1)
     &                  - ddot(j, p1i(k), 1, p2i(l), -1)
        p3i(i) = p3i(i) + ddot(j, p1r(k), 1, p2i(l), -1)
     &                  + ddot(j, p1i(k), 1, p2r(l), -1)
   40 continue
      return
c si d2 > d1
   25 do 30 i = e1, e2
        k = k + 1
        p3r(i) = p3r(i) + ddot(dmin+1, p2r(k), -1, p1r(1), 1)
     &                  - ddot(dmin+1, p2i(k), -1, p1i(1), 1)
        p3i(i) = p3i(i) + ddot(dmin+1, p2r(k), -1, p1i(1), 1)
     &                  + ddot(dmin+1, p2i(k), -1, p1r(1), 1)
   30 continue
      e1 = dmax + 2
      e2 = dsum + 1
      l = 1
      j = dmin + 1
      do 50 i = e1, e2
        j = j -1
        k = k + 1
        l =l + 1
        p3r(i) = p3r(i) + ddot(j, p1r(l), 1, p2r(k), -1)
     &                  - ddot(j, p1i(l), 1, p2i(k), -1)
        p3i(i) = p3i(i) + ddot(j, p1r(l), 1, p2i(k), -1)
     &                  + ddot(j, p1i(l), 1, p2r(k), -1)
   50 continue
      return
c cas des degres egaux a zero
   53 if (d1 .eq. 0 .and. d2 .eq. 0) goto 100
      e1 = 1
      if (d1 .eq.0) goto 60
      e2 = d1 + 1
      do 55 i = e1, e2
        p3r(i) = p3r(i) + p1r(i) * p2r(1) - p1i(i) * p2i(1)
        p3i(i) = p3i(i) + p1r(i) * p2i(1) + p1i(i) * p2r(1)
   55 continue
      return
   60 e2 = d2 + 1
      do 65 i = e1, e2
        p3r(i) = p3r(i) + p2r(i) * p1r(1) - p2i(i) * p1i(1)
        p3i(i) = p3i(i) + p2r(i) * p1i(1) + p2i(i) * p1r(1)
   65 continue
      return
  100 p3r(1) = p3r(1) + p1r(1) * p2r(1) - p1i(1) * p2i(1)
      p3i(1) = p3i(1) + p1r(1) * p2i(1) + p1i(1) * p2r(1)
      return
      end
