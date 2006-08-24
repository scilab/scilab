      subroutine wmprod(flag,ar,ai,na,m,n,vr,vi,nv)
c!purpose
c     computes the product of the entries of a complex matrix according to flag
c!calling sequence
c     subroutine wmprod(flag,ar,ai,na,m,n,vr,vi,nv)
c     double precision ar(na,n),ai(na,n),vr(*),vi(*)
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
c     v    : array containing the result, 
c            vr (resp vi) may be confused with a row or
c            a column of the ar (resp ai) matrix
c            if flag==0 size(v)>=1
c            if flag==1 size(v)>=n*nv
c            if flag==1 size(v)>=m*nv
c     nv   : increment between to consecutive entries ov v
c!
c     Copyright INRIA
      double precision ar(na,n),ai(na,n),vr(*),vi(*)
      integer na,n,m,nv
      integer flag
c
      double precision tr,ti
      integer iv
c
      iv=1
      if(flag.eq.0) then
c     product of all the entries
         tr=1.0d0
         ti=0.0d0
         do 10 j=1,n
            call wvmul(m,ar(1,j),ai(1,j),1,tr,ti,0)
 10      continue
         vr(1)=tr
         vi(1)=ti
      elseif(flag.eq.1) then
         do 20 j=1,n
            tr=1.0d0
            ti=0.0d0
            call wvmul(m,ar(1,j),ai(1,j),1,tr,ti,0)
            vr(iv)=tr
            vi(iv)=ti
            iv=iv+nv
 20      continue
      elseif(flag.eq.2) then
         do 30 i=1,m
            tr=1.0d0
            ti=0.0d0
            call wvmul(n,ar(i,1),ai(i,1),m,tr,ti,0)
            vr(iv)=tr
            vi(iv)=ti
            iv=iv+nv
 30      continue
      endif
      return
      end
