      subroutine trbize(nmaxi,maxdeg,nzm,nzero,sm,zm,zzr,zzi)
c!purpose
c bilinear transformation of the zeros and the locations of the extrema
c!
c
      implicit double precision (a-h,o-z)
      dimension nzm(*)
      dimension sm(maxdeg,*), nzero(*)
      double precision zm(maxdeg,*), zzr(*), zzi(*)
      external slamch, dlamch
      real slamch
c
      flma=2.0d+0**(int(slamch('l'))-2)
      flmi=2.0d+0*dlamch('p')
      fa = 1.0d+0
        do 50 j=1,4
        me = nzm(j)
        do 40 i=1,me
          qi = sm(i,j)
          zm(i,j) = 2.0d+0*atan(qi)
          if (j.ne.4) go to 40
          if (qi.ge.flma) go to 10
          if (qi.lt.flmi) go to 20
          qqi = qi*qi
          q = 1.0d+0 + qqi
          zzr(i) = (1.0d+0-qqi)/q
          zzi(i) = 2.0d+0*qi/q
          nu = nzero(i)/2
          fa = fa*q**nu
          go to 40
c
  10      zzr(i) = -1.0d+0
          go to 30
c
  20      zzr(i) = 1.0d+0
   30      zzi(i) = 0.0d+0
  40    continue
  50  continue
c
      sm(nmaxi-1,1) = fa
      return
      end
