      subroutine dmsum(flag,a,na,m,n,v,nv)
c!purpose
c     computes the sum of the entries of a matrix according to flag
c!calling sequence
c     subroutine dmsum(flag,a,na,m,n,v,nv)
c     double precision a(na,n),v(*)
c     integer na,n,m,nv
c     integer flag
c!parameters
c     flag : indicates operation to perform
c            0 : returns in v(1) the sum of all entries of a
c            1 : returns in v(j) the sum of jth column of a
c            2 : returns in v(i) the sum of ith row of a
c     a    : array containing the a matrix
c     na   : a matrix leading dimension
c     m    : a matrix row dimension
c     n    : a matrix column dimension
c     v    : array containing the result, may be confused with a row or
c            a column of the a matrix
c            if flag==0 size(v)>=1
c            if flag==1 size(v)>=n*nv
c            if flag==1 size(v)>=m*nv
c     nv   : increment between to consecutive entries ov v
c!
c     Copyright INRIA
      double precision a(na,n),v(nv)
      integer na,n,m,nv
      integer flag
c
      double precision t,dsum
      integer iv
c
      iv=1
      if(flag.eq.0) then
         v(1) = dsum(m*n,a(1,1),1)
c     sum of all the entries
c         t=0.0d0
c         do 10 j=1,n
c            t=t+dsum(m,a(1,j),1)
c 10      continue
c         v(1)=t
      elseif(flag.eq.1) then
         do 20 j=1,n
            t=dsum(m,a(1,j),1)
            v(iv)=t
            iv=iv+nv
 20      continue
      elseif(flag.eq.2) then
         do 30 i=1,m
            t=dsum(n,a(i,1),m)
            v(iv)=t
            iv=iv+nv
 30      continue
      endif
      return
      end
