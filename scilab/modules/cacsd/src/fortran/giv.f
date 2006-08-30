      subroutine giv(sa,sb,sc,ss)
c     Copyright INRIA
      double precision sa,sb,sc,ss
c!purpose
c      this routine constructs the givens transformation
c
c                ( sc  ss )
c            g = (        ),   sc**2+ss**2 = 1. ,
c                (-ss  sc )
c
c      which zeros the second entry of the 2-vector (sa,sb)**t
c      this routine is a modification of the blas routine srotg
c      (algorithm 539) in order to leave the arguments sa and sb
c      unchanged
c
c!calling sequence
c
c     subroutine giv(sa,sb,sc,ss)
c     double precision sa,sb,sc,ss
c!auxiliary routines
c     sqrt abs (fortran)
c!
      double precision r,u,v
      if(abs(sa).le.abs(sb)) go to 10
c* here abs(sa) .gt. abs(sb)
      u=sa+sa
      v=sb/u
      r=sqrt(0.250d+0+v*v)*u
      sc=sa/r
      ss=v*(sc+sc)
      return
c* here abs(sa) .le. abs(sb)
  10  if(sb.eq.0.0d+0) go to 20
      u=sb+sb
      v=sa/u
      r=sqrt(0.250d+0+v*v)*u
      ss=sb/r
      sc=v*(ss+ss)
      return
c* here sa = sb = 0.
  20  sc=1.0d+0
      ss=0.0d+0
      return
      end
