      subroutine dmprod(flag,a,na,m,n,v,nv)
c!purpose
c     computes the product of the entries of a matrix according to flag
c!calling sequence
c     subroutine dmprod(flag,a,na,m,n,v,nv)
c     double precision a(na,n),v(*)
c     integer na,n,m,nv
c     integer flag
c!parameters
c     flag : indicates operation to perform
c            0 : returns in v(1) the product of all entries of a
c            1 : returns in v(j) the product of jth column of a
c            2 : returns in v(i) the product of ith row of a
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
      double precision t
      integer iv
c
      iv=1
      if(flag.eq.0) then
c     product of all the entries
         t=1.0d0
c         do 10 j=1,n
c            call dvmul(m,a(1,j),1,t,0)
c 10      continue
         call dvmul(m*n,a(1,1),1,t,0)
         v(1)=t
      elseif(flag.eq.1) then
         do 20 j=1,n
            t=1.0d0
            call dvmul(m,a(1,j),1,t,0)
            v(iv)=t
            iv=iv+nv
 20      continue
      elseif(flag.eq.2) then
         do 30 i=1,m
            t=1.0d0
            call dvmul(n,a(i,1),m,t,0)
            v(iv)=t
            iv=iv+nv
 30      continue
      endif
      return
      end
