c     Examples for schur 
C     when schur is called with option 'c'
c     the function folhp1 is called
c--------------------------------------------
c     Copyright INRIA
      integer function folhp1(ls,alpha,beta,s,p)
      integer ls
      double precision alpha,beta,s,p
c!purpose
c      this routine checks if
c    the real root alpha/beta lies in the open left half plane
c       (if ls=1)
c     the complex conjugate roots with sum s and product p lie
c     in the open left half plane (if ls=2)
c     if so, folhp1=1, otherwise, folhp1=-1
c      in this function the parameter p is not referenced
c
c!calling sequence
c
c     integer function folhp1(ls,alpha,beta,s,p)
c     integer ls
c     double precision alpha,beta,s,p
c!auxiliary routines
c     none
c!
      folhp1=-1
      if(ls.eq.2) go to 2
      if(alpha*beta.lt.0.0d+0) folhp1=1
      return
 2    if(s.lt.0.0d+0) folhp1=1
      return
      end
c--------------------------------------------
