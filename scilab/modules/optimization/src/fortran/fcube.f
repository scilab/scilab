      subroutine fcube(t,f,fp,ta,fa,fpa,tlower,tupper)
c     Copyright INRIA
      implicit double precision (a-h,o-z)
c
c           Using f and fp at t and ta, computes new t by cubic formula
c           safeguarded inside [tlower,tupper].
c
      z1=fp+fpa-3.d0*(fa-f)/(ta-t)
      b=z1+fp
c
c              first compute the discriminant (without overflow)
c
      if (dabs(z1).le.1.d0) then
          discri=z1*z1-fp*fpa
        else
          discri=fp/z1
          discri=discri*fpa
          discri=z1-discri
          if (z1.ge.0.d0 .and. discri.ge.0.d0) then
              discri=dsqrt(z1)*dsqrt(discri)
              go to 200
          endif
          if (z1.le.0.d0 .and. discri.le.0.d0) then
              discri=dsqrt(-z1)*dsqrt(-discri)
              go to 200
          endif
          discri=-1.d0
      endif
      if (discri.lt.0.d0) then
          if (fp.lt.0.d0) t=tupper
          if (fp.ge.0.d0) t=tlower
          go to 990
      endif
c
c       discriminant nonnegative, stable solution formula
c
      discri=dsqrt(discri)
  200 if (t-ta.lt.0.d0) discri=-discri
      sign=(t-ta)/dabs(t-ta)
      if (b*sign.gt.0.) then
          anum=(ta-t)*fp
          den=b+discri
        else
          den=z1+b+fpa
          anum=(ta-t)*(b-discri)
      endif
c
c               now compute the ratio (without overflow)
c
      if (dabs(den).ge.1.d0) then
          t=t+anum/den
        else
          if (dabs(anum).lt.(tupper-tlower)*dabs(den)) then
              t=t+anum/den
            else
              if (fp.lt.0.d0) t=tupper
              if (fp.ge.0.d0) t=tlower
          endif
      endif
c
c                       finally, safeguard
c
      t=dmax1(t,tlower)
      t=dmin1(t,tupper)
  990 return
      end
