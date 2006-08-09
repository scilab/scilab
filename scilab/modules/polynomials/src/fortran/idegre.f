      subroutine idegre(a,majo,nvrai)
c!but
c    calcul du degre d un polynome
c!liste d'appel
c    a=coeff par ordre croissant
c    majo=majorant
c    nvrai=degre calcule
c!
c     Copyright INRIA
      dimension a(*)
      double precision a,test,dasum,an
c
      an=dasum(majo+1,a,1)
      if(an.eq.0.0d+0) goto 20
      if(majo.eq.0) goto 20
      do 10 k=1,majo+1
      kk=majo+2-k
      test=abs(a(kk))/an
      if(1.0d+0+test.ne.1.0d+0) then
      nvrai=kk-1
      return
      endif
  10  continue
  20  nvrai=0
      return
      end
