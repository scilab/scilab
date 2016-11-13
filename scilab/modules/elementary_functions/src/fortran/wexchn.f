c Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c Copyright (C) INRIA - Serge STEER
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
      subroutine wexchn(ar, ai, vr, vi, n, l, fail, na, nv)
c!purpose
c
c     given the upper triangular complex matrix a ,wexchn produce a
c     unitary transformation which exchange the two consecutive blocks
c     starting at a(l,l),along with their eigenvalues.
c     the transformation is accumulated in v.
c!calling sequence
c
c     subroutine exchng(ar, ai, vr, vi, n, l, fail, na, nv)
c
c     integer l, na, nv
c     double precision ar, ai, vr, vi
c     dimension ar(na,n) , ai(na,n) ,vr(nv,n) ,vi(nv,n)
c     logical fail
c
c     starred parameters are altered by the subroutine
c
c        *ar,ai  the matrix whose blocks are to be
c                interchanged.
c        *vr,vi  the array into which the transformations
c                are to re accumulated.
c         n      the order of the matrix a.
c         l      the position of the blocks.
c       *fail    a logical variable which is false on a
c                normal return.  if thirty iterations were
c                performed without convergence, fail is set
c                to true and the element
c                a(l+b2,l+b2-1) cannot be assumed zero.
c         na     the first dimension of the array a.
c         nv     the first dimension of the array v.
c
c!auxiliary routines
c     max sqrt abs (fortran)
c!originator
c     steer i.n.r.i.a  from routine exchng
c
      integer l,na,nv
      double precision ar,ai,vr,vi
      dimension ar(na, n),  ai(na, n), vr(nv, n), vi(nv, n)
      logical fail
c
c     internal variables.
c
      double precision pr,pi,qr,qi,r,sr,si,tr,ti,zero
      integer i, j, l1
      data zero /0.0d+0/
      l1 = l + 1
c
      fail = .false.
c
c         interchange 1x1 and 1x1 blocks.
c
      qr = ar(l1,l1) - ar(l,l)
      pr = ar(l,l1)
      qi = ai(l1,l1) - ai(l,l)
      pi = ai(l,l1)
      r = max(abs(pr),abs(pi),abs(qr),abs(qi))
      if (r.eq.zero) return
      pr = pr/r
      qr = qr/r
      pi = pi/r
      qi = qi/r
      r = sqrt(pr*pr + pi*pi + qr*qr + qi*qi)
      pr = pr/r
      qr = qr/r
      pi = pi/r
      qi = qi/r
      do 10 j = l,n
      sr = ar(l,j)
      si = ai(l,j)
      tr = ar(l1,j)
      ti = ai(l1,j)
         ar(l,j) = pr*sr + pi*si + qr*tr + qi*ti
         ai(l,j) = pr*si - pi*sr + qr*ti - qi*tr
         ar(l1,j) = pr*tr - pi*ti - qr*sr + qi*si
         ai(l1,j) = pr*ti + pi*tr - qr*si - qi*sr
   10 continue
      do 20 i = 1,l1
         sr = ar(i,l)
         si = ai(i,l)
         tr = ar(i,l1)
         ti = ai(i,l1)
         ar(i,l) = pr*sr + qr*tr - pi*si - qi*ti
         ai(i,l) = pi*sr + qi*tr + pr*si + qr*ti
         ar(i,l1) = pr*tr + pi*ti - qr*sr - qi*si
         ai(i,l1) = pr*ti - pi*tr - qr*si + qi*sr
   20 continue
      do 30 i = 1,n
         sr = vr(i,l)
         si = vi(i,l)
         tr = vr(i,l1)
         ti = vi(i,l1)
         vr(i,l) = pr*sr + qr*tr - pi*si - qi*ti
         vi(i,l) = pi*sr + qi*tr + pr*si + qr*ti
         vr(i,l1) = pr*tr + pi*ti - qr*sr - qi*si
         vi(i,l1) = pr*ti - pi*tr - qr*si + qi*sr
   30 continue
      ar(l1,l) = zero
      ai(l1,l) = zero
      return
      end
