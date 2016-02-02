c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA
c 
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
c
      subroutine wmsum(flag,ar,ai,na,m,n,vr,vi,nv)
c!purpose
c     computes the sum of the entries of a complexmatrix according to flag
c!calling sequence
c     subroutine wmsum(flag,ar,ai,na,m,n,vr,vi,nv)
c     double precision ar(na,n),ai(na,n),vr(*),vi(*)
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
c
      double precision ar(na,n),ai(na,n),vr(*),vi(*)
      integer na,n,m,nv
      integer flag
c
      double precision tr,ti,dsum
      integer iv
c
      iv=1
      if(flag.eq.0) then
c     sum of all the entries
         tr=0.0d0
         ti=0.0d0
         do 10 j=1,n
            tr=tr+dsum(m,ar(1,j),1)
            ti=ti+dsum(m,ai(1,j),1)
 10      continue
         vr(1)=tr
         vi(1)=ti
      elseif(flag.eq.1) then
         do 20 j=1,n
            tr=dsum(m,ar(1,j),1)
            ti=dsum(m,ai(1,j),1)
            vr(iv)=tr
            vi(iv)=ti
            iv=iv+nv
 20      continue
      elseif(flag.eq.2) then
         do 30 i=1,m
            tr=dsum(n,ar(i,1),m)
            ti=dsum(n,ai(i,1),m)
            vr(iv)=tr
            vi(iv)=ti
            iv=iv+nv
 30      continue
      endif
      return
      end
