      integer function find(lsize,alpha,beta,s,p)
c     Copyright INRIA
      integer lsize
      double precision alpha,beta,s,p
c!purpose
c      this function checks if
c       the real root alpha/beta lies in the unit disc (if lsize=1)
c       the complex conjugate roots with sum s and product p lie
c       in the unit disc (if lsize=2)
c      if so, find=1, otherwise find=-1
c      in this function the parameter s is not referenced
c
c!calling sequence
c
c     integer function find(lsize,alpha,beta,s,p)
c     integer lsize
c     double precision alpha,beta,s,p
c!auxiliary routines
c     abs (fortran)
c!
      find=-1
      if(lsize.eq.2) go to 2
      if(abs(alpha).lt.abs(beta)) find=1
      return
  2   if(abs(p).lt.1.0d+0) find=1
      return
      end
