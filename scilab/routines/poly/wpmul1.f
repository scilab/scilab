C/MEMBR ADD NAME=WPMUL1,SSI=0
c     Copyright INRIA
      subroutine wpmul1(p1r,p1i,d1,p2r,p2i,d2,p3r,p3i)
c!but
c  ce sous programme effectue le produit polynomial:
c
c                p3(x) = p1(x) * p2(x)
c
c     ou p1 ,p2, p3 sont des polynomes a coefficients complexes
c!liste d'appel
c      subroutine wpmul1(p1r,p1i,d1,p2r,p2i,d2,p3r,p3i)
c     double precision p1(d1+1),p2(d2+1),p3(d1+d2+1)
c     integer d1,d2,d3
c
c     p1 : contient les coefficient du premier polynome ranges
c          suivant les puissances croissantes
c     p2 : contient les coefficients du second polynome ranges
c          suivant les puissances croissantes
c     p3 :contient les coefficient du resultats.
c         p3 peut designer la meme adresse que p1 ou p2
c     d1,d2 : degre respectifs des  polynomesp1 et p2
c!sous programmes appeles
c     ddot (blas)
c     min (fortran)
c!
c auteur: s. steer inria.
c var
      double precision p1r(*),p1i(*),p2r(*),p2i(*),p3r(*),p3i(*)
      integer d1,d2,d3
c
      double precision ddot,sr,si
      integer k,l1,l2,l3,l,m3
c ker
      l=1
      l1=d1+1
      l2=d2+1
      d3=d1+d2
      l3=d3+1
c
      m3=min(l1,l2)
      do 10 k=1,m3
      sr=ddot(l,p1r(l1),1,p2r(l2),-1)-ddot(l,p1i(l1),1,p2i(l2),-1)
      si=ddot(l,p1r(l1),1,p2i(l2),-1)+ddot(l,p1i(l1),1,p2r(l2),-1)
      p3r(l3)=sr
      p3i(l3)=si
      l=l+1
      l3=l3-1
      l1=l1-1
      l2=l2-1
   10 continue
      l=l-1
c
      if(l1.eq.0) goto 30
      m3=l1
      do 20 k=1,m3
      sr=ddot(l,p1r(l1),1,p2r,-1)-ddot(l,p1i(l1),1,p2i,-1)
      si=ddot(l,p1r(l1),1,p2i,-1)+ddot(l,p1i(l1),1,p2r,-1)
      p3r(l3)=sr
      p3i(l3)=si
      l1=l1-1
      l3=l3-1
   20 continue
      goto 40
   30 if(l2.eq.0) goto 40
      m3=l2
      do 31 k=1,m3
      sr=ddot(l,p1r,1,p2r(l2),-1)-ddot(l,p1i,1,p2i(l2),-1)
      si=ddot(l,p1r,1,p2i(l2),-1)+ddot(l,p1i,1,p2r(l2),-1)
      p3r(l3)=sr
      p3i(l3)=si
      l2=l2-1
      l3=l3-1
   31 continue
c
   40 if(l3.eq.0) return
      m3=l3
      do 41 k=1,m3
      l=l-1
      sr=ddot(l,p1r,1,p2r,-1)-ddot(l,p1i,1,p2i,-1)
      si=ddot(l,p1r,1,p2i,-1)+ddot(l,p1i,1,p2r,-1)
      p3r(l3)=sr
      p3i(l3)=si
      l3=l3-1
   41 continue
      return
      end
