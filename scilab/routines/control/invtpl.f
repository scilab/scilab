      subroutine invtpl(t,n,m,tm1,ierr)
c ce sous programme calcule les coefficients de l'inverse d'une matrice
c de toeplitz triangulaire bande
c
c t:vecteur de taille m contenant les elements des la matrice de toeplit
c   ranges par sous diagonales et tel que t(m) soit l'element de la diag
c n:dimension de la matrice de toeplitz
c m:largeur de bande m.le.n
c tm1 vecteur de dimension n  contenant les coeff de l'inverse ranges
c     comme precedemment.
c ierr:code d'erreur si ierr.ne.0 matrice non inversible avec l'ordinate
c
c     Copyright INRIA
      implicit double precision (a-h,o-z)
      dimension t(m),tm1(n)
      data amax/1.0e+30/
      ierr=1
      if(m.le.1) goto 50
      tm=amax
      if(abs(t(m)).lt.1) tm=abs(t(m))*amax
      tm1(n)=1.0d+0
      it=n
      do 10 i=2,m
      it=it-1
      tm1(it)=-ddot(i-1,t(m+1-i),1,tm1(it+1),-1)
c test d'overflow
      if(abs(tm1(it)).ge.tm) return
c division
   10 tm1(it)=tm1(it)/t(m)
      if(n.le.m) goto 30
      nmm=n-m
      do 20 i=1,nmm
      it=it-1
      tm1(it)=-ddot(m-1,t,1,tm1(nmm-i+2),-1)
      if(abs(tm1(it)).ge.tm)return
   20 tm1(it)=tm1(it)/t(m)
c normalisation
   30 do 40 i=1,n
      if(abs(tm1(i)).gt.tm) return
   40 tm1(i)=tm1(i)/t(m)
      ierr=0
      return
   50 continue
      if(t(m).eq.0.0d+0) return
      do 60 i=1,n
   60 tm1(i)=0.0d+0
      tm1(1)=1.0d+0/t(m)
      ierr=0
      return
      end
