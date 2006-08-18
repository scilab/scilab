C/MEMBR ADD NAME=WDEGRE,SSI=0
c     Copyright INRIA
      subroutine wdegre(ar,ai,majo,nvrai)
c    calcul du degre d un polynome a coefficients complexes
c    a=ar+i*ai=coeffs par ordre croissant
c    majo=majorant du degre
c    nvrai=degre calcule
      dimension ar(*),ai(*)
      double precision ar,ai,test
      if(majo.eq.0) goto 20
      do 10 k=1,majo+1
      kk=majo+2-k
      test=abs(ar(kk))+abs(ai(kk))
      if(1.0d+0+test.ne.1.0d+0) then
      nvrai=kk-1
      return
      endif
  10  continue
  20  nvrai=0
      return
      end
