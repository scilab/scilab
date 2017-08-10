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
C/MEMBR ADD NAME=WCERR,SSI=0
c
      subroutine wcerr(ar,ai,w,ia,n,ndng,m,maxc)
c!purpose
c     wcerr evaluate the error introduced by pade
c     approximant and normalise the complex matrix a accordingly
c!calling sequence
c
c     subroutine wcerr(ar,ai,w,ia,n,ndng,m,maxc)
c
c     ar,ai    : array containing the matrix a
c
c     w        : work space array of size 4*n*n + 2*n
c
c     ia       : leading dimension of array a
c
c     n        : size of matrix a
c
c     ndng     : degree of pade approximant
c
c     m        :  the factor of normalization is 2**(-m)
c
c     maxc     : maximum admissible for m
c
c!auxiliary routines
c     wmmul dmcopy  gdcp2i (blas.extension)
c     dset dcopy (blas)
c     ddot (blas)
c     abs real dble (fortran)
c!
c     w  tableau de travail de taille 4*n*n+2*n
c
      integer ia,n,ndng,m,maxc
      double precision ar,ai,w
      dimension ar(ia,n),ai(ia,n),w(*)
c
c internal variables
      integer k,mm,i,j,mt,kr,ki,ker,kei,kwr,kwi
      double precision norm,alpha,zero,two,norm1,one,ddot
      logical itab(15)
c
      data zero, one, two /0.0d+0,1.0d+0,2.0d+0/
c
c
      n2=n*n
      kr=1
      ki=kr+n2
      ker=ki+n2
      kei=ker+n2
      kwr=kei+n2
      kwi=kwr+n
      k = 2*ndng
      call wmmul(ar,ai,ia,ar,ai,ia,w(ker),w(kei),n,n,n,n)
      call gdcp2i(k, itab, mt)
      if (.not.itab(1)) go to 30
      norm = zero
      do 20 i=1,n
         alpha = zero
         do 10 j=1,n
            alpha = alpha + abs(ar(i,j)) + abs(ai(i,j))
   10    continue
         if (alpha.gt.norm) norm = alpha
   20 continue
      call dmcopy(ar,ia,w(kr),n,n,n)
      call dmcopy(ai,ia,w(ki),n,n,n)
      go to 40
   30 call dset(n2,0.0d+0,w(kr),1)
      call dset(n,1.0d+0,w(kr),n+1)
      call dset(n2,0.0d+0,w(ki),1)
   40 if (mt.eq.1) go to 110
      do 100 i1=2,mt
         do 70 j=1,n
            l = 0
            do 50 i=1,n
               w(kwr-1+i) = ddot(n,w(kr-1+j),n,w(ker+l),1)
     1                      -ddot(n,w(ki-1+j),n,w(kei+l),1)
               w(kwi-1+i) = ddot(n,w(kr-1+j),n,w(kei+l),1)
     1                      +ddot(n,w(ki-1+j),n,w(ker+l),1)
               l = l + n
   50       continue
            call dcopy(n,w(kwr),1,w(kr-1+j),n)
            call dcopy(n,w(kwi),1,w(ki-1+j),n)
   70    continue
         if (.not.itab(i1)) go to 100
         norm1 = zero
         do 90 i=1,n
            alpha = zero
            l = i - 1
            do 80 j=1,n
               alpha = alpha + abs(w(kr+l)) + abs(w(ki+l))
               l = l + n
   80       continue
            if (alpha.gt.norm1) norm1 = alpha
   90    continue
         norm = norm*norm1
  100 continue
  110 continue
      norm = norm/dble(real(k+1))
      do 120 i=1,ndng
         norm = norm/dble(real((k-i+1)**2))
  120 continue
      norm = 8.0d+0*norm
      mm = 0
  130 if (norm+one .le. one) go to 140
      mm = mm + 1
      alpha = two**mm
      norm = norm/alpha
      if ((mm+m).gt.maxc) go to 140
      go to 130
  140 continue
      alpha = (two**mm)
      do 160 i=1,n
         do 150 j=1,n
            ar(i,j) = ar(i,j)/alpha
            ai(i,j) = ai(i,j)/alpha
  150    continue
  160 continue
      m = m + mm
      return
      end
